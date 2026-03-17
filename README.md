# ESP32-S3 RFID API Scanner 📡

This project is an IoT RFID scanner built with an **ESP32-S3-DevKitC-1** and an **RC522 RFID Reader**. It scans MIFARE RFID badges and automatically sends the badge's Unique ID (UID) to a local API server over WiFi.

It is programmed in C++ using **PlatformIO** and features a modular code structure, automatic WiFi reconnection, and detailed serial debugging.

## 🛠️ Hardware Requirements
* **Microcontroller:** ESP32-S3-DevKitC-1
* **RFID Module:** RC522 (MIFARE Classic 1K reader)
* **Jumper Wires**

### ⚠️ Critical Wiring Warning
The RC522 is a **3.3V device**. Do not connect the VCC pin to the 5V/USB pin on the ESP32, as it may damage the RFID module.

### Wiring Table (Hardware SPI)
| RC522 Pin | ESP32-S3 Pin | Notes |
| :--- | :--- | :--- |
| **VCC** | **3.3V** | **CRITICAL: Use 3.3V only!** |
| **RST** | **GPIO 5** | Reset pin |
| **GND** | **GND** | Common Ground |
| **MISO** | **GPIO 13** | Hardware SPI MISO |
| **MOSI** | **GPIO 11** | Hardware SPI MOSI |
| **SCK** | **GPIO 12** | Hardware SPI Clock |
| **NSS (SDA)**| **GPIO 10** | Slave Select / Chip Select |

---

## 💻 Software & Dependencies
This project is built using [PlatformIO](https://platformio.org/) (via CLion).

**Libraries used:**
* `miguelbalboa/MFRC522` (v1.4.11+) - For communicating with the RC522 module.
* Built-in ESP32 core libraries: `WiFi.h`, `HTTPClient.h`, `SPI.h`.

---

## 📂 Project Structure
The code is divided into modular files for cleaner maintenance:

```text
├── include/
│   └── rfid_wifi.h       # Function prototypes and shared external variables
├── src/
│   ├── main.cpp          # System setup, main loop, and configuration variables
│   └── rfid_wifi.cpp     # Core logic for RFID reading