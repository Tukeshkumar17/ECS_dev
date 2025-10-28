# ECS_Dev - Emergency Communication System
![](https://github.com/Tukeshkumar17/ECS_dev/intro1.gif)

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Version](https://img.shields.io/badge/version-1.0.0-green.svg)

A battery-powered emergency communication device using LoRa technology for long-range, infrastructure-free messaging during disasters like floods, wars, earthquakes, and power outages.

## 🌟 Features

- **Long-Range Communication**: Up to 10km line-of-sight using LoRa technology
- **No Infrastructure Required**: Works without internet, cellular networks, or WiFi
- **Mesh Networking**: Messages relay through other devices automatically
- **Battery Powered**: Runs for days on a single 18650 battery
- **OLED Display**: Clear 0.96" display for reading messages
- **Simple Interface**: 3-button navigation system
- **Emergency Broadcast**: Send alerts to all nearby devices
- **Message History**: Store last 20 messages
- **Low Power Mode**: Automatic sleep to conserve battery

## 📦 Hardware Requirements

| Component | Specification | Qty |
|-----------|--------------|-----|
| ESP32 DevKit | ESP32-WROOM-32 | 1 |
| LoRa Module | SX1276/SX1278 (433MHz or 915MHz) | 1 |
| OLED Display | 0.96" I2C 128x64 SSD1306 | 1 |
| Push Buttons | Tactile switches | 3 |
| 18650 Battery | 3.7V Li-ion | 1 |
| Battery Holder | 18650 holder with leads | 1 |
| TP4056 Module | Li-ion charging module | 1 |
| Antenna | 433/915MHz antenna (match LoRa frequency) | 1 |
| Resistors | 10kΩ pull-up | 3 |
| Enclosure | Weatherproof box (optional) | 1 |

## 🔌 Wiring Diagram

```
ESP32          LoRa SX1276/78       OLED Display        Buttons
─────          ──────────────       ────────────        ───────
GPIO5   ───────> CS (NSS)           
GPIO19  ───────> MISO               
GPIO27  ───────> MOSI               
GPIO18  ───────> SCK                
GPIO14  ───────> RESET              
GPIO26  ───────> DIO0               
                                    
GPIO21  ───────────────────────────> SDA
GPIO22  ───────────────────────────> SCL
                                    
GPIO32  ───────────────────────────────────────> UP Button
GPIO33  ───────────────────────────────────────> SELECT Button
GPIO25  ───────────────────────────────────────> DOWN Button

3.7V Battery ──> TP4056 ──> ESP32 VIN
                  │
                  └──> USB (for charging)
```

## 🚀 Quick Start

### 1. Install Arduino IDE & Libraries

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Add ESP32 board support:
   - Go to `File > Preferences`
   - Add to "Additional Board Manager URLs": 
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to `Tools > Board > Boards Manager`
   - Search "ESP32" and install

3. Install required libraries via `Sketch > Include Library > Manage Libraries`:
   - **LoRa** by Sandeep Mistry (v0.8.0 or later)
   - **Adafruit GFX Library**
   - **Adafruit SSD1306**

### 2. Upload the Code

1. Connect ESP32 to your computer via USB
2. Open `ECS_Dev.ino` in Arduino IDE
3. Select board: `Tools > Board > ESP32 Arduino > ESP32 Dev Module`
4. Select port: `Tools > Port > [Your ESP32 Port]`
5. Configure LoRa frequency in code (line 15):
   - US: `915E6`
   - EU/Asia: `433E6`
6. Click Upload button

### 3. Assemble Hardware

1. Solder/connect components according to wiring diagram
2. Attach antenna to LoRa module
3. Insert charged 18650 battery
4. Power on - device should show "ECS_Dev v1.0" splash screen

## 📱 How to Use

### Navigation
- **UP Button**: Scroll up / Navigate menus
- **DOWN Button**: Scroll down / Type next character
- **SELECT Button**: Confirm selection / Send message

### Main Menu Options

1. **Send Message**: Compose and send a message
2. **Inbox**: View received messages (last 20)
3. **Broadcast**: Send emergency alert to all devices
4. **Settings**: View device info and battery status

### Sending a Message

1. Select "Send Message" from main menu
2. Use DOWN button to cycle through characters (A-Z, 0-9, space, punctuation)
3. Press SELECT to add character
4. Select "SEND" when done
5. Message broadcasts to all nearby devices

### Emergency Broadcast

1. Select "Broadcast" from main menu
2. Select emergency type (Flood/Fire/Medical/War/Other)
3. Message sends immediately to all devices with high priority

## 🔋 Battery Life

- **Active Use**: 24-48 hours
- **Standby Mode**: 5-7 days
- **Sleep Mode**: 2-3 weeks

Tips for longer battery life:
- Device auto-sleeps after 5 minutes of inactivity
- Press any button to wake
- Reduce screen brightness in settings (future update)

## 📡 Communication Range

- **Line of Sight**: Up to 10km
- **Urban Areas**: 1-3km
- **Indoor**: 200-500m
- **Factors**: Buildings, trees, and terrain affect range

## 🔧 Troubleshooting

| Issue | Solution |
|-------|----------|
| Device won't power on | Check battery polarity and charge level |
| Display blank | Verify I2C connections (SDA/SCL) |
| No messages received | Check antenna connection and LoRa frequency |
| Upload fails | Hold BOOT button during upload |
| Poor range | Ensure antenna is properly connected and vertical |

## 📊 Technical Specifications

- **Microcontroller**: ESP32 (240MHz dual-core)
- **LoRa Frequency**: 433/915MHz (configurable)
- **Transmit Power**: 20dBm (100mW)
- **Bandwidth**: 125kHz
- **Spreading Factor**: SF7-SF12 (adaptive)
- **Power Consumption**: 
  - Active: 120mA
  - Receiving: 50mA
  - Sleep: <1mA
- **Operating Temp**: -20°C to 60°C

## 🛡️ Security & Privacy

- **No Encryption**: Messages are transmitted in plain text
- **Public Broadcast**: Anyone with a compatible device can receive messages
- **No Authentication**: Messages cannot be verified

**For Production**: Implement AES encryption and authentication (add crypto library)

## 📝 Message Protocol

```
Format: [SENDER_ID]|[TIMESTAMP]|[MESSAGE_TYPE]|[CONTENT]
Example: DEV_A3F2|1234567890|MSG|Hello World
Types: MSG (normal), BCST (broadcast), EMRG (emergency)
```

## 🔮 Future Enhancements

- [ ] GPS integration for location sharing
- [ ] AES-256 encryption
- [ ] Voice message recording
- [ ] Solar panel support
- [ ] SD card for message storage
- [ ] Web interface for configuration
- [ ] Multiple channels
- [ ] Message acknowledgment

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Open Pull Request

## 📄 License

This project is licensed under the MIT License - see LICENSE file for details.

## ⚠️ Disclaimer

This device is for emergency communication only. Always comply with local radio frequency regulations. The creators are not responsible for misuse or regulatory violations.

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/Tukeshkumar17/ECS_dev/issues)
- **Discussions**: [GitHub Discussions](https://github.com/Tukeshkumar17/ECS_dev/discussions)

