# Plant-Monitoring-System
The Plant Monitoring System tracks temperature, humidity, and soil moisture. Displays real-time data on a web server and TFT display. Sends email alerts on low moisture levels. The web interface also logs the last 10 sensor readings, includes a progress bar for moisture levels, and allows users to download logs as a text file.

## **Inspiration**  
With increasing urbanization, many people struggle to maintain healthy plants due to busy schedules, lack of knowledge, or limited access to real-time monitoring. Water conservation is also a growing concern, making it crucial to optimize plant care without overwatering. This project was inspired by the need for **an affordable, automated solution** that helps urban gardeners, homeowners, and farmers **monitor their plants remotely and efficiently**.  

## **What it does**  
The **ESP32 Plant Monitoring System** continuously tracks **temperature, humidity, and soil moisture**. It displays real-time data on **a web dashboard** and **a TFT display**. If the soil moisture **drops below 60%**, the system **sends an email alert**, notifying the user to water the plant. The web interface also logs the last 10 sensor readings, includes **a progress bar for moisture levels**, and allows users to **download logs as a text file** for further analysis.  

## **How we built it**  
- **ESP32** handles all processing, WiFi communication, and web hosting.  
- **AHT10 sensor** measures temperature and humidity.  
- **KS0049 soil moisture sensor** detects soil hydration levels.  
- **ST7735 TFT display** shows real-time sensor readings.  
- **A web server** built into the ESP32 provides remote monitoring.  
- **ESP32 Mail Client** sends automated alerts via email.  
- **HTML, CSS, and JavaScript** create an interactive web dashboard.  

## **Challenges we ran into**  
- **Power fluctuations** affected sensor readings and display brightness. We solved this by optimizing power distribution and ensuring stable voltage levels.  
- **ESP32 WiFi authentication issues** caused connection failures on some networks, requiring troubleshooting of network settings and authentication methods.  
- **Email sending limitations** due to SMTP security restrictions, which required configuring **Gmail App Passwords** for authentication.  
- **Logging data efficiently** without causing memory overflow, which was solved by **storing only the last 10 readings in a circular buffer**.  

## **Accomplishments that we're proud of**  
- Successfully integrated **live sensor monitoring, web logging, and email alerts** into a single ESP32-based system.  
- Developed **a modern, user-friendly web interface** with a **progress bar, dark mode toggle, and log download feature**.  
- Optimized **data logging and email notifications** to prevent redundant alerts and excessive data storage.  
- Created a **scalable and cost-effective solution** that can be expanded for **smart irrigation, hydroponics, and urban farming applications**.  

## **What we learned**  
- How to implement **real-time IoT monitoring** with ESP32 and sensors.  
- Optimizing **ESP32 power consumption and sensor accuracy** in long-term monitoring applications.  
- Working with **Gmail’s SMTP service** and handling email security constraints.  
- Improving **user experience in IoT dashboards** with **JavaScript-based interactions** like downloading logs and toggling themes.  
- How **urban gardening challenges** can be addressed with **IoT and automation**.  

## **What's next for Plant Monitoring System**  
- **Automated watering system** that controls a servo valve based on soil moisture.  
- **Cloud integration** to store long-term data in Firebase or Google Sheets.  
- **Mobile app support** for real-time notifications and remote access.  
- **Machine learning-based plant care predictions**, analyzing trends in temperature, humidity, and soil moisture.  
- **Battery and solar-powered version** for outdoor use in urban gardens and farms.  

This project lays the foundation for smart urban agriculture and sustainable plant care solutions
