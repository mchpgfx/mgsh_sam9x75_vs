
# MGS Quickstart for SAM9X75 Curiosity Hybrid with Raspberry Pi 7-inch Display

This is a Microchip Graphics Suite (MGS) Harmony quickstart project for the **SAM9X75 Curiosity Hybrid Kit** with the **Raspberry Pi 7-inch Touchscreen Display**.

## User Guide

For complete setup instructions, hardware configuration, and getting started information, refer to the official user guide:

**[SAM9X75 Curiosity Hybrid Kit User Guide](https://ww1.microchip.com/downloads/aemDocuments/documents/MPU32/ProductDocuments/UserGuides/SAM9X75-Curiosity-Hybrid-Kit-User-Guide-DS50004176.pdf)**

## Hardware Requirements

- **Development Board**: [SAM9X75 Curiosity Hybrid Kit](https://www.microchip.com/en-us/development-tool/sam9x75-chy-ev)
- **Display**: [Raspberry Pi 7-inch Touchscreen Display](https://www.raspberrypi.com/products/raspberry-pi-touch-display/)

## About the SAM9X75

The SAM9X75 is an Arm-based embedded MPU running up to 800 MHz. It features:
- MIPI DSI and LVDS with 2D graphics support
- 10/100 Mbps Ethernet
- CAN FD connectivity
- Security features including tamper detection and secure boot
- Graphical display support for embedded applications

## Project Structure

| Path                                            | Purpose                                                                                                                             |
|-------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------|
| _build                                          | The [CMake build tree](https://cmake.org/cmake/help/latest/manual/cmake.1.html#introduction-to-cmake-buildsystems), can be deleted. |
| cmake                                           | Generated [CMake](https://cmake.org/) files. May be deleted if user.cmake has not been added                                        |
| .vscode                                         | See [VSCode](https://code.visualstudio.com/docs/getstarted/settings)                                                                |
| .vscode\settings.json                           | Workspace specific settings                                                                                                         |
| .vscode\mgs_qs_9x75_hybrid_rpi_7inch.mplab.json | The MPLAB project file, should not be deleted                                                                                       |
| out                                             | Final build artifacts                                                                                                               |
