# MCXN947 Interactive Game Console

An interactive embedded game console featuring an OLED display with dynamic LED visualization, multiple input modes, and real-time sensor feedback. Developed as part of the **NXP MCX Embedded Programming** course.

## Features

This project implements **4 interactive operating modes**, each demonstrating different embedded systems concepts:

| Mode | Description |
|------|-------------|
| **Potentiometer Mode** | LED ring rotation speed controlled by analog input |
| **Temp/Light Mode** | Real-time environmental sensing with visual feedback |
| **Encoder Mode** | Precise rotary encoder control of LED position |
| **Joystick Mode** | Directional navigation with LED mapping |

### Key Highlights

- **OLED Menu System** - Clear visual interface for mode selection and status display
- **LED Ring Visualization** - 8-LED ring with configurable rotation patterns
- **Multiple Input Methods** - Supports buttons, encoders, joysticks, and analog inputs
- **Real-time Response** - Interrupt-driven architecture for responsive controls

## Project Structure

```
mcxn947-game-console/
├── src/
│   ├── main.c          # Main application and state machine
│   ├── led_control.c   # LED ring control module
│   ├── menu.c          # OLED menu system
│   └── sensors.c       # Sensor reading and processing
├── include/
│   ├── led_control.h   # LED control interface
│   ├── menu.h          # Menu system interface
│   └── sensors.h       # Sensor interface definitions
├── lib/
│   └── oled/           # OLED display driver (BSD-3-Clause, NXP)
│       ├── oled.c
│       └── oled.h
├── README.md
└── LICENSE
```

### Module Overview

| Module | Purpose |
|--------|---------|
| `main.c` | Application entry, state machine, interrupt handlers |
| `led_control` | LED array management, rotation logic |
| `menu` | OLED display management, UI rendering |
| `sensors` | ADC reading, DIP switch state management |
| `lib/oled` | Low-level OLED I2C driver *(third-party)* |

## State Diagram



## License & Credits

### Project Code
This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

### Third-Party Components

**OLED Display Driver** (`lib/oled/`)
- Copyright © NXP Semiconductors
- Licensed under **BSD-3-Clause License**
- Part of the NXP MCUXpresso SDK

> **Note**: The OLED library in `lib/oled/` is NOT authored by the project maintainer. It is included under the BSD-3-Clause license from NXP Semiconductors. See the license headers in those files for details.

## Course Project

This project was developed as part of the **NXP MCX Embedded Programming** course, demonstrating practical embedded systems concepts including:

- Interrupt-driven programming
- ADC and sensor integration
- I2C communication (OLED)
- GPIO management
- State machine design
- Timer/Counter usage
