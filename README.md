<div align="center">

# The Pythons Fast Line Follower

### Autonomous Line-Following Robot: Hardware, Firmware, Vision, and Control

[![Arduino](https://img.shields.io/badge/Arduino-Firmware-00979D?logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Python](https://img.shields.io/badge/Python-Computer%20Vision-3776AB?logo=python&logoColor=white)](https://www.python.org/)

</div>

## Overview

This repository contains the design assets and control software for a high-speed autonomous line-following robot. It brings together mechanical CAD, electrical schematics and PCB files, Arduino firmware, camera-based line-following code, and a Bluetooth control application.

## Project Areas

| Area | Contents |
| --- | --- |
| Hardware | PCB board and schematic design files |
| Mechanical | Base-plate and camera-holder STEP models |
| Firmware | Multiple Arduino sketches with movement, sensor-read, and PID-control modules |
| Vision | Python camera line-following scripts |
| Mobile control | MIT App Inventor source and Android APK |

## Repository Structure

```text
.
├── Hardware/                       # Electrical design assets
├── Mechanical/                     # Robot CAD models
└── Software/
    ├── Bluetooth App/              # App Inventor project and APK
    ├── Camera Code/                # Python vision scripts
    └── The_Pythons_.../            # Arduino firmware variants
```

## Firmware Variants

The Arduino projects include sensor-only, camera-assisted, camera-only, and weighted-control variants. The sketches are organized with separate modules for movement, PID calculations, and sensor readings.

Open the intended `.ino` file with Arduino IDE, select the connected board and serial port, then upload. Calibrate sensor thresholds, motor wiring, and PID values for the physical robot before operating it on a track.

## Author

**Youssef Mohamed Attia**  
[GitHub](https://github.com/YoussefMohAttia) · [LinkedIn](https://www.linkedin.com/in/youssefmohattia/)
