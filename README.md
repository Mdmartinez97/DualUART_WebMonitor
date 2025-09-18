# 💻  Dual UART Web Monitor

A serial monitor with real-time web interaction to read UART data from up to two devices simultaneously. Able to adapt to the required serial communication type.

## 🦾 Features

- 📱 **Responsive web interface** - Full-screen terminal with auto-scrolling
- 🔌 **Dual UART** - Read two UART devices simultaneously
- ⚙️ **Dynamic Baudrate** - Select the speed of each device from the web page
- 🕒 **Timestamps** - Timestamp on each received message
- 🔄 **Automatic update** - Real-time data every second

## 👀 Preview
![Image](/Capture.JPG)


## 🚗 Hardware
 The project was developed on an ESP32 C3 Mini, but a wide variety of microcontrollers can be used.
👉 [Datasheet](https://michiel.vanderwulp.be/domotica/Modules/ESP32-C3-SuperMini/)

## 🛠️ Initial Setup

### UART
It is possible to configure in the code the UART ports and GPIOs to be used according to availability and need.

```c
#define RX_PIN_DEVICE1 20  // GPIO20 (UART 1)
#define RX_PIN_DEVICE2 7   // GPIO7 (UART 2)
```

### Wi-Fi Credentials
You will need to manually set the credentials of the network the device will connect to.

```c
// Wi-Fi config
const char* ssid = "SSID";
const char* password = "PASSWORD";
```

## ⚡ Execution

### Access the Web Portal
- Find the IP address printed on the Serial Monitor
- Open a browser and go to (for example): `http://192.168.1.123`

## 👇 How to Use

### Web Interface
1. **Main Terminal**: Displays real-time data from both devices
2. **Baud Rate Selector**: Changes the speed independently for each device
3. **Clear Buffer**: Clears all history
4. **Smart Auto-Scroll**: Only scrolls if you're at the bottom of the screen

### Supported Baudrates
- 9600
- 19200  
- 115200

 Protocol 8N1 (8 bits, no parity, 1 stop bit)


## 🎨 Customization

### Change Device Colors
In `readUARTData()` function:
```cpp
// Device 1 (currently green #00ff00)
String dataWithTime = "<span style='color: #TU_COLOR'>" + timestamp + "[Device1] " + newData1 + "</span>\n";

// Device 2 (currently cyan #00ffff)  
String dataWithTime = "<span style='color: #TU_COLOR'>" + timestamp + "[Device2] " + newData2 + "</span>\n";
```

### Add More Baudrates
In `handleSetBaud1()` and `handleSetBaud2()` functions:
```cpp
if (newBaud == 9600 || newBaud == 19200 || newBaud == 115200 || newBaud == NEW_BAUDRATE) {
```

And also in HTML code:
```cpp
html += "<option value='NEW_BAUDRATE'>NEW_BAUDRATE</option>";
```

### Change Buffer Size
```cpp
const int MAX_BUFFER_SIZE = 20000; // Change as needed
```

## 🔧 Troubleshooting

### Not Connecting to WiFi
- ✅ Check SSID and Password
- ✅ Check that the network is 2.4GHz
- ✅ Check the WiFi signal

### No UART Data Received
- ✅ Check the connections (TX → RX, GND → GND)
- ✅ Confirm the correct baud rate
- ✅ Check that the device is transmitting
- ✅ Try reversing TX/RX if necessary

### Web Page Not Loading
- ✅ Check the IP in the Serial Monitor
- ✅ Make sure you are on the same WiFi network
- ✅ Try restarting the ESP32-C3

### Cut or Lost Data
- ✅ Reduce the baud rate of the device read
- ✅ Increase `MAX_BUFFER_SIZE` if necessary
- ✅ Check physical connections

## 👤 Author
- 📧 Email: marcosmartinez.bo@gmail.com
- 🐛 [Open an Issue](https://github.com/tu-usuario/dual-uart-monitor/issues)
- 💬 [Discussions](https://github.com/tu-usuario/dual-uart-monitor/discussions)


---
