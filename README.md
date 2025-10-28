# 📡 ECS_Dev - Emergency Communication System

<div align="center">

![ECS_Dev Demo](https://github.com/Tukeshkumar17/ECS_dev/blob/main/intro1.gif)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/Tukeshkumar17/ECS_dev/releases)
[![Hardware](https://img.shields.io/badge/hardware-ESP32-green.svg)](https://www.espressif.com/en/products/socs/esp32)
[![LoRa](https://img.shields.io/badge/wireless-LoRa-red.svg)](https://lora-alliance.org/)

**When infrastructure fails, communication survives.**

*A robust, off-grid emergency communication device designed for disaster scenarios, built with LoRa technology and ESP32.*

[Features](#-why-ecs_dev) • [Hardware](#-components-you-need) • [Setup](#-getting-started) • [Documentation](#-how-it-works) • [Contributing](#-join-the-mission)

</div>

---

## 🎯 Why ECS_Dev?

In the aftermath of natural disasters, conflicts, or infrastructure failures, traditional communication networks often collapse when they're needed most. **ECS_Dev** bridges this critical gap by providing reliable, long-range communication that operates completely independently of existing infrastructure.

### What Makes This Special

**🌍 Infrastructure-Free Operation**  
No cellular towers, no internet, no WiFi required. Your messages travel directly from device to device using LoRa radio technology.

**📶 Impressive Range**  
Communicate up to **10 kilometers** in open terrain. Even in dense urban environments, you can maintain contact across **1-3 kilometers** of city blocks.

**🔋 Power Efficient Design**  
A single 18650 battery provides **24-48 hours** of active communication or up to **2-3 weeks** in sleep mode. Perfect for extended emergency situations.

**🕸️ Automatic Mesh Networking**  
Messages intelligently relay through other ECS_Dev devices, extending your reach beyond line-of-sight and creating a resilient communication web.

**💬 Simple, Reliable Interface**  
Three buttons and an OLED display. No complicated menus or settings. In a crisis, simplicity saves lives.

**🆘 Emergency Broadcasting**  
Instantly alert everyone in range with predefined emergency types: Flood, Fire, Medical, War, and more.

---

## 📦 Components You Need

Building your own ECS_Dev is straightforward. Here's everything you'll need:

### Core Electronics

| Component | Specification | Purpose | Qty |
|-----------|--------------|---------|-----|
| **ESP32 DevKit** | ESP32-WROOM-32 | Main microcontroller | 1 |
| **LoRa Module** | SX1276 or SX1278 | Long-range radio communication | 1 |
| **OLED Display** | 0.96" I2C 128x64 (SSD1306) | Message display | 1 |
| **Push Buttons** | Tactile 6x6mm switches | User interface | 3 |
| **18650 Battery** | 3.7V Li-ion (2500mAh+) | Power source | 1 |
| **Battery Holder** | Single 18650 holder | Battery mounting | 1 |
| **TP4056 Module** | Li-ion charging board | USB charging capability | 1 |
| **LoRa Antenna** | 433MHz or 915MHz (match your region) | Signal transmission | 1 |

### Additional Parts

- **3x 10kΩ Resistors** - Pull-up resistors for buttons
- **Jumper Wires** - For prototyping connections
- **Perfboard/PCB** - For permanent assembly (optional)
- **Enclosure** - Weatherproof case recommended
- **Micro-USB Cable** - For charging and programming

**💰 Estimated Total Cost:** $25-35 USD per device

---

## 🔌 Wiring Guide

Connect your components following this pin configuration:

```
┌─────────────────────────────────────────────────────────────┐
│                         ESP32 GPIO                           │
└─────────────────────────────────────────────────────────────┘
         │
         ├──> GPIO 5  ────────────────────────> LoRa CS (NSS)
         ├──> GPIO 19 ────────────────────────> LoRa MISO
         ├──> GPIO 27 ────────────────────────> LoRa MOSI
         ├──> GPIO 18 ────────────────────────> LoRa SCK
         ├──> GPIO 14 ────────────────────────> LoRa RESET
         ├──> GPIO 26 ────────────────────────> LoRa DIO0
         │
         ├──> GPIO 21 ────────────────────────> OLED SDA
         ├──> GPIO 22 ────────────────────────> OLED SCL
         │
         ├──> GPIO 32 ────────────────────────> UP Button
         ├──> GPIO 33 ────────────────────────> SELECT Button
         └──> GPIO 25 ────────────────────────> DOWN Button

┌─────────────────────────────────────────────────────────────┐
│                      Power Circuit                           │
└─────────────────────────────────────────────────────────────┘

    18650 Battery ──> TP4056 Module ──> ESP32 VIN (3.3V)
                           │
                           └────> Micro-USB (Charging Port)
```

**⚡ Important Notes:**
- Always connect ground (GND) pins together
- Use the 3.3V output from ESP32 for LoRa and OLED
- Add 10kΩ pull-up resistors between button pins and 3.3V
- Double-check LoRa frequency band matches your region (433MHz for Asia/EU, 915MHz for US)

---

## 🚀 Getting Started

### Step 1: Prepare Your Development Environment

**Install Arduino IDE**
1. Download [Arduino IDE 2.0+](https://www.arduino.cc/en/software)
2. Install and launch the application

**Add ESP32 Board Support**
1. Navigate to `File > Preferences`
2. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Open `Tools > Board > Boards Manager`
4. Search for "ESP32" by Espressif Systems
5. Click "Install"

**Install Required Libraries**

Open `Sketch > Include Library > Manage Libraries` and install:
- **LoRa** by Sandeep Mistry (v0.8.0+)
- **Adafruit GFX Library** by Adafruit
- **Adafruit SSD1306** by Adafruit

### Step 2: Configure and Upload

1. **Clone the Repository**
   ```bash
   git clone https://github.com/Tukeshkumar17/ECS_dev.git
   cd ECS_dev
   ```

2. **Open the Project**
   - Launch Arduino IDE
   - Open `ECS_Dev.ino`

3. **Configure LoRa Frequency**
   
   Find line 15 in the code and set your region:
   ```cpp
   // For United States / Australia / South America
   #define LORA_FREQ 915E6
   
   // For Europe / Asia / Africa
   #define LORA_FREQ 433E6
   ```

4. **Select Board Settings**
   - `Tools > Board > ESP32 Arduino > ESP32 Dev Module`
   - `Tools > Upload Speed > 115200`
   - `Tools > Flash Frequency > 80MHz`

5. **Connect and Upload**
   - Connect ESP32 via USB cable
   - Select port under `Tools > Port`
   - Click the **Upload** button (→)
   - Wait for "Done uploading" message

**Troubleshooting Upload Issues:**  
If upload fails, hold the **BOOT** button on ESP32 when you see "Connecting..." in the console.

### Step 3: Assemble Your Device

1. **Wire Components** following the schematic above
2. **Attach the Antenna** to your LoRa module (critical for range!)
3. **Insert Charged Battery** into the holder
4. **Power On** - you should see the "ECS_Dev v1.0" splash screen
5. **Test with Another Device** - build at least two units for testing

---

## 📱 How It Works

### Interface Overview

Your ECS_Dev has three buttons:

- **⬆️ UP** - Navigate up in menus / Scroll through messages
- **⬇️ DOWN** - Navigate down / Cycle through characters when typing
- **✓ SELECT** - Confirm choices / Add character to message

The OLED display shows all menus, messages, and status information clearly, even in bright sunlight.

### Main Menu

After the splash screen, you'll see four options:

**1. 📤 Send Message**  
Compose and broadcast a text message to all nearby devices.

**2. 📥 Inbox**  
View the last 20 received messages with sender ID and timestamp.

**3. 🚨 Emergency Broadcast**  
Send urgent alerts with predefined categories instantly.

**4. ⚙️ Settings**  
Check battery status, device ID, and system information.

### Composing a Message

1. Select **"Send Message"** from the main menu
2. Use **DOWN** button to cycle through available characters:
   - Uppercase letters (A-Z)
   - Numbers (0-9)
   - Space and punctuation (. , ! ?)
   - Special option: **SEND** (to broadcast) or **DEL** (to delete last character)
3. Press **SELECT** to add the highlighted character
4. Navigate to **SEND** and press **SELECT** to broadcast
5. Your message transmits to all devices within range

**Character Set:** `ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,!?`

### Emergency Broadcasting

For time-critical situations:

1. Select **"Broadcast"** from main menu
2. Choose emergency type using UP/DOWN:
   - 🌊 **FLOOD** - Water-related emergencies
   - 🔥 **FIRE** - Fire outbreaks or smoke
   - 🏥 **MEDICAL** - Medical emergencies or injuries
   - ⚔️ **WAR/CONFLICT** - Security threats or violence
   - ⚠️ **OTHER** - General urgent situations
3. Press **SELECT** to immediately broadcast to all devices

Emergency messages have priority routing and distinctive alerts on receiving devices.

### Understanding Message Flow

```
Your Device                Network                Other Devices
     │                        │                         │
     ├──> Send Message ──────>│                         │
     │                        ├──> Relay Broadcast ────>│
     │                        │                         ├──> Display
     │                        │<──── Potential Reply ───┤
     │<──── Receive Reply ────┤                         │
```

Every ECS_Dev device automatically relays messages it receives, creating a mesh network that extends far beyond individual device range.

---

## 🔋 Power Management

### Battery Life Expectations

| Mode | Duration | Power Draw |
|------|----------|------------|
| **Active Communication** | 24-48 hours | ~120mA |
| **Receiving Only** | 48-72 hours | ~50mA |
| **Standby Mode** | 5-7 days | ~10mA |
| **Deep Sleep** | 2-3 weeks | <1mA |

### Maximizing Battery Life

✅ **Auto-Sleep Feature:** Device automatically enters low-power mode after 5 minutes of inactivity  
✅ **Wake on Button:** Press any button to instantly resume operation  
✅ **Efficient Radio:** LoRa uses significantly less power than WiFi or cellular  
✅ **OLED Display:** Only illuminates pixels as needed, saving energy

### Charging

Simply connect a Micro-USB cable to the TP4056 module. The LED on the charging module indicates status:
- **Red**: Charging in progress
- **Green**: Fully charged
- **Off**: No power connected

**Charge Time:** Approximately 3-4 hours for a depleted 2500mAh battery

---

## 📡 Communication Range

### Real-World Performance

| Environment | Typical Range | Notes |
|-------------|---------------|-------|
| **Open Fields** | 8-10 km | Clear line-of-sight, ideal conditions |
| **Suburban Areas** | 3-5 km | Houses and trees create obstacles |
| **Urban City** | 1-3 km | Buildings reflect and absorb signals |
| **Indoor** | 200-500 m | Walls significantly reduce range |
| **Mountains/Hills** | 15+ km | Elevation advantage extends range |

### Factors Affecting Range

**🌲 Terrain & Obstacles**  
Trees, buildings, and hills block or reflect radio waves. Higher elevation dramatically improves range.

**📶 Antenna Quality**  
A proper antenna matched to your LoRa frequency is critical. The included wire antenna works, but upgrading to a tuned antenna can double your range.

**⚡ Spreading Factor**  
ECS_Dev automatically adjusts spreading factor (SF7-SF12) based on signal quality. Higher SF means longer range but slower data transfer.

**🌦️ Weather**  
Heavy rain slightly reduces range. Generally, weather has minimal impact on LoRa performance.

### Mesh Network Advantage

With multiple devices, messages relay automatically:
```
Device A ←─── 5km ────→ Device B ←─── 5km ────→ Device C

Effective Range: Device A can reach Device C (10km away) 
through Device B's relay, even though direct communication 
would be impossible!
```

---

## 🔧 Troubleshooting

### Common Issues & Solutions

**Problem: Device Won't Power On**
- ✓ Check battery polarity (positive to positive, negative to negative)
- ✓ Verify battery has charge (measure voltage - should be >3.0V)
- ✓ Ensure TP4056 output is connected to ESP32 VIN pin
- ✓ Try powering via USB cable directly

**Problem: Display Shows Nothing**
- ✓ Verify I2C connections (SDA to GPIO21, SCL to GPIO22)
- ✓ Check OLED receives power (3.3V and GND)
- ✓ Confirm I2C address in code matches your display (usually 0x3C)
- ✓ Test OLED separately with example sketch

**Problem: Can't Send or Receive Messages**
- ✓ Ensure antenna is properly connected to LoRa module
- ✓ Verify LoRa frequency matches between devices
- ✓ Check SPI wiring between ESP32 and LoRa module
- ✓ Confirm both devices are using the same code version
- ✓ Increase spreading factor for better reception

**Problem: Upload Fails**
- ✓ Hold BOOT button on ESP32 when upload begins
- ✓ Select correct port in Tools > Port
- ✓ Try different USB cable (some are charge-only)
- ✓ Install CP210x or CH340 USB drivers if needed
- ✓ Reduce upload speed to 115200

**Problem: Short Range**
- ✓ Ensure antenna is vertical and fully extended
- ✓ Verify antenna frequency matches LoRa module
- ✓ Check for loose connections in RF path
- ✓ Test in open area away from interference
- ✓ Consider upgrading to a higher-gain antenna

**Problem: Battery Drains Quickly**
- ✓ Ensure device enters sleep mode (check code configuration)
- ✓ Verify battery capacity (test with multimeter)
- ✓ Check for short circuits in wiring
- ✓ Reduce unnecessary transmissions

---

## 📊 Technical Specifications

### Hardware Details

**Microcontroller**
- **Chip:** ESP32-WROOM-32
- **CPU:** Dual-core Xtensa LX6, 240MHz
- **RAM:** 520KB SRAM
- **Flash:** 4MB
- **Operating Voltage:** 3.0-3.6V

**Radio Module**
- **IC:** Semtech SX1276 or SX1278
- **Frequency Bands:** 433MHz, 868MHz, 915MHz
- **Transmit Power:** Up to 20dBm (100mW)
- **Sensitivity:** -148dBm
- **Modulation:** LoRa spread spectrum

**Communication Parameters**
- **Bandwidth:** 125kHz (configurable to 250/500kHz)
- **Spreading Factor:** SF7 to SF12 (adaptive)
- **Coding Rate:** 4/5 (forward error correction)
- **Preamble Length:** 8 symbols
- **Max Payload:** 256 bytes per packet

**Display**
- **Type:** OLED (Organic LED)
- **Resolution:** 128x64 pixels
- **Driver:** SSD1306
- **Interface:** I2C
- **Viewing Angle:** 160°

**Power Specifications**
- **Battery Type:** 18650 Li-ion, 3.7V nominal
- **Capacity:** 2500-3500mAh (user choice)
- **Charging:** Micro-USB, 5V input
- **Charging Current:** 1A (adjustable via TP4056)

**Environmental**
- **Operating Temperature:** -20°C to +60°C
- **Storage Temperature:** -40°C to +85°C
- **Humidity:** 0-95% non-condensing
- **Enclosure Rating:** IP54 (with proper case)

---

## 🔒 Security & Privacy Considerations

### Current Implementation

**⚠️ Important Security Notice**

ECS_Dev in its current form is designed for **emergency scenarios** where speed and reliability trump privacy. You should be aware of the following:

**No Encryption**  
Messages are transmitted as plain text. Anyone with a compatible LoRa receiver can intercept and read your communications.

**No Authentication**  
There's no verification of sender identity. Malicious actors could potentially spoof sender IDs or inject false messages.

**Open Broadcast**  
All messages are publicly broadcast to any device in range. Think of it like shouting in a crowded room - everyone can hear.

**No Message Tracking**  
The system doesn't confirm message delivery or provide read receipts.

### When Security Matters

For production deployments where privacy is critical, consider implementing:

**🔐 AES-256 Encryption**  
Add the [AESLib library](https://github.com/DavyLandman/AESLib) for Arduino to encrypt message payloads.

**🔑 Pre-Shared Keys**  
Devices in your network share a secret key for encryption/decryption.

**✍️ Digital Signatures**  
Use ECDSA signatures to verify message authenticity and prevent spoofing.

**🆔 Device Whitelisting**  
Only accept messages from known, trusted device IDs.

### Best Practices

- Don't transmit sensitive personal information
- Use code words or predetermined signals for critical information
- Assume anyone within range can intercept messages
- Test security additions thoroughly before deployment

---

## 📡 Message Protocol

### Packet Structure

Every message follows a standardized format for compatibility:

```
[SENDER_ID]|[TIMESTAMP]|[MSG_TYPE]|[CONTENT]

Example Messages:
DEV_A3F2|1730140800|MSG|Water levels rising quickly
DEV_B7E1|1730140825|EMRG|MEDICAL|Injured person at coordinates
DEV_C2A9|1730140850|BCST|All clear, evacuation route open
```

### Message Types

| Code | Type | Purpose | Priority |
|------|------|---------|----------|
| `MSG` | Normal Message | Standard text communication | Low |
| `BCST` | Broadcast | Wide-area announcements | Medium |
| `EMRG` | Emergency | Urgent alerts requiring immediate attention | High |
| `ACK` | Acknowledgment | Message receipt confirmation | Low |

### Packet Flow

```
┌──────────────────────────────────────────────────────────┐
│  Application Layer (Your Message)                        │
├──────────────────────────────────────────────────────────┤
│  Protocol Layer (Adds Header Info)                       │
├──────────────────────────────────────────────────────────┤
│  LoRa MAC Layer (Error Correction, CRC)                  │
├──────────────────────────────────────────────────────────┤
│  Physical Layer (Radio Transmission)                     │
└──────────────────────────────────────────────────────────┘
```

### Limitations

- **Maximum Message Length:** 200 characters (imposed by LoRa payload limits)
- **Character Set:** ASCII alphanumeric and basic punctuation
- **Transmission Time:** 0.5-2 seconds depending on message length and SF
- **Retry Logic:** Not implemented (send-and-forget approach)

---

## 🛣️ Roadmap & Future Enhancements

We're continuously improving ECS_Dev. Here's what's on the horizon:

### Version 2.0 (In Progress)

- [x] Core messaging functionality
- [x] Emergency broadcast system
- [x] Power management and sleep modes
- [ ] **GPS Integration** - Share location coordinates with messages
- [ ] **Message Acknowledgment** - Confirm delivery with ACK packets
- [ ] **AES-256 Encryption** - Secure communications
- [ ] **Multi-Channel Support** - Separate channels for different groups

### Version 3.0 (Planned)

- [ ] **Voice Messages** - Record and transmit short audio clips
- [ ] **SD Card Storage** - Save 1000+ messages locally
- [ ] **Solar Panel Support** - Indefinite operation with solar charging
- [ ] **Web Configuration** - WiFi-based setup and management interface
- [ ] **Group Messaging** - Private channels with access control
- [ ] **Mesh Routing Improvements** - Smarter relay algorithms

### Long-Term Vision

- [ ] **Mobile App Companion** - Bluetooth bridge to smartphone
- [ ] **Weather Station Integration** - Share environmental data
- [ ] **Gateway Mode** - Bridge to internet when available
- [ ] **Custom PCB Design** - Compact, production-ready board
- [ ] **3D Printable Case** - Ruggedized, weatherproof enclosure

**Want to help?** Check the [Contributing](#-join-the-mission) section!

---

## 🤝 Join the Mission

This project thrives on community contributions. Whether you're a seasoned developer or just getting started, there's a place for you here.

### How You Can Contribute

**🐛 Report Bugs**  
Found something broken? [Open an issue](https://github.com/Tukeshkumar17/ECS_dev/issues) with details about the problem and steps to reproduce it.

**💡 Suggest Features**  
Have ideas for improvements? Start a [discussion](https://github.com/Tukeshkumar17/ECS_dev/discussions) and let's talk about it!

**📝 Improve Documentation**  
Help others by clarifying instructions, adding examples, or translating to other languages.

**💻 Submit Code**  
Ready to contribute code? Awesome! Here's how:

1. **Fork** the repository to your GitHub account
2. **Clone** your fork locally:
   ```bash
   git clone https://github.com/Tukeshkumar17/ECS_dev.git
   ```
3. **Create** a feature branch:
   ```bash
   git checkout -b feature/new-feature
   ```
4. **Make** your changes and test thoroughly
5. **Commit** with clear messages:
   ```bash
   git commit -m "Add: GPS coordinate sharing feature"
   ```
6. **Push** to your fork:
   ```bash
   git push origin feature/new-feature
   ```
7. **Open** a Pull Request with a detailed description

### Contribution Guidelines

- Write clear, commented code
- Follow the existing code style
- Test your changes on actual hardware when possible
- Update documentation for new features
- Keep PRs focused on a single feature or fix

### Code of Conduct

Be respectful, inclusive, and constructive. We're all here to build something that could save lives during emergencies. Toxic behavior won't be tolerated.

---

## 📄 License

This project is released under the **MIT License**, which means:

✅ You can use it for personal projects  
✅ You can use it for commercial projects  
✅ You can modify it however you want  
✅ You can distribute it freely  

The only requirement: include the original copyright notice.

See the [LICENSE](LICENSE) file for complete details.

---

## ⚠️ Legal & Regulatory Disclaimer

### Radio Frequency Regulations

**Important:** Operating radio transmitters is regulated in every country. Before using ECS_Dev, ensure compliance with local laws.

**Frequency Band Regulations:**
- **433MHz (Europe/Asia):** Generally ISM band, unlicensed for low-power use
- **915MHz (Americas):** ISM band in US, but verify local regulations
- **Power Limits:** Most regions allow 100mW (20dBm) or less without license

**Your Responsibilities:**
- Check your country's telecommunications authority website
- Verify allowed frequencies and power limits
- Obtain necessary licenses if required
- Use appropriate antenna types

### Liability Waiver

This device is provided as-is for **emergency communication purposes only**. The creators, contributors, and maintainers:

- Make no warranties about device reliability or performance
- Are not responsible for regulatory violations or misuse
- Cannot guarantee message delivery or reception
- Assume no liability for consequences of device use or failure

**In emergencies:** Always prioritize established emergency services (911, etc.) when available. ECS_Dev is a backup communication tool, not a replacement for professional emergency response systems.

---

## 📞 Support & Community

### Get Help

**📖 Documentation Issues**  
If something in this README is unclear, [open an issue](https://github.com/Tukeshkumar17/ECS_dev/issues) with the "documentation" label.

**🐞 Technical Problems**  
Post detailed bug reports in [GitHub Issues](https://github.com/Tukeshkumar17/ECS_dev/issues). Include:
- Hardware configuration
- Code version
- Error messages or unexpected behavior
- Steps to reproduce

**💬 General Questions**  
Join the conversation in [GitHub Discussions](https://github.com/Tukeshkumar17/ECS_dev/discussions) for:
- Build advice and tips
- Feature discussions
- Sharing your deployments
- Connecting with other builders

### Stay Updated

- **⭐ Star this repo** to get notifications about new releases
- **👀 Watch** for updates and discussions
- **🍴 Fork** to start building your own version

---

## 🙏 Acknowledgments

**Built with open-source excellence:**
- [Arduino](https://www.arduino.cc/) - Development platform
- [ESP32](https://www.espressif.com/) - Espressif Systems' powerful microcontroller
- [LoRa Library](https://github.com/sandeepmistry/arduino-LoRa) by Sandeep Mistry
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) - Graphics library
- [Semtech](https://www.semtech.com/) - LoRa technology pioneers

**Inspired by real-world needs** and dedicated to everyone working on disaster resilience and humanitarian technology.

---

<div align="center">

**Built with ❤️ by TukeshKS**

*When traditional networks fail, community-powered communication prevails.*

[⬆ Back to Top](#-ecdev---emergency-communication-system)

</div>
