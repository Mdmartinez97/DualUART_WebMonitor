# 💻  Dual UART Web Monitor

Un monitor serie con interaz web en tiempo real para leer datos UART de hasta dos dispositivos simultáneamente. Capaz de adaptarse al tipo de comunicación serie necesaria.

## ✨ Características

- 📱 **Interfaz web responsive** - Terminal de pantalla completa con scroll inteligente
- 🔌 **Dual UART** - Lee dos dispositivos UART simultáneamente
- ⚙️ **Baudrate dinámico** - Selecciona la velocidad de cada dispositivo desde la web
- 🕒 **Timestamps** - Marca temporal en cada mensaje recibido
- 📊 **Buffer inteligente** - 20KB de historial con auto-scroll condicional
- 🔄 **Actualización automática** - Datos en tiempo real cada segundo

## 🎯 Vista Previa

```
[15s] [Device1] Temperatura: 25.3°C      <- En verde
[16s] [Device2] Humedad: 67%             <- En cian  
[17s] [Device1] Presión: 1013 hPa        <- En verde
[18s] [Device2] CO2: 420 ppm             <- En cian
```

## 🛠️ Hardware Requerido

- **ESP32-C3** (cualquier variante)
- **2 dispositivos UART** para conectar
- Cables de conexión
- Fuente de alimentación

## 🔌 Conexiones

### Device 1 (Verde)
```
Dispositivo 1  →  ESP32-C3
TX             →  GPIO20
GND            →  GND
```

### Device 2 (Cian)
```
Dispositivo 2  →  ESP32-C3  
TX             →  GPIO7
GND            →  GND
```

## ⚡ Instalación Rápida

### 1. Configurar Arduino IDE
- Instala el [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- Selecciona la placa: `ESP32C3 Dev Module`

### 2. Configurar WiFi
Edita estas líneas en el código:
```cpp
const char* ssid = "TU_WIFI_SSID";
const char* password = "TU_WIFI_PASSWORD";
```

### 3. Subir el código
- Conecta el ESP32-C3 vía USB
- Compila y sube el código
- Abre el Monitor Serie (115200 baudios)

### 4. Acceder al portal web
- Busca la IP en el Monitor Serie
- Abre un navegador y ve a: `http://[IP_DEL_ESP32]`

## 🎮 Cómo usar

### Interfaz Web
1. **Terminal principal**: Muestra datos de ambos dispositivos en tiempo real
2. **Selector de baudrate**: Cambia velocidad independiente para cada dispositivo  
3. **Clear Buffer**: Limpia todo el historial
4. **Auto-scroll inteligente**: Solo hace scroll si estás al final

### Baudrates soportados
- 9600
- 19200  
- 115200

### Navegación del terminal
- ✅ **Scroll libre** - Navega el historial sin interrupciones
- ✅ **Auto-scroll** - Solo cuando estás al final del terminal
- ✅ **Buffer de 20KB** - Mantiene historial extenso

## 🎨 Personalización

### Cambiar colores de dispositivos
En la función `readUARTData()`:
```cpp
// Device 1 (actualmente verde #00ff00)
String dataWithTime = "<span style='color: #TU_COLOR'>" + timestamp + "[Device1] " + newData1 + "</span>\n";

// Device 2 (actualmente cian #00ffff)  
String dataWithTime = "<span style='color: #TU_COLOR'>" + timestamp + "[Device2] " + newData2 + "</span>\n";
```

### Agregar más baudrates
En las funciones `handleSetBaud1()` y `handleSetBaud2()`:
```cpp
if (newBaud == 9600 || newBaud == 19200 || newBaud == 115200 || newBaud == TU_NUEVO_BAUD) {
```

Y en el HTML:
```cpp
html += "<option value='TU_NUEVO_BAUD'>TU_NUEVO_BAUD</option>";
```

### Cambiar tamaño del buffer
```cpp
const int MAX_BUFFER_SIZE = 20000; // Cambiar según necesidad
```

## 🔧 Solución de Problemas

### No conecta a WiFi
- ✅ Verifica SSID y contraseña
- ✅ Revisa que la red sea 2.4GHz
- ✅ Comprueba la señal WiFi

### No recibo datos UART
- ✅ Verifica las conexiones (TX → RX, GND → GND)
- ✅ Confirma el baudrate correcto
- ✅ Revisa que el dispositivo esté transmitiendo
- ✅ Prueba invertir TX/RX si es necesario

### Página web no carga
- ✅ Verifica la IP en el Monitor Serie
- ✅ Asegúrate de estar en la misma red WiFi
- ✅ Prueba reiniciar el ESP32-C3
- ✅ Desactiva VPN si la tienes activa

### Datos cortados o perdidos
- ✅ Reduce la velocidad de transmisión del dispositivo
- ✅ Aumenta `MAX_BUFFER_SIZE` si es necesario
- ✅ Verifica conexiones físicas

## 📋 Especificaciones Técnicas

| Característica | Valor |
|----------------|-------|
| Microcontrolador | ESP32-C3 |
| Puertos UART | 2 (UART0, UART1) |
| Buffer total | 20KB |
| Baudrates | 9600, 19200, 115200 |
| Protocolo | 8N1 (8 bits, sin paridad, 1 stop bit) |
| Servidor web | Puerto 80 |
| Actualización | 1 segundo |

## 🚀 Características Avanzadas

### Auto-scroll Inteligente
El terminal solo hace scroll automático si estás viendo el final. Si navegas hacia arriba para ver historial, respeta tu posición.

### Buffer Circular
Cuando el buffer alcanza 20KB, automáticamente elimina los datos más antiguos para mantener el rendimiento.

### Timestamps Precisos  
Cada mensaje incluye timestamp basado en `millis()` para seguimiento temporal exacto.

### Responsive Design
La interfaz se adapta automáticamente a dispositivos móviles y tablets.

## 🤝 Contribuir

¡Las contribuciones son bienvenidas! Puedes:

1. 🐛 **Reportar bugs** - Abre un issue describiendo el problema
2. 💡 **Sugerir mejoras** - Propón nuevas características  
3. 🔧 **Enviar Pull Requests** - Mejora el código existente
4. 📖 **Mejorar documentación** - Ayuda a otros usuarios

### Para contribuir:
1. Fork el repositorio
2. Crea una branch (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la branch (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## 📄 Licencia

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para detalles.

## 👤 Autor

Creado con ❤️ para la comunidad ESP32

## 🙏 Agradecimientos

- Comunidad ESP32-Arduino por el excelente framework
- Espressif por el hardware ESP32-C3
- Contribuidores de la comunidad open source

## 📞 Soporte

¿Problemas? ¿Preguntas? 

- 🐛 [Abre un Issue](https://github.com/tu-usuario/dual-uart-monitor/issues)
- 💬 [Discussions](https://github.com/tu-usuario/dual-uart-monitor/discussions)
- 📧 Email: tu-email@ejemplo.com

---

⭐ **¡Si este proyecto te fue útil, dale una estrella!** ⭐