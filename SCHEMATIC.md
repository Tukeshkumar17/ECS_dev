# ECS_Dev Hardware Schematic

## Complete Wiring Diagram

### LoRa Module (SX1276/SX1278) to ESP32

| LoRa Pin | ESP32 Pin | Description |
|----------|-----------|-------------|
| VCC | 3.3V | Power supply (3.3V only!) |
| GND | GND | Ground |
| MISO | GPIO19 | SPI Master In Slave Out |
| MOSI | GPIO27 | SPI Master Out Slave In |
| SCK | GPIO18 | SPI Clock |
| NSS/CS | GPIO5 | Chip Select |
| RESET | GPIO14 | Reset pin |
| DIO0 | GPIO26 | Digital I/O 0 (interrupt) |
| ANT | - | Connect antenna (433MHz or 915MHz) |

⚠️ **CRITICAL**: LoRa modules are 3.3V only! Do NOT use 5V.

### OLED Display (0.96" I2C SSD1306) to ESP32

| OLED Pin | ESP32 Pin | Description |
|----------|-----------|-------------|
| VCC | 3.3V | Power supply |
| GND | GND | Ground |
| SDA | GPIO21 | I2C Data |
| SCL | GPIO22 | I2C Clock |

### Push Buttons to ESP32

| Button | ESP32 Pin | Resistor | Description |
|--------|-----------|----------|-------------|
| UP | GPIO32 | 10kΩ to 3.3V | Navigation up |
| SELECT | GPIO33 | 10kΩ to 3.3V | Confirm/Select |
| DOWN | GPIO25 | 10kΩ to 3.3V | Navigation down |

**Button Wiring**: Each button connects between GPIO pin and GND. The 10kΩ resistor pulls the pin HIGH when button is not pressed.

```
3.3V ----[10kΩ]---- GPIO Pin ----[Button]---- GND
```

### Power Supply Circuit

```
18650 Battery (3.7V)
        |
        |---- TP4056 Charging Module
        |         |
        |         |---- Micro USB (for charging)
        |
        |---- Switch (optional)
        |
        |---- ESP32 VIN Pin
```

| Component | Connection |
|-----------|------------|
| 18650 Battery + | TP4056 B+ |
| 18650 Battery - | TP4056 B- |
| TP4056 OUT+ | ESP32 VIN (or 5V pin) |
| TP4056 OUT- | ESP32 GND |
| USB Port | TP4056 for charging |

### Optional: Battery Voltage Monitor

For accurate battery monitoring, connect a voltage divider:

```
Battery + ----[100kΩ]---- GPIO34 (ADC) ----[100kΩ]---- GND
```

This divides battery voltage by 2, allowing ESP32 to read it safely.

## PCB Layout Recommendations

### Component Placement
1. **LoRa Module**: Place away from ESP32 WiFi antenna
2. **OLED Display**: Front panel mounting
3. **Buttons**: Front panel, ergonomic spacing
4. **Battery**: Bottom/back of enclosure
5. **Antenna**: External or through enclosure hole

### Trace Routing
- **SPI Traces**: Keep short (<5cm), same length
- **I2C Traces**: Keep short, can tolerate more length
- **Power Traces**: Use wider traces (1mm+)
- **Antenna Trace**: Use 50Ω impedance, keep away from other traces

## Enclosure Design

### Recommended Dimensions
- **Size**: 100mm x 60mm x 30mm
- **Material**: ABS plastic or weatherproof case
- **Features**:
  - Display window
  - Button holes
  - Antenna port (SMA connector)
  - USB charging port access
  - Ventilation holes

### Weatherproofing
- IP54+ rating recommended
- Rubber gaskets for display/buttons
- Sealed USB port cover
- External antenna with waterproof connector

## Assembly Steps

### 1. Prepare Components
- [ ] Solder headers to ESP32 if needed
- [ ] Solder headers to LoRa module
- [ ] Test OLED display separately
- [ ] Test LoRa module separately

### 2. Breadboard Prototype
- [ ] Connect all components on breadboard
- [ ] Upload test code
- [ ] Verify each component works
- [ ] Test button inputs
- [ ] Test LoRa transmission/reception

### 3. Solder Permanent Connections
- [ ] Use 22-26 AWG wire for connections
- [ ] Solder LoRa module connections
- [ ] Solder OLED connections
- [ ] Solder button connections with pull-up resistors
- [ ] Solder power connections

### 4. Final Assembly
- [ ] Mount components in enclosure
- [ ] Attach antenna
- [ ] Install battery
- [ ] Secure all connections
- [ ] Test complete system

## Testing Checklist

### Power Tests
- [ ] Battery charges correctly
- [ ] Device powers on with battery
- [ ] Current draw: <150mA active, <1mA sleep

### Display Tests
- [ ] Splash screen appears
- [ ] All text readable
- [ ] No dead pixels
- [ ] Good contrast

### Button Tests
- [ ] UP button responds
- [ ] SELECT button responds
- [ ] DOWN button responds
- [ ] No false triggers

### LoRa Tests
- [ ] Transmits successfully
- [ ] Receives messages from other devices
- [ ] Range test: >500m line of sight
- [ ] RSSI values reasonable (-50 to -120 dBm)

### Integration Tests
- [ ] Send message works
- [ ] Receive message works
- [ ] Inbox stores messages
- [ ] Broadcast works
- [ ] Sleep mode activates
- [ ] Wake from sleep works

## Troubleshooting Guide

### Display Issues

**Problem**: Display shows nothing
- Check VCC/GND connections
- Verify I2C address (try 0x3C or 0x3D)
- Check SDA/SCL connections
- Test with I2C scanner sketch

**Problem**: Display dim or flickering
- Check power supply voltage
- Check for loose connections
- Replace display if faulty

### LoRa Issues

**Problem**: LoRa initialization fails
- Check all SPI connections
- Verify 3.3V power (NOT 5V!)
- Check CS, RESET, DIO0 pins
- Verify correct frequency in code

**Problem**: No messages received
- Check antenna connection
- Verify frequency matches other devices
- Check if antenna is correct frequency
- Verify spreading factor settings
- Test with two devices close together

**Problem**: Short range
- Check antenna is proper length
- Verify antenna connection is good
- Increase TX power (max 20 dBm)
- Check for interference
- Ensure antenna is vertical

### Button Issues

**Problem**: Buttons don't respond
- Check pull-up resistor connections
- Verify GPIO pin assignments
- Test with multimeter for continuity
- Check for cold solder joints

**Problem**: False button triggers
- Add debounce capacitor (0.1µF)
- Increase software debounce delay
- Check for electrical noise

### Power Issues

**Problem**: Device resets randomly
- Battery voltage too low
- Insufficient current supply
- Add decoupling capacitor near ESP32 (100µF)
- Check all ground connections

**Problem**: Battery drains fast
- Reduce TX power if not needed
- Increase sleep timeout
- Check for current leaks
- Verify sleep mode working

## Bill of Materials (BOM)

| Qty | Component | Specification | Approx. Cost |
|-----|-----------|---------------|--------------|
| 1 | ESP32 DevKit | ESP32-WROOM-32 | $5-8 |
| 1 | LoRa Module | SX1276/SX1278 433/915MHz | $8-15 |
| 1 | OLED Display | 0.96" 128x64 I2C | $3-5 |
| 3 | Push Buttons | 6x6mm tactile | $0.50 |
| 3 | Resistors | 10kΩ 1/4W | $0.15 |
| 1 | 18650 Battery | 3.7V 2000-3000mAh | $3-6 |
| 1 | Battery Holder | 18650 with leads | $1-2 |
| 1 | TP4056 Module | Li-ion charger | $1-2 |
| 1 | Antenna | 433/915MHz with connector | $2-5 |
| 1 | Enclosure | Weatherproof box | $5-10 |
| - | Wire | 22-26 AWG various colors | $2 |
| - | Solder | Lead-free | $3 |
| **Total** | | | **$35-65** |

## Advanced Modifications

### Add GPS Module
- Connect NEO-6M GPS to UART pins
- Add location to messages
- Requires additional library

### Add Solar Panel
- 5V 1W solar panel
- Connect to TP4056 solar input
- Enables indefinite operation

### Add Buzzer
- Piezo buzzer to GPIO pin
- Alert on emergency broadcasts
- Add beep patterns

### Add Encryption
- Implement AES-256
- Use ESP32 crypto library
- Exchange keys via NFC or manual entry

### Increase Storage
- Add SD card module
- Store unlimited messages
- Log all communications
