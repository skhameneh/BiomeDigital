# BiomeDigital

This is a personal all-inclusive IoT attempt using the ESP32 for BLE -> WiFi -> WebSocket handoff.  

You may explore this code for non-commecial use only.  
This software is only to be used for personal education.

You may not use my GATT identifiers and public SSO services.  
Please generate your own identities.

## Critical Issues

 - Mid-migration to ESP-IDF due to bad router handshakes using arduino-esp32.
 - ESP-IDF BLE stack is incomplete, may move Bluetooth stack to a secondary mbed device.

## Requirements

 - An ESP32 developer kit.
 - An Android or iOS device with Bluetooth support.

## Setup

- Install [VSCode](https://code.visualstudio.com/Download)
- Install [PlatformIO](https://platformio.org/install/ide?install=vscode) for VSCode.
- Install [NodeJS](https://nodejs.org/en/download/)
- Open BiomeDigital.code-workspace in VSCode.

## Prototype

![Breadboard Prototype Image](https://github.com/skhameneh/BiomeDigital/raw/master/Breadboard.png)
