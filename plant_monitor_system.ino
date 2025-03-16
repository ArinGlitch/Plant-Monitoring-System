#include <Wire.h>
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <Adafruit_AHTX0.h>  
#include <SPI.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h> 

// **ESP32-WROOM TFT Display Pins**
#define TFT_DC   12  
#define TFT_CS   13  
#define TFT_MOSI 14  
#define TFT_CLK  27  
#define TFT_RST  0   
#define TFT_MISO 0   

// **I2C Pins for AHT10 (Temp & Humidity Sensor)**
#define AHT10_SDA 21  
#define AHT10_SCL 22  

// **Soil Moisture Sensor KS0049**
#define SOIL_SENSOR_PIN 34  

// **WiFi Credentials**
const char* ssid = "your_ssid";        
const char* password = "your_password"; 

// **Gmail SMTP Settings**
#define SMTP_SERVER "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "your_email@gmail.com"  
#define AUTHOR_PASSWORD "app_password"  
#define RECIPIENT_EMAIL "recipient_mail@gmail.com"

// **ESP-Mail Client Setup**
SMTPSession smtp;
ESP_Mail_Session session;
SMTP_Message message;

// **Create Display Object**
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

// **Create AHT10 Sensor Object**
Adafruit_AHTX0 aht10;

// **Web Server on Port 80**
WiFiServer server(80);

bool emailSent = false;

// **Log Storage (Stores last 10 logs)**
#define LOG_SIZE 10
String logEntries[LOG_SIZE];
int logIndex = 0;

void setup() {
    Serial.begin(115200);
    
    // **Initialize Display**
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST77XX_BLACK);
    tft.setRotation(1);

    // **Initialize I2C for AHT10**
    Wire.begin(AHT10_SDA, AHT10_SCL);
    
    if (!aht10.begin()) {
        Serial.println("AHT10 Sensor NOT Found!");
        tft.setCursor(10, 30);
        tft.setTextColor(ST77XX_RED);
        tft.setTextSize(2);
        tft.print("AHT10 Error!");
        while (1);
    }
    Serial.println("AHT10 Sensor Initialized!");

    // **Connect to WiFi**
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    Serial.print("ESP32 Web Server IP Address: ");
    Serial.println(WiFi.localIP());

    // **Start Web Server**
    server.begin();
    Serial.println("Web Server Started...");

    // **Configure SMTP Session**
    session.server.host_name = SMTP_SERVER;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "smtp.gmail.com";
    session.time.ntp_server = "pool.ntp.org";

    smtp.debug(1);
}

void loop() {
    // **Read Temperature & Humidity**
    sensors_event_t humidity, temp;
    aht10.getEvent(&humidity, &temp);

    // **Read Soil Moisture**
    int soilMoistureRaw = analogRead(SOIL_SENSOR_PIN);
    int soilMoisture = map(soilMoistureRaw, 0, 4095, 0, 100);

    // **Log Data in Serial Monitor & Webpage**
    String logEntry = "Temp: " + String(temp.temperature, 1) + "C | Humidity: " + String(humidity.relative_humidity, 1) + "% | Soil Moisture: " + String(soilMoisture) + "%";
    Serial.println(logEntry);
    
    // **Store Log Entry**
    logEntries[logIndex] = logEntry;
    logIndex = (logIndex + 1) % LOG_SIZE;

    // **Update TFT Display with Sensor Data**
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.print("Temp: ");
    tft.print(temp.temperature, 1);
    tft.print("C");

    tft.setCursor(10, 20);
    tft.print("Humidity: ");
    tft.print(humidity.relative_humidity, 1);
    tft.print("%");

    tft.setCursor(10, 30);
    tft.print("Soil: ");
    tft.print(soilMoisture);
    tft.print("%");

    int barWidth = map(soilMoisture, 0, 100, 0, 128);
    tft.fillRect(10, 50, barWidth, 10, ST77XX_GREEN);
    tft.drawRect(10, 50, 128, 10, ST77XX_WHITE);
    
    // **Send Email if Soil is Too Dry**
    if (soilMoisture < 60 && !emailSent) {
        Serial.println("Soil moisture is low! Sending email alert...");
        sendEmail(soilMoisture);
        emailSent = true;
    } 
    else if (soilMoisture >= 70) {
        emailSent = false; 
    }

    // **Handle Web Server Requests**
    WiFiClient client = server.available();
    if (client) {
        Serial.println("New client connected!");
        sendWebPage(client, temp.temperature, humidity.relative_humidity, soilMoisture);
        client.stop(); 
        Serial.println("Client disconnected.");
    }

    delay(5000);
}

// **📧 Function to Send Email**
void sendEmail(int soilMoisture) {
    smtp.connect(&session);

    message.sender.name = "ESP32 Plant Monitor";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "🚨 Low Soil Moisture Alert!";
    message.addRecipient("Plant Owner", RECIPIENT_EMAIL);

    String emailBody = "ALERT! Soil moisture is too low! Current Level: " + String(soilMoisture) + "%.\nPlease water the plant ASAP!";
    message.text.content = emailBody.c_str();

    if (smtp.connected()) {
        if (!MailClient.sendMail(&smtp, &message)) {
            Serial.println("Email failed to send.");
        } else {
            Serial.println("Email sent successfully!");
        }
    }
    smtp.sendingResult.clear();
    smtp.closeSession();
}

// 🌱 ESP32 Plant Monitoring Web Server 🌿
void sendWebPage(WiFiClient client, float temp, float humidity, int soilMoisture) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    client.println("<!DOCTYPE html><html lang='en'><head>");
    client.println("<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.println("<title>Plant Monitoring Dashboard</title>");
    client.println("<style>");
    client.println("body { background: linear-gradient(135deg, #1e3c72, #2a5298); color: #fff; font-family: 'Arial', sans-serif; text-align: center; margin: 0; padding: 20px;}");
    client.println("h1 { font-size: 26px; text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.3);}");
    client.println(".dashboard { background: rgba(255, 255, 255, 0.1); padding: 20px; border-radius: 12px; box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.3); width: 90%; max-width: 400px; margin: auto;}");
    client.println(".data { font-size: 18px; margin: 10px 0;}");
    client.println(".progress-bar-container { width: 100%; background-color: rgba(255, 255, 255, 0.2); border-radius: 8px; overflow: hidden; margin: 20px 0;}");
    client.println(".progress-bar { height: 25px; width: " + String(soilMoisture) + "%; background: linear-gradient(90deg, #4caf50, #8bc34a); transition: width 0.5s ease-in-out;}");
    client.println(".log-box { width: 90%; background: white; color: black; padding: 10px; margin: 10px auto; border-radius: 5px; text-align: left;}");
    client.println(".button { padding: 10px 20px; font-size: 16px; border: none; border-radius: 5px; background-color: #ff9800; color: white; cursor: pointer; transition: 0.3s; margin: 5px;}");
    client.println(".button:hover { background-color: #e68900;}");
    client.println(".dark-mode { background: #121212; color: #e0e0e0; }");
    client.println("</style></head><body>");

    client.println("<h1>🌱 Plant Monitoring Dashboard 🌿</h1>");
    client.println("<div class='dashboard'>");
    client.println("<p class='data'>🌡️ Temperature: <span id='temp'>" + String(temp) + " &#8451;</span></p>");
    client.println("<p class='data'>💧 Humidity: <span id='humidity'>" + String(humidity) + "%</span></p>");
    client.println("<p class='data'>🌿 Soil Moisture: <span id='soil'>" + String(soilMoisture) + "%</span></p>");

    // 🌿 Soil Moisture Progress Bar
    client.println("<div class='progress-bar-container'><div id='progress' class='progress-bar' style='width:" + String(soilMoisture) + "%'></div></div>");

    // 🔄 Buttons for Refresh, Dark Mode, Alerts, and Data Download
    client.println("<button class='button' onclick='location.reload()'>🔄 Refresh</button>");
    client.println("<button class='button' onclick='toggleDarkMode()'>🌙 Toggle Dark Mode</button>");
    client.println("<button class='button' onclick='downloadLogs()'>📥 Download Logs</button>");
    client.println("<button class='button' onclick='showAlert()'>⚠️ Show Alert</button>");
    
    // 📝 Logs Section
    client.println("<h2>📋 System Logs</h2>");
    client.println("<ul id='log-list'>");
    for (int i = 0; i < LOG_SIZE; i++) {
        int logPos = (logIndex - 1 - i + LOG_SIZE) % LOG_SIZE; // Show newest logs first
        if (logEntries[logPos] != "") {
            client.println("<li class='log-box'>" + logEntries[logPos] + "</li>");
        }
    }
    client.println("</ul>");

    // 🖥️ JavaScript Functions for Dark Mode, Data Download & Alerts
    client.println("<script>");
    
    // 🌙 Toggle Dark Mode
    client.println("function toggleDarkMode() { document.body.classList.toggle('dark-mode'); }");

    // 📥 Download Logs as a TXT File
    client.println("function downloadLogs() {");
    client.println("    let logs = '📋 Plant Monitoring System Logs:\\n';");
    client.println("    let logItems = document.querySelectorAll('.log-box');");
    client.println("    logItems.forEach(item => { logs += item.innerText + '\\n'; });");
    client.println("    let blob = new Blob([logs], { type: 'text/plain' });");
    client.println("    let anchor = document.createElement('a'); anchor.href = URL.createObjectURL(blob); anchor.download = 'plant_logs.txt'; anchor.click();");
    client.println("}");

    // ⚠️ Motivational Alert
    client.println("function showAlert() { alert('Stay hydrated! Your plant needs water just like you!'); }");
    
    client.println("</script>");
    client.println("</body></html>");
}

