# **ESP32 Plant Monitoring System - Documentation**  

## **1. Project Overview**  
The **ESP32 Plant Monitoring System** is an **IoT-based smart plant care solution** that tracks **temperature, humidity, and soil moisture levels**. It provides a **web-based dashboard** for real-time monitoring and **sends email alerts** when soil moisture levels drop below 60%. This project is useful for **home gardeners, urban farms, and smart agriculture applications**, helping prevent plant dehydration and optimize water usage.  

---

## **2. Required Libraries**  
Before compiling the code, ensure you have the following libraries installed in the **Arduino IDE**:  

| **Library Name**            | **Purpose** |
|----------------------------|------------|
| **WiFi.h**                 | Enables ESP32 WiFi connection |
| **Wire.h**                 | Manages I2C communication for sensors |
| **SPI.h**                  | Handles SPI communication for the TFT display |
| **Adafruit_GFX.h**         | Graphics library for the TFT display |
| **Adafruit_ST7735.h**      | Driver for ST7735 TFT display |
| **Adafruit_AHTX0.h**       | Library for AHT10 temperature and humidity sensor |
| **ESP_Mail_Client.h**      | Enables ESP32 to send email notifications |

To install these libraries:  
1. Open **Arduino IDE**  
2. Go to **Sketch > Include Library > Manage Libraries**  
3. Search for each library and click **Install**  

---

## **3. Hardware Components & Connections**  

### **Required Hardware**
| **Component**           | **Description** |
|------------------------|----------------|
| **ESP32-WROOM-32**     | Microcontroller for processing & WiFi |
| **AHT10 Sensor**       | Measures temperature & humidity |
| **KS0049 Soil Sensor** | Detects soil moisture levels |
| **ST7735 TFT Display** | Displays sensor data |
| **Jumper Wires**       | Connects components |
| **Breadboard**         | Organizes circuit connections |

### **Pin Connections**  
| **Component**          | **ESP32 Pin** |
|-----------------------|--------------|
| **AHT10 SDA**         | **GPIO 21** |
| **AHT10 SCL**         | **GPIO 22** |
| **Soil Sensor**       | **GPIO 34** |
| **TFT DC (A0)**       | **GPIO 12** |
| **TFT CS**            | **GPIO 13** |
| **TFT MOSI (SDA)**    | **GPIO 14** |
| **TFT SCK (CLK)**     | **GPIO 27** |

---

## **4. Setup Instructions**

### **4.1 Setting Up ESP32 in Arduino IDE**  
1. Open **Arduino IDE**  
2. Go to **File > Preferences**  
3. Add the following **Board Manager URL** under **Additional Board Manager URLs**:  
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Go to **Tools > Board > Boards Manager**, search for **ESP32**, and click **Install**  
5. Select **ESP32 Dev Module** under **Tools > Board**  

### **4.2 Configuring WiFi Credentials**
In the code, update the WiFi credentials with your network name and password:  
```cpp
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";
```

### **4.3 Setting Up Email Alerts (Gmail SMTP)**
1. Enable **Less Secure Apps** in your Gmail account (or create an **App Password**)  
2. Replace the following in the code:  
```cpp
#define AUTHOR_EMAIL "your_email@gmail.com"
#define AUTHOR_PASSWORD "your_app_password"
#define RECIPIENT_EMAIL "recipient_email@gmail.com"
```

### **4.4 Uploading the Code to ESP32**
1. Connect ESP32 to your computer via USB  
2. Select the correct **Board** and **Port** under **Tools**  
3. Click **Upload** and wait for the code to compile and flash to ESP32  

---

## **5. Web Dashboard Features**
Once the ESP32 is running, you can access the **Plant Monitoring Dashboard** from any web browser:  
1. Find the ESP32’s IP Address in the **Serial Monitor**  
2. Open a web browser and enter the **ESP32’s IP Address**  
3. The dashboard will display:  
   - **Temperature, Humidity, and Soil Moisture levels**  
   - **A progress bar indicating soil moisture**  
   - **A log of the last 10 sensor readings**  
   - **Buttons for refreshing data, toggling dark mode, and downloading logs**  

---

## **6. Expected Behavior**
| **Action**                  | **Expected Outcome** |
|-----------------------------|---------------------|
| Soil moisture **drops below 60%** | **Sends an email alert** |
| Soil moisture **rises above 70%** | **Stops sending alerts** |
| User clicks **Refresh Button** | **Updates sensor readings** |
| User enables **Dark Mode** | **Switches theme to dark mode** |
| User downloads **Logs** | **Saves last 10 readings as TXT file** |

---

## **7. Troubleshooting**
### **Issue: ESP32 Not Connecting to WiFi**  
✔ Double-check WiFi credentials in the code  
✔ Restart ESP32 and try again  
✔ Make sure your network allows **2.4 GHz connections** (ESP32 does not support 5 GHz)  

### **Issue: TFT Display Not Showing Data**  
✔ Verify **correct wiring of SPI pins**  
✔ Ensure **Adafruit GFX and ST7735 libraries** are installed  
✔ Try adding a **delay(1000);** before initializing the display  

### **Issue: Email Alerts Not Working**  
✔ Check if **SMTP settings are correct**  
✔ Ensure Gmail App Password is used instead of regular password  
✔ Check the **Serial Monitor** for error messages  

---

## **8. Future Improvements**
- **Cloud storage integration** (Firebase/Google Sheets) for historical tracking  
- **Automated watering system** with servo-controlled irrigation  
- **Mobile app support** for remote monitoring  
- **AI-powered watering predictions** based on environmental trends  

This system is designed to be **scalable and adaptable** for various applications in **smart agriculture, urban gardening, and IoT-based plant monitoring**.
