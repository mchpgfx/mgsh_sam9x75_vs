# SAM9x75 MGS Application Examples for VS Code with MPLAB Extensions

Welcome to the SAM9x75 MGS Application Examples repository! This collection provides sample projects and code snippets for developing applications on the Microchip SAM9x75 microcontroller using Visual Studio Code with MPLAB extensions.

---

**Repository:**  
[mgsh_sam9x75_vs (dev branch)](https://github.com/mchpgfx/mgsh_sam9x75_vs/tree/dev "mgsh_sam9x75_vs GitHub")

---

## Project Types

This repository contains two types of quickstart projects, each designed for different use cases:

### MGS Quickstart (`mgs_quickstart/`)

Full-featured graphics applications using the **MGS Harmony Legato graphics library**. These projects are ideal for:

- **Rapid UI development** - Design screens visually using MGS Composer
- **Widget-based interfaces** - Buttons, labels, images, progress bars, and more
- **Event-driven programming** - Built-in touch and input event handling
- **Production applications** - Complete graphics stack with hardware acceleration

The Legato library provides a high-level API for creating professional user interfaces with minimal code.

### Blank Quickstart (`blank_quickstart/`)

**Headless graphics applications** that provide direct framebuffer and touch access without a graphics library. These projects are ideal for:

- **Custom rendering engines** - Integrate third-party libraries (LVGL, emWin, TouchGFX)
- **Minimal footprint** - When a full graphics stack is unnecessary
- **Learning and prototyping** - Understand low-level display and touch driver interfaces
- **Performance-critical applications** - Direct pixel manipulation without library overhead

Each blank_quickstart project includes documentation on interfacing with the XLCDC display driver and System Input Service.

---

## Features

- Example projects for SAM9x75 MGS applications
- Step-by-step instructions for setup and build
- Integration with VS Code and MPLAB extensions
- Modular code structure for easy customization
- Support for multiple display types and sizes

### Available Projects

| Project | Display | Type |
|---------|---------|------|
| `mgs_qs_9x75_curiosity_ac69t88a` | AC69T88A 5" WVGA (800x480) | MGS Quickstart |
| `mgs_qs_9x75_curiosity_ws_2_8inch` | Waveshare 2.8" | MGS Quickstart |
| `mgs_qs_9x75_curiosity_ws_4in_round` | Waveshare 4" Round | MGS Quickstart |
| `mgs_qs_9x75_curiosity_ws_7_9inch` | Waveshare 7.9" | MGS Quickstart |
| `mgs_qs_9x75_curiosity_nvdi_10_1inch` | NVDI 10.1" | MGS Quickstart |
| `mgs_qs_9x75_rdk` | RDK Display | MGS Quickstart |
| `blank_qs_9x75_curiosity_ac69t88a_rgb565` | AC69T88A 5" WVGA (800x480) RGB565 | Blank Quickstart |
| `blank_qs_9x75_curiosity_ac69t88a` | AC69T88A 5" WVGA (800x480) RGBA8888 | Blank Quickstart |


---

## Getting Started

### 1. Prerequisites

- Visual Studio Code (latest version)
- MPLAB Extensions for VS Code
- Microchip SAM9x75 development board

### 2. Installation

- Clone this repository:
  
```bash
git clone https://github.com/mchpgfx/mgsh_sam9x75_vs.git
```

- Switch to the `dev` branch:
```bash
git checkout dev
```

- Open the folder in VS Code.
- Install MPLAB extensions if not already installed.

### 3. Building and Running Examples

- Select the desired example project from the repository.
- Follow the instructions in the project’s README or documentation folder.
- Use the MPLAB extension commands to build and flash the application to your SAM9x75 board.

---

## Documentation & Developer Help

For detailed guides, troubleshooting, and API references, visit the  
[Microchip Developer Help Page for SAM9x75 MGS Dev Kits](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/dev-kits/sam9x75-ug/ "SAM9x75 MGS Dev Kits Help").

---

## License

See the LICENSE file for details.

---

## Contact

For questions or support, open an issue in this repository.
