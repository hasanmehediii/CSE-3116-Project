# 🎓 FireFighter Robot Car - Viva Preparation Guide

## 📋 Table of Contents
1. [ESP32 Microcontroller Questions (30)](#esp32-questions)
2. [Project-Specific Questions (30)](#project-questions)

---

## ESP32 Questions

### 1. What is ESP32?
ESP32 is a low-cost, low-power system on a chip (SoC) microcontroller with integrated Wi-Fi and Bluetooth capabilities, developed by Espressif Systems. It features a dual-core Tensilica Xtensa LX6 processor running at up to 240 MHz, making it ideal for IoT applications.

### 2. What are the main features of ESP32?
ESP32 offers built-in Wi-Fi (802.11 b/g/n) and Bluetooth 4.2, 34 programmable GPIO pins, multiple communication interfaces (UART, SPI, I2C), ADC, DAC, PWM, and touch sensors. It has 520 KB SRAM and supports external flash memory up to 16 MB.

### 3. What is the operating voltage of ESP32?
ESP32 operates at 3.3V logic level. The development boards typically accept 5V input through USB or VIN pin, which is regulated down to 3.3V using an onboard voltage regulator. Applying 5V directly to GPIO pins can damage the chip.

### 4. How many GPIO pins does ESP32 have?
ESP32 has 34 GPIO pins, though not all are usable. Some pins are input-only (GPIO 34-39), some are used for internal flash (GPIO 6-11), and some have special functions. Typically, around 25-28 pins are available for general use.

### 5. What is the difference between ESP32 and Arduino?
Arduino (like Uno) is an 8-bit AVR microcontroller with limited processing power and no built-in wireless connectivity. ESP32 is a 32-bit dual-core processor with integrated Wi-Fi and Bluetooth, significantly more processing power, more memory, and lower cost.

### 6. What is PWM and how does ESP32 generate it?
PWM (Pulse Width Modulation) is a technique to control analog devices using digital signals by varying the duty cycle. ESP32 has 16 independent PWM channels (LEDC) with configurable frequency and resolution up to 16-bit, allowing precise motor speed and LED brightness control.

### 7. What are the ADC capabilities of ESP32?
ESP32 has two 12-bit SAR ADCs supporting up to 18 channels. ADC1 has 8 channels (GPIO 32-39) and ADC2 has 10 channels (GPIO 0, 2, 4, 12-15, 25-27). Note that ADC2 cannot be used when Wi-Fi is active.

### 8. How does ESP32 connect to Wi-Fi?
ESP32 uses the WiFi.h library to connect to networks. It supports Station mode (connects to existing network), Access Point mode (creates its own network), and AP+STA mode (both simultaneously). Connection requires SSID and password configuration.

### 9. What is the clock speed of ESP32?
ESP32 has a dual-core processor that can run at frequencies from 80 MHz to 240 MHz. The clock speed is configurable through software, allowing power consumption optimization. Higher speeds provide better performance but consume more power.

### 10. What communication protocols does ESP32 support?
ESP32 supports UART (serial communication), SPI (high-speed peripheral communication), I2C (two-wire protocol for sensors), I2S (audio data), CAN bus, and Ethernet MAC. It also has built-in Wi-Fi and Bluetooth for wireless communication.

### 11. Can ESP32 be programmed using Arduino IDE?
Yes, ESP32 can be programmed using Arduino IDE by installing the ESP32 board package. This allows developers to use familiar Arduino syntax and libraries while leveraging ESP32's advanced features. The board support is maintained by Espressif.

### 12. What is the difference between ESP32 and ESP8266?
ESP8266 has a single-core processor, less GPIO pins (11 usable), and no Bluetooth. ESP32 has dual-core, more GPIOs (34), Bluetooth 4.2, more ADC channels, and better processing power, though it consumes slightly more power.

### 13. How much current can ESP32 GPIO pins source/sink?
Each GPIO pin can source or sink a maximum of 40mA, with a recommended maximum of 20-30mA for reliable operation. The total current for all GPIO pins combined should not exceed 200mA to prevent damage to the chip.

### 14. What is Deep Sleep mode in ESP32?
Deep Sleep is an ultra-low power mode where most of the ESP32 chip is powered down, consuming only 10-150µA. Only the RTC controller, RTC peripherals, and RTC memory remain active. Wake-up sources include timer, touch pad, or external signal.

### 15. What are strapping pins in ESP32?
Strapping pins (GPIO 0, 2, 5, 12, 15) are sampled during boot to determine boot mode and other configurations. GPIO 0 determines boot mode (LOW for programming, HIGH for normal), so avoid conflicting connections on these pins.

### 16. How does ESP32 create a web server?
ESP32 uses the WebServer library to create an HTTP server. It listens on port 80, handles GET/POST requests, and can serve HTML pages, handle form submissions, and send responses. The handleClient() function processes incoming requests.

### 17. What is SPIFFS in ESP32?
SPIFFS (Serial Peripheral Interface Flash File System) is a file system for ESP32's flash memory. It allows storing files like HTML, CSS, images, and configuration data that persist across reboots. Files can be accessed using standard file operations.

### 18. Can ESP32 handle multiple tasks simultaneously?
Yes, ESP32's dual-core architecture and FreeRTOS support true multitasking. Tasks can be assigned to specific cores (Core 0 or Core 1), with priorities and independent execution. This is ideal for handling sensors, communication, and control simultaneously.

### 19. What is the flash memory size of ESP32?
ESP32 development boards typically come with 4MB external SPI flash memory, though variants with 2MB, 8MB, or 16MB exist. Flash stores the program code, while SRAM (520KB) is used for runtime variables and stack.

### 20. What is the purpose of the EN (Enable) pin?
The EN (Enable) or RST (Reset) pin is used to reset the ESP32. Pulling it LOW resets the chip, while keeping it HIGH allows normal operation. It's typically connected to a pull-up resistor and a reset button.

### 21. How does ESP32 handle interrupts?
ESP32 supports hardware interrupts on any GPIO pin using attachInterrupt() function. Interrupts can be triggered on RISING, FALLING, CHANGE, or LOW/HIGH states. Interrupt Service Routines (ISR) should be short and use IRAM_ATTR attribute for fast execution.

### 22. What is the WDT (Watchdog Timer) in ESP32?
Watchdog Timer monitors program execution and resets the ESP32 if it detects a hang or infinite loop. ESP32 has multiple WDTs including Task WDT, Interrupt WDT, and RTC WDT. They must be fed/reset periodically to prevent automatic resets.

### 23. Can ESP32 work with 5V sensors directly?
No, ESP32 GPIO pins are 3.3V tolerant only. Connecting 5V signals directly can damage the chip. Use level shifters, voltage dividers, or opt for 3.3V compatible sensors. Many modern sensors support both voltage levels.

### 24. What is the boot process of ESP32?
On power-up, ESP32 executes bootloader from ROM, checks strapping pins to determine boot mode, then loads user program from flash memory. If GPIO 0 is LOW during boot, it enters download mode for programming.

### 25. How does Bluetooth work on ESP32?
ESP32 supports Bluetooth Classic and BLE (Bluetooth Low Energy). It can act as server or client, supporting profiles like SPP (Serial Port Profile) for serial communication. BLE is more power-efficient for IoT applications.

### 26. What libraries are commonly used with ESP32?
Common libraries include WiFi.h (Wi-Fi connectivity), WebServer.h (HTTP server), Wire.h (I2C), SPI.h (SPI communication), ESP32Servo.h (servo control), and BluetoothSerial.h (Bluetooth). Arduino.h provides core functions.

### 27. What is the role of the Xtensa processor in ESP32?
Xtensa is a configurable 32-bit RISC processor architecture used in ESP32's dual-core design. It provides efficient instruction execution, low power consumption, and supports DSP operations, making it suitable for real-time embedded applications.

### 28. Can ESP32 be battery powered?
Yes, ESP32 can run on batteries (typically 3.7V LiPo or 3xAA). However, continuous Wi-Fi operation drains batteries quickly. Using Deep Sleep mode, power consumption can be reduced to 10-150µA, extending battery life significantly for IoT sensors.

### 29. What is the purpose of the Boot button on ESP32 dev boards?
The Boot button (connected to GPIO 0) is used to manually enter programming/download mode. Holding it while pressing Reset puts ESP32 into flash mode, allowing new programs to be uploaded via USB.

### 30. What is OTA (Over The Air) programming in ESP32?
OTA allows updating ESP32 firmware wirelessly over Wi-Fi without physical USB connection. The new firmware is downloaded to unused flash space, verified, and then ESP32 reboots to run the new code, ideal for remote deployments.

---

## Project Questions

### 1. What is the main objective of your FireFighter Robot project?
The main objective is to create an autonomous robot that can detect, track, and extinguish fires without human intervention, thereby reducing the risk to firefighters' lives. It addresses the problem of firefighter casualties during rescue operations in Bangladesh.

### 2. Why did you choose ESP32 for this project?
ESP32 was chosen because it provides built-in Wi-Fi for remote control, sufficient GPIO pins for multiple sensors and actuators, PWM capabilities for motor control, and enough processing power for autonomous decision-making, all at a low cost.

### 3. How does the robot detect fire?
The robot uses three KY-026 flame sensors (left, center, right) that detect infrared radiation from flames. These sensors output LOW when fire is detected within their range (up to 1 meter), providing 180° coverage for comprehensive fire detection.

### 4. Explain the AUTO mode operation of your robot.
In AUTO mode, the robot continuously scans all flame sensors. When fire is detected, it rotates to align the center sensor with the flame, moves forward to approach, verifies the fire is still present, then executes a spray cycle with servo-controlled water nozzle.

### 5. How does the manual control work?
Manual control is accessed via a web interface at 192.168.4.1 when connected to the robot's Wi-Fi network. Users can control directional movement (forward, backward, left, right), stop the robot, and manually activate the water pump using on-screen buttons.

### 6. What is the purpose of using three flame sensors?
Three sensors provide 180° fire detection coverage and enable the robot to determine fire direction. Left/right sensors help the robot rotate toward the fire, while the center sensor confirms alignment for accurate approach and extinguishing.

### 7. How does the L298N motor driver work in your project?
L298N is a dual H-bridge driver that controls two DC motors independently. It receives direction signals (IN1-IN4) and speed control via PWM (ENA, ENB) from ESP32, allowing the robot to move forward, backward, turn left/right with variable speed.

### 8. Why did you use a servo motor for the water nozzle?
The servo motor allows precise angular control of the water nozzle direction. By sweeping from 70° to 110° during spraying, it provides wider coverage area, ensuring more effective fire suppression compared to a fixed nozzle position.

### 9. Explain the relay circuit with BC547 transistor.
ESP32's GPIO outputs only 3.3V with limited current, insufficient to drive the relay coil. BC547 NPN transistor acts as a switch: ESP32 controls the base, allowing higher current from the power supply through collector-emitter to activate the relay safely.

### 10. What is the detection range of your flame sensors?
The KY-026 flame sensors can detect flames from approximately 60cm to 1 meter away, depending on fire intensity and sensor sensitivity adjustment via the onboard potentiometer. The detection angle is typically 60° per sensor, combining for 180° total coverage.

### 11. How did you implement the web interface?
The web interface is created using ESP32's WebServer library. HTML, CSS, and JavaScript are embedded as strings in the code. When users connect to the robot's Wi-Fi and visit 192.168.4.1, ESP32 serves this page and handles button click requests.

### 12. What happens if fire is detected on the left sensor?
When the left sensor detects fire, the robot rotates right (both motors or differential speed) until the center sensor locks onto the flame. This alignment ensures the robot faces the fire directly before moving forward to approach it.

### 13. Why do you verify the fire again before spraying?
The verification step (checking center sensor after approach) prevents false positives and wasted water. If the fire was extinguished during approach or was a false detection, the robot returns to scanning instead of unnecessarily activating the pump.

### 14. What is the water spray cycle duration?
The spray cycle involves activating the pump and sweeping the servo from 70° to 110° (40° range) with small increments and delays. Total cycle time is approximately 2-3 seconds, providing thorough coverage while conserving water.

### 15. How is the robot powered?
The robot is powered by 7.4V-11.1V Li-ion battery pack (2-3 18650 cells in series). The L298N motor driver has an onboard 5V regulator that powers the ESP32, while motors run on battery voltage directly for higher torque.

### 16. What challenges did you face with motor direction?
Initial motor connections may cause reversed directions. This was solved by either physically swapping motor wire polarity or modifying the code's LEFT_FORWARD_LOGIC and RIGHT_FORWARD_LOGIC constants to invert the control signals for correct directional movement.

### 17. How do you switch between AUTO and MANUAL modes?
Mode switching is implemented through the web interface with a toggle button. When pressed, it sends an HTTP request to ESP32, which updates a global mode variable. The main loop checks this variable to execute either autonomous or manual control logic.

### 18. What is the purpose of the motorSpeed variable?
The motorSpeed variable (0-255) controls the PWM duty cycle sent to the motor driver's enable pins (ENA, ENB). Higher values mean faster motor rotation. It's set to around 150-200 for balance between speed and control/battery efficiency.

### 19. Why can't you use localStorage in artifacts?
LocalStorage is a browser API for persistent data storage, but it's not supported in Claude's artifact environment. Instead, the project uses React state (useState) or JavaScript variables for in-memory storage during the session, which is sufficient for real-time control.

### 20. How does the water pump activation work?
When GPIO 23 goes HIGH, it turns on the BC547 transistor, which completes the relay coil circuit. The relay then switches its contacts, connecting the water pump to the power supply (battery positive to pump positive), activating the pump.

### 21. What improvements would you make to this project?
Future improvements include adding temperature sensors for heat detection, ultrasonic sensors for obstacle avoidance, GPS module for location tracking, battery level monitoring with low battery alerts, and a camera for remote video monitoring via the web interface.

### 22. Why is the approach time set to 0.6 seconds?
0.6 seconds provides enough time for the robot to get closer to the fire source for effective spraying without getting too close, which could damage components or cause the robot to overshoot. This value was determined through testing and can be adjusted.

### 23. How do you handle multiple fires simultaneously?
Currently, the robot handles one fire at a time based on which sensor detects first. For multiple fires, it would extinguish the detected fire, return to scanning, and then detect and approach the next fire. Priority could be added based on sensor readings.

### 24. What is the maximum water spray distance?
The water spray distance depends on the pump pressure and nozzle design, typically 1-2 meters for small DC pumps. The servo sweeping motion increases effective coverage area, ensuring the water reaches the fire even if alignment isn't perfect.

### 25. Why did you choose Access Point mode for Wi-Fi?
Access Point (AP) mode allows the robot to create its own Wi-Fi network, making it independent of existing networks. This is crucial for emergency situations where infrastructure networks may be unavailable or for field deployment without prior network setup.

### 26. How does the robot handle sensor noise or false positives?
The code implements a verification step where the center sensor is checked again after approach before activating the pump. This double-checking reduces false activations. Additionally, sensor sensitivity can be adjusted via the onboard potentiometer to reduce ambient light interference.

### 27. What safety features are implemented?
Safety features include: verification before spraying (prevents false activations), stop button in manual mode (emergency halt), mode switching ability (human override), and limited approach time (prevents overrun). The relay also provides electrical isolation for the pump circuit.

### 28. How would you scale this for industrial use?
For industrial scaling, we'd need: larger battery/power supply, more powerful motors for heavier chassis, industrial-grade sensors with longer range, multiple water tanks with auto-refill detection, mesh networking for multi-robot coordination, and thermal cameras for better fire detection.

### 29. What real-world incidents inspired this project?
The project was inspired by tragic firefighter casualties in Bangladesh, particularly the Chittagong BM Container Depot fire (2022) where 9 firefighters died, the FR Tower Banani fire (2019) where firefighter Sohel Rana died, and the recent Secretariat fire (2024) where firefighter Shoyanur Zaman Noyon died.

### 30. How does this project contribute to fire safety?
This project provides a first-response solution that can be deployed before human firefighters arrive, potentially containing small fires before they spread. It reduces direct human exposure to dangerous fire situations, buying time for evacuation and allowing firefighters to assess situations remotely before entering hazardous areas.

---

**Good luck with your viva! 🎓🔥🤖**
