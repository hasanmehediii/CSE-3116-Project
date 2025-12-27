# 🚒 FireFighter Robot Car

<div align="center">

![Demo](Demo.gif)

**An autonomous fire-fighting robot car built with ESP32, featuring automatic flame detection, tracking, and extinguishing capabilities with manual override control via WiFi.**

[![ESP32](https://img.shields.io/badge/ESP32-Microcontroller-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Arduino](https://img.shields.io/badge/Arduino-IDE-00979D.svg)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Components Required](#-components-required)
- [Circuit Connections](#-circuit-connections)
- [Pin Configuration](#-pin-configuration)
- [How It Works](#-how-it-works)
- [Installation & Setup](#-installation--setup)
- [Usage](#-usage)
- [Code Structure](#-code-structure)
- [Troubleshooting](#-troubleshooting)
- [Future Improvements](#-future-improvements)
- [License](#-license)

---

## 🔥 Overview

This project is an ESP32-based autonomous firefighter robot car that can detect, track, and extinguish flames. It features both **automatic** and **manual** control modes accessible through a responsive web interface. The robot uses flame sensors for fire detection, a servo-mounted water pump for extinguishing, and a dual-motor drive system for navigation.

---

## ✨ Features

- **🤖 Autonomous Fire Detection**: Three flame sensors (left, center, right) for 180° fire detection
- **🎯 Smart Tracking**: Automatically rotates and moves toward detected flames
- **💧 Servo-Controlled Water Spray**: Sweeping water spray pattern for effective fire suppression
- **📱 WiFi Web Interface**: Control the robot from any device connected to its WiFi network
- **🔄 Dual Mode Operation**: 
  - **AUTO Mode**: Autonomous fire detection and extinguishing
  - **MANUAL Mode**: Full directional control via web interface
- **🎮 Responsive Controls**: Real-time button controls with visual feedback
- **⚡ PWM Motor Control**: Smooth speed control for precise movements

---

## 🛠️ Components Required

### Electronics
| Component | Quantity | Description |
|-----------|----------|-------------|
| **ESP32 Development Board** | 1 | Main microcontroller |
| **L298N Motor Driver** | 1 | Dual H-Bridge for motor control |
| **DC Motors (with wheels)** | 2 | For robot movement |
| **Flame Sensors** | 3 | KY-026 or similar IR flame sensors |
| **SG90 Servo Motor** | 1 | For water nozzle direction control |
| **Mini Water Pump** | 1 | 3-6V DC submersible pump |
| **5V Relay Module** | 1 | To control water pump |
| **BC547 NPN Transistor** | 1 | For relay switching |
| **1kΩ Resistor** | 1 | Base resistor for transistor |
| **18650 Batteries** | 2-3 | Power supply (7.4V or 11.1V) |
| **Battery Holder** | 1 | For batteries |
| **Jumper Wires** | - | Male-to-male and male-to-female |
| **Chassis** | 1 | Robot car base with wheels |

### Optional
- Small water container/tank
- Plastic tubing for water delivery
- Switch for main power
- LED indicators

---

## 🔌 Circuit Connections

### 1. **L298N Motor Driver to ESP32**

| L298N Pin | ESP32 Pin | Description |
|-----------|-----------|-------------|
| ENA | GPIO 25 | Left motor speed (PWM) |
| IN1 | GPIO 26 | Left motor direction 1 |
| IN2 | GPIO 27 | Left motor direction 2 |
| ENB | GPIO 14 | Right motor speed (PWM) |
| IN3 | GPIO 18 | Right motor direction 1 |
| IN4 | GPIO 13 | Right motor direction 2 |
| 12V/VCC | Battery + | Motor power supply |
| GND | Battery - & ESP32 GND | Common ground |

**Note**: Remove jumpers on ENA and ENB for PWM speed control.

### 2. **Flame Sensors to ESP32**

| Sensor Position | ESP32 Pin | Connection |
|----------------|-----------|------------|
| Left Flame Sensor | GPIO 34 | Digital OUT → GPIO 34 |
| Center Flame Sensor | GPIO 35 | Digital OUT → GPIO 35 |
| Right Flame Sensor | GPIO 32 | Digital OUT → GPIO 32 |
| VCC (all sensors) | 3.3V | Power |
| GND (all sensors) | GND | Ground |

**Sensor Logic**: Output = LOW when flame detected, HIGH when no flame.

### 3. **Servo Motor Connection**

| Servo Wire | ESP32 Pin | Description |
|------------|-----------|-------------|
| Signal (Orange/Yellow) | GPIO 33 | PWM control signal |
| VCC (Red) | 5V | Power (can use external 5V if needed) |
| GND (Brown/Black) | GND | Ground |

### 4. **Water Pump with Relay & BC547 Transistor**

This circuit uses a BC547 NPN transistor to safely switch the relay from the ESP32:

```
ESP32 GPIO 23 → 1kΩ Resistor → BC547 Base (Middle pin)
BC547 Emitter (Left pin) → GND
BC547 Collector (Right pin) → Relay IN pin

Relay Module:
├─ VCC → 5V
├─ GND → GND
├─ IN → BC547 Collector
├─ COM → Pump VCC (+)
└─ NO → Battery/Power Supply (+)

Water Pump:
├─ Positive → Relay COM
└─ Negative → Battery GND
```

**Why use BC547?**
- ESP32 GPIO pins output only 3.3V with limited current (~40mA)
- Relay coils typically need 5V and more current
- BC547 acts as a switch, allowing ESP32 to control the relay safely

---

## 📍 Pin Configuration

```cpp
/* MOTOR PINS (L298N) */
#define ENA 25         // Left motor speed
#define IN1 26         // Left motor direction
#define IN2 27         // Left motor direction
#define ENB 14         // Right motor speed
#define IN3 18         // Right motor direction
#define IN4 13         // Right motor direction

/* SERVO */
#define SERVO_PIN 33   // Water nozzle servo

/* FLAME SENSORS */
#define FLAME_LEFT 34     // Left flame sensor
#define FLAME_CENTER 35   // Center flame sensor
#define FLAME_RIGHT 32    // Right flame sensor

/* RELAY / PUMP */
#define PUMP_RELAY 23  // Pump control via transistor
```

---

## ⚙️ How It Works

### AUTO Mode Operation

1. **Fire Detection**: The robot continuously monitors all three flame sensors
2. **Target Acquisition**:
   - If **center sensor** detects fire → Move forward toward flame
   - If **left sensor** detects fire → Rotate right until center sensor locks on
   - If **right sensor** detects fire → Rotate left until center sensor locks on
3. **Approach**: Robot moves closer to the fire for 0.6 seconds
4. **Verification**: Double-checks if fire is still detected by center sensor
5. **Extinguish**: Executes spray cycle:
   - Activates water pump
   - Servo sweeps from 70° to 110° and back (covering the fire area)
   - Pump turns off after spray cycle
6. **Repeat**: Returns to scanning mode

### MANUAL Mode Operation

- User has full control via web interface
- Directional buttons: Forward, Backward, Left, Right
- Manual water pump control (hold to spray)
- Stop button for emergency halt

### Web Interface

- **WiFi Access Point**: Robot creates "FireFighter_Bot" network (password: 12345678)
- **Control Panel**: Responsive web interface accessible at `http://192.168.4.1`
- **Features**: Mode toggle, directional controls, pump control

---

## 📥 Installation & Setup

### 1. **Arduino IDE Setup**

Install required libraries:
```
- ESP32 Board Support
- ESP32Servo library
```

**Steps**:
1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add ESP32 board URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
4. Go to **Tools → Board → Boards Manager**, search "ESP32" and install
5. Install **ESP32Servo** library: **Sketch → Include Library → Manage Libraries** → Search "ESP32Servo"

### 2. **Upload Code**

1. Connect ESP32 to computer via USB
2. Select board: **Tools → Board → ESP32 Dev Module**
3. Select correct COM port: **Tools → Port**
4. Click **Upload** button

### 3. **Hardware Assembly**

1. Mount motors and motor driver on chassis
2. Connect all components according to circuit diagram
3. Secure ESP32, sensors, and relay module
4. Mount servo with water nozzle at front
5. Attach water container and connect tubing to pump
6. Connect battery (ensure proper voltage: 7.4V-11.1V)

---

## 🎮 Usage

### Connecting to Robot

1. **Power on** the robot
2. Wait for ESP32 to boot (~5 seconds)
3. On your phone/laptop, connect to WiFi:
   - **SSID**: `FireFighter_Bot`
   - **Password**: `12345678`
4. Open browser and go to: `http://192.168.4.1`

### Web Interface Controls

**Mode Selection**:
- **AUTO**: Robot autonomously detects and fights fires
- **MANUAL**: User controls all movements

**Manual Controls**:
- **▲** Forward
- **▼** Backward  
- **◄** Turn Left
- **►** Turn Right
- **■** Stop
- **HOLD FOR WATER 💧**: Press and hold to activate pump

### Testing

1. Start in MANUAL mode to test motors and pump
2. Switch to AUTO mode
3. Use a lighter or candle to test flame detection
4. Observe robot's tracking and extinguishing behavior

---

## 📂 Code Structure

```cpp
/* Main Sections */
├─ WiFi & WebServer Setup
├─ Pin Definitions
├─ Global Variables (mode, speeds)
├─ setup() - Initialize all components
├─ loop() - Main logic
│   ├─ Auto mode: Flame detection & response
│   └─ Manual mode: Web command processing
├─ executeSprayCycle() - Water spray pattern
├─ Motor Control Functions
│   ├─ moveForward()
│   ├─ moveBackward()
│   ├─ turnLeft()
│   ├─ turnRight()
│   └─ stopMoving()
└─ handleRoot() - Web interface HTML/CSS
```

---

## 🔧 Troubleshooting

| Problem | Possible Solution |
|---------|------------------|
| **Motors don't move** | • Check L298N connections<br>• Ensure battery is charged<br>• Verify ENA/ENB jumpers are removed<br>• Check motor polarity |
| **Wrong direction** | Swap IN1↔IN2 or IN3↔IN4 connections, or modify `LEFT_FORWARD_LOGIC` and `RIGHT_FORWARD_LOGIC` constants in code |
| **Pump doesn't activate** | • Check relay connections<br>• Verify BC547 transistor orientation<br>• Test relay with multimeter<br>• Check pump power supply |
| **Flame sensors not detecting** | • Adjust sensor sensitivity (potentiometer on sensor)<br>• Check 3.3V power supply<br>• Test sensors individually with Serial monitor |
| **Can't connect to WiFi** | • Reset ESP32<br>• Check WiFi credentials in code<br>• Ensure device WiFi is enabled |
| **Servo jitters** | • Use external 5V power source for servo<br>• Add capacitor across servo power lines |
| **Robot moves too fast/slow** | Adjust `motorSpeed` and `turnSpeed` variables (0-255 range) |

---

## 🚀 Future Improvements

- [ ] Add temperature sensor for heat detection
- [ ] Implement obstacle avoidance with ultrasonic sensors
- [ ] Battery level monitoring and low battery warning
- [ ] Add buzzer for audio alerts
- [ ] Implement camera streaming for remote monitoring
- [ ] Add GPS module for location tracking
- [ ] Create mobile app for better control interface
- [ ] Multiple pump nozzles for wider coverage
- [ ] Data logging to SD card
- [ ] Integration with IoT platforms (Firebase, Blynk)

---

## 📝 License

This project is open-source and available under the MIT License.

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome! Feel free to check the issues page.

---

## 👨‍💻 Author

Created with ❤️ for making fire safety smarter

---

## 📸 Gallery

Add more images/videos of your project here:
- Assembly process
- Circuit diagrams
- Testing photos
- Different angles of the robot

---

<div align="center">

**⭐ If you found this project helpful, please give it a star! ⭐**

</div>
