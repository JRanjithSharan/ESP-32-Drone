# ESP-32 Drone

This repository hosts the code and configuration for controlling a drone using an ESP-32 microcontroller. The project enables remote control and monitoring through a web-based interface.

## Overview

The ESP-32 Drone project integrates an ESP-32 microcontroller with a drone to allow remote control via a web interface. The system includes features for real-time communication and control, and it provides a user-friendly interface accessible from any device with a web browser.

## Features

- **Web-based Control Interface:** Provides remote control and monitoring of the drone through a web interface.
- **ESP-32 Integration:** Utilizes the ESP-32 microcontroller for efficient control and communication with the drone.
- **Real-time Alerts:** Sends real-time notifications to mobile devices in case of critical events or statuses.
- **Customizable Commands:** Allows for various drone operations including takeoff, landing, and directional movement.

## Hardware Components

- **ESP-32 Microcontroller:** The central control unit for interfacing with the drone and handling web communications.
- **MPU 6050 accelerometer:**  Measures acceleration in three axes, providing data for motion and orientation detection.
- **HMC5883L magnetic compass:** Measures magnetic field strength in three axes, helping determine the drone's heading and orientation.
- **tp4056 USB-C charging module**
- **720 microcoreless motor**
- **Propellers**
- **Custom built ESC:** The Electronic speed controller was custom built to be able to control the 720 micro coreless motor
- **3.7V li-po battery**

## Code Files

- **`main.cpp`**: Contains the core logic for initializing the ESP-32, connecting to Wi-Fi, and managing drone operations. It includes setup and loop functions that handle web server requests and control commands.

- **`web_server.cpp`**: Implements the web server functionality. It processes incoming HTTP requests, updates the drone status, and sends control commands based on user inputs from the web interface.

- **`config.h`**: Configuration file containing network settings and other parameters essential for ESP-32 setup and operation. It includes Wi-Fi credentials and server settings.

## Getting Started

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/JRanjithSharan/ESP-32-Drone.git
   ```

2. **Configure the ESP-32:**

   Modify `config.h` to include your network settings and other configuration details.

3. **Upload Code:**

   Use your preferred development environment (ESP-IDF or Arduino IDE) to upload the code to the ESP-32.

4. **Access the Web Interface:**

   After uploading, connect the ESP-32 to your network. Access the web interface by navigating to the ESP-32's IP address in your web browser.

## Usage

The web interface allows you to control the drone’s movements, including takeoff, landing, and directional adjustments. The web interface is still under development, and further changes will be updated

