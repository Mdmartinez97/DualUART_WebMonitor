#include <WiFi.h>
#include <WebServer.h>
#include "driver/uart.h"

// Wi-Fi config
const char* ssid = "SSID";
const char* password = "PASSWORD";

WebServer server(80);

// UART config
#define UART_DEVICE1 UART_NUM_0
#define UART_DEVICE2 UART_NUM_1
#define RX_PIN_DEVICE1 20  // GPIO20 (UART 1)
#define RX_PIN_DEVICE2 7   // GPIO7 (UART 2)
#define BUF_SIZE 1024

// Buffers
String uartBuffer = "";
String lastData1 = "";
String lastData2 = "";
const int MAX_BUFFER_SIZE = 20000;

// Initial Baudrates
long currentBaudRate1 = 115200;
long currentBaudRate2 = 115200;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("ESP32-C3 Dual UART Web Server");
  Serial.println("Device 1: GPIO 20 - Device 2: GPIO 7");
  Serial.println("================================");
  
  // Init UART1 and UART2
  initUART();
  
  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Configurar rutas del servidor web
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/clear", handleClear);
  server.on("/setBaud1", handleSetBaud1);
  server.on("/setBaud2", handleSetBaud2);
  
  server.begin();
  Serial.println("Web Server started");
}

void loop() {
  server.handleClient();
  
  // Read both UART devices
  readUARTData();
  
  delay(1);
}

void initUART() {
  // Device 1 UART config
  uart_config_t uart_config1 = {
    .baud_rate = currentBaudRate1,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .rx_flow_ctrl_thresh = 122,
  };
  
  ESP_ERROR_CHECK(uart_param_config(UART_DEVICE1, &uart_config1));
  ESP_ERROR_CHECK(uart_set_pin(UART_DEVICE1, UART_PIN_NO_CHANGE, RX_PIN_DEVICE1, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
  ESP_ERROR_CHECK(uart_driver_install(UART_DEVICE1, BUF_SIZE * 2, 0, 0, NULL, 0));

  //  Device 2 UART config
  uart_config_t uart_config2 = {
    .baud_rate = currentBaudRate2,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .rx_flow_ctrl_thresh = 122,
  };
  
  ESP_ERROR_CHECK(uart_param_config(UART_DEVICE2, &uart_config2));
  ESP_ERROR_CHECK(uart_set_pin(UART_DEVICE2, UART_PIN_NO_CHANGE, RX_PIN_DEVICE2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
  ESP_ERROR_CHECK(uart_driver_install(UART_DEVICE2, BUF_SIZE * 2, 0, 0, NULL, 0));

  Serial.println("UART ports configured correctly");
}

void readUARTData() {
  uint8_t data[BUF_SIZE];
  
  // Read Device 1
  int len1 = uart_read_bytes(UART_DEVICE1, data, BUF_SIZE - 1, 10 / portTICK_PERIOD_MS);
  if (len1 > 0) {
    data[len1] = 0;
    String newData1 = String((char*)data);
    newData1.trim();
    
    if (newData1.length() > 0) {
      String timestamp = "[" + String(millis() / 1000) + "s] ";
      String dataWithTime = timestamp + "<span style='color: #00ff00'> [Device1] " + newData1 + "</span>\n";
      
      uartBuffer += dataWithTime;
      lastData1 = newData1;
      
      // Limit buffer size
      if (uartBuffer.length() > MAX_BUFFER_SIZE) {
        int newlinePos = uartBuffer.indexOf('\n', uartBuffer.length() - MAX_BUFFER_SIZE);
        if (newlinePos > 0) {
          uartBuffer = uartBuffer.substring(newlinePos + 1);
        }
      }
      
      //Serial.print("[Device 1]: ");
      //Serial.println(newData1);
    }
  }
  
  // Read Device 2
  int len2 = uart_read_bytes(UART_DEVICE2, data, BUF_SIZE - 1, 10 / portTICK_PERIOD_MS);
  if (len2 > 0) {
    data[len2] = 0;
    String newData2 = String((char*)data);
    newData2.trim();
    
    if (newData2.length() > 0) {
      String timestamp = "[" + String(millis() / 1000) + "s] ";
      String dataWithTime = timestamp + "<span style='color: #00ffff'> [Device2] " + newData2 + "</span>\n";
      
      uartBuffer += dataWithTime;
      lastData2 = newData2;
      
      // Limit buffer size
      if (uartBuffer.length() > MAX_BUFFER_SIZE) {
        int newlinePos = uartBuffer.indexOf('\n', uartBuffer.length() - MAX_BUFFER_SIZE);
        if (newlinePos > 0) {
          uartBuffer = uartBuffer.substring(newlinePos + 1);
        }
      }
      
      //Serial.print("[Device 2]: ");
      //Serial.println(newData2);
    }
  }
}

void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html><head><title>Dual UART Web Monitor</title>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<link rel='icon' href='data:image/svg+xml,<svg xmlns=%22http://www.w3.org/2000/svg%22 viewBox=%220 0 100 100%22><text y=%22.9em%22 font-size=%2290%22>💻</text></svg>'>";
  html += "<style>";
  html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
  html += "body { font-family: Arial, sans-serif; background-color: #000; color: #fff; height: 100vh; display: flex; flex-direction: column; }";
  html += ".header { padding: 15px 20px; background-color: #333; border-bottom: 2px solid #555; }";
  html += ".title { color: white; text-align: center; margin-bottom: 15px; font-size: 34px; font-weight: bold; }";
  html += ".controls { display: grid; grid-template-columns: 1fr 1fr; gap: 20px; }";
  html += ".device-controls { display: flex; align-items: center; gap: 10px; padding: 10px; background-color: #444; border-radius: 5px; }";
  html += ".device-label { font-weight: bold; color: #fff; min-width: 80px; }";
  html += ".button { background-color: #f44336; color: white; padding: 8px 16px; border: none; border-radius: 4px; cursor: pointer; font-size: 14px; }";
  html += ".button:hover { background-color: #da190b; }";
  html += ".baud-selector { background-color: #666; color: white; padding: 8px 12px; border: 1px solid #888; border-radius: 4px; font-size: 14px; }";
  html += ".terminals { height: calc(100vh - 120px); }";
  html += ".terminal { flex: 1; background-color: #000; color: #fff; padding: 15px; font-family: 'Courier New', monospace; font-size: 13px; overflow-y: auto; white-space: pre-wrap; word-wrap: break-word; border: 1px solid #333; width: 100%; height: 100%; }";
  html += ".device1-color { color: #00ff00 !important; }";
  html += ".device2-color { color: #00ffff !important; }";
  html += "@media (max-width: 768px) {";
  html += "  .controls { grid-template-columns: 1fr; }";
  html += "  .terminals { grid-template-columns: 1fr; height: auto; }";
  html += "  .terminal { height: 300px; }";
  html += "}";
  html += "</style>";
  
  html += "<script>";
  html += "function updateData() {";
  html += "  var container = document.getElementById('terminal');";
  html += "  var isNearBottom = container.scrollTop + container.clientHeight >= container.scrollHeight - 50;";
  html += "  fetch('/data').then(response => response.text()).then(data => {";
  html += "    container.innerHTML = data;";
  html += "    if (isNearBottom) {";
  html += "      container.scrollTop = container.scrollHeight;";
  html += "    }";
  html += "  });";
  html += "}";
  html += "function clearData() {";
  html += "  fetch('/clear').then(() => {";
  html += "    document.getElementById('terminal').innerHTML = 'Buffer Cleared...';";
  html += "    setTimeout(updateData, 500);";
  html += "  });";
  html += "}";
  html += "function changeBaudRate1() {";
  html += "  var select = document.getElementById('baudSelect1');";
  html += "  var newBaud = select.value;";
  html += "  fetch('/setBaud1?baud=' + newBaud).then(() => {";
  html += "    document.getElementById('terminal').innerHTML = '<span style=\"color: #00ff00\">[Device1] Baudrate changed to ' + newBaud + ' - Restarting UART...</span>\\n';";
  html += "    setTimeout(updateData, 1000);";
  html += "  });";
  html += "}";
  html += "function changeBaudRate2() {";
  html += "  var select = document.getElementById('baudSelect2');";
  html += "  var newBaud = select.value;";
  html += "  fetch('/setBaud2?baud=' + newBaud).then(() => {";
  html += "    document.getElementById('terminal').innerHTML = '<span style=\"color: #00ffff\">[Device2] Baudrate changed to ' + newBaud + ' - Restarting UART...</span>\\n';";
  html += "    setTimeout(updateData, 1000);";
  html += "  });";
  html += "}";
  html += "setInterval(updateData, 1000);";
  html += "window.onload = function() { updateData(); }";
  html += "</script></head>";
  
  html += "<body>";
  html += "<div class='header'>";
  html += "<div class='title'>Dual UART Web Monitor</div>";
  html += "<div class='controls'>";
  html += "<div class='device-controls'>";
  html += "<span class='device-label' style='color: #00ff00;'>Device 1</span>";
  html += "<select id='baudSelect1' class='baud-selector' onchange='changeBaudRate1()'>";
  html += "<option value='9600'>9600</option>";
  html += "<option value='19200'>19200</option>";
  html += "<option value='115200' selected>115200</option>";
  html += "</select>";
  html += "</div>";
  html += "<div class='device-controls'>";
  html += "<span class='device-label' style='color: #00ffff;'>Device 2</span>";
  html += "<select id='baudSelect2' class='baud-selector' onchange='changeBaudRate2()'>";
  html += "<option value='9600'>9600</option>";
  html += "<option value='19200'>19200</option>";
  html += "<option value='115200' selected>115200</option>";
  html += "</select>";
  html += "</div>";
  html += "</div>";
  html += "<div style='text-align: center; margin-top: 10px;'>";
  html += "<button class='button' onclick='clearData()'>Clear Buffer</button>";
  html += "</div>";
  html += "</div>";
  
  html += "<div class='terminals'>";
  html += "<div id='terminal' class='terminal'>Loading data...</div>";
  html += "</div>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleData() {
  if (uartBuffer.length() == 0) {
    server.send(200, "text/html", "[...]");
  } else {
    server.send(200, "text/html", uartBuffer);
  }
}

void handleClear() {
  uartBuffer = "";
  lastData1 = "";
  lastData2 = "";
  server.send(200, "text/plain", "Buffer cleared");
}

void handleSetBaud1() {
  if (server.hasArg("baud")) {
    long newBaud = server.arg("baud").toInt();
    
    if (newBaud == 9600 || newBaud == 19200 || newBaud == 115200) {
      // Reconfigure UART1
      uart_driver_delete(UART_DEVICE1);
      delay(100);
      
      currentBaudRate1 = newBaud;
      uart_config_t uart_config1 = {
        .baud_rate = currentBaudRate1,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
      };
      
      ESP_ERROR_CHECK(uart_param_config(UART_DEVICE1, &uart_config1));
      ESP_ERROR_CHECK(uart_set_pin(UART_DEVICE1, UART_PIN_NO_CHANGE, RX_PIN_DEVICE1, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
      ESP_ERROR_CHECK(uart_driver_install(UART_DEVICE1, BUF_SIZE * 2, 0, 0, NULL, 0));
      
      server.send(200, "text/plain", "Device1 Baudrate changed to " + String(currentBaudRate1));
      
      uartBuffer = "";
    } else {
      server.send(400, "text/plain", "Invalid Baudrate");
    }
  } else {
    server.send(400, "text/plain", "Required baud parameter");
  }
}

void handleSetBaud2() {
  if (server.hasArg("baud")) {
    long newBaud = server.arg("baud").toInt();
    
    if (newBaud == 9600 || newBaud == 19200 || newBaud == 115200) {
      // Reconfigure UART2
      uart_driver_delete(UART_DEVICE2);
      delay(100);
      
      currentBaudRate2 = newBaud;
      uart_config_t uart_config2 = {
        .baud_rate = currentBaudRate2,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
      };
      
      ESP_ERROR_CHECK(uart_param_config(UART_DEVICE2, &uart_config2));
      ESP_ERROR_CHECK(uart_set_pin(UART_DEVICE2, UART_PIN_NO_CHANGE, RX_PIN_DEVICE2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
      ESP_ERROR_CHECK(uart_driver_install(UART_DEVICE2, BUF_SIZE * 2, 0, 0, NULL, 0));
      
      server.send(200, "text/plain", "Device2 Baudrate changed to " + String(currentBaudRate2));
      
      uartBuffer = "";
    } else {
      server.send(400, "text/plain", "Invalid Baudrate");
    }
  } else {
    server.send(400, "text/plain", "Required baud parameter");
  }
}