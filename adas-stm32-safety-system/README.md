# ADAS STM32 Safety System

> Bare-metal Advanced Driver Assistance prototype on STM32F401CC — real-time obstacle detection, autonomous speed control, lane correction, and live UART telemetry, built entirely without HAL middleware.

![Platform](https://img.shields.io/badge/Platform-STM32F401CC-blue)
![Language](https://img.shields.io/badge/Language-C%20%28Bare--metal%29-lightgrey)
![IDE](https://img.shields.io/badge/IDE-STM32CubeIDE-orange)
![Simulation](https://img.shields.io/badge/Simulation-Proteus-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

---

## Overview

This project implements a scaled-down ADAS vehicle prototype on an STM32F401CC microcontroller. The system continuously reads an HC-SR04 ultrasonic sensor and two IR sensors, then transitions autonomously between three driving states — idle (stopped), low-speed, and high-speed — while correcting lane drift and transmitting live telemetry over UART.

All peripheral drivers (GPIO, RCC, SysTick, TIM, NVIC, UART) are written from scratch against the STM32F4 register map. No STM32 HAL or LL middleware is used.

---

## Features

| Feature | Status | Implementation |
|---|---|---|
| Obstacle detection | ✅ Implemented | HC-SR04 via Timer Input Capture (interrupt-driven) |
| Adaptive speed control | ✅ Implemented | 3-state machine, PWM duty cycle via TIM |
| Emergency braking | ✅ Implemented | Motor stop when distance < 10 cm |
| Lane departure detection | ✅ Implemented | Dual IR sensors, left/right reading |
| Automatic lane correction | ✅ Implemented | Motor direction adjusted per IR output |
| Real-time LCD display | ✅ Implemented | Mode, distance, speed updated each cycle |
| UART telemetry | ✅ Implemented | Speed + distance streamed to PC/GUI |
| Buzzer alerts | ✅ Implemented | Active on lane departure and idle state |

---

## Hardware

| Component | Part | Role |
|---|---|---|
| Microcontroller | STM32F401CC (Black Pill) | Main controller |
| Ultrasonic sensor | HC-SR04 | Front obstacle distance measurement |
| IR sensors | Generic 2-channel IR | Left / right lane edge detection |
| Motor driver | L298N dual H-bridge | Speed + direction control for 2 DC motors |
| Display | 16×2 Character LCD | Real-time mode, speed, distance readout |
| Alert | Active buzzer | Audible warning on lane departure / stop |

---

## System Architecture

```
┌─────────────────────────────────────────────────────┐
│                  Application Layer                   │
│  main.c  ──►  SInit  ──►  SModes (State Machine)    │
│                              │                       │
│              ┌───────────────┼──────────────┐        │
│           Idle_State   LowSpeed_State  HighSpeed_State│
└──────────────────────────────┼──────────────────────┘
                               │
┌──────────────────────────────▼──────────────────────┐
│                    HAL / Service Layer                │
│   HCSR04 │ IR │ MotorDriver │ Buzzer │ CLCD │ UART  │
└──────────────────────────────┬──────────────────────┘
                               │
┌──────────────────────────────▼──────────────────────┐
│                      MCAL Layer                       │
│         RCC │ GPIO │ SysTick │ TIM │ NVIC            │
└─────────────────────────────────────────────────────┘
```

---

## State Machine

```
               ┌──────────┐
   distance    │          │  distance
   > 20 cm     │   IDLE   │◄──────────  < 10 cm (any state)
   ────────────►  STOP    │
               │  BUZZER  │
               └────┬─────┘
                    │ distance > 20 cm
                    ▼
          ┌─────────────────┐
          │   HIGH SPEED    │◄──── distance > 20 cm
          │   speed = 100   │
          │   IR correction │
          └────────┬────────┘
                   │ 10 cm ≤ distance ≤ 20 cm
                   ▼
          ┌─────────────────┐
          │   LOW SPEED     │
          │   speed = 50    │
          │   IR correction │
          └─────────────────┘
```

**Distance thresholds** (configurable in `SMode_Config.h`):

| Macro | Value | Meaning |
|---|---|---|
| `HighDistance` | 20 cm | Transition to high-speed above this |
| `LowDistance` | 10 cm | Emergency stop below this |
| `HighSpeed` | 100 (PWM) | Full duty cycle |
| `LowSpeed` | 50 (PWM) | Half duty cycle |

**IR lane correction** (same logic in all moving states):

| Left IR | Right IR | Action |
|---|---|---|
| White | White | Move forward |
| Black | White | Turn right (left edge detected) |
| White | Black | Turn left (right edge detected) |
| Black | Black | Stop (both edges — abort) |

---

## Repository Structure

```
adas-stm32-safety-system/
├── firmware/
│   └── stm32_f401/
│       ├── Src/               # All .c implementation files
│       ├── Inc/               # All .h headers
│       ├── Startup/           # startup_stm32f401ccfx.s
│       ├── STM32F401CCFX_FLASH.ld  # Linker script
│       ├── .project           # STM32CubeIDE project file
│       └── .cproject          # CDT build settings
├── simulation/
│   └── proteus/
│       ├── NTI project.pdsprj # Proteus schematic + simulation
│       ├── NTI project.PDF    # Exported schematic PDF
│       └── InfraredSensorsTEP.HEX  # Pre-built HEX for simulation
├── docs/
│   ├── ARCHITECTURE.md        # Detailed layer breakdown
│   └── SETUP.md               # Build and flash instructions
├── .gitignore
├── LICENSE
└── README.md
```

---

## Getting Started

### Prerequisites

- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) (tested with 1.13+)
- ST-Link V2 programmer
- [Proteus](https://www.labcenter.com/) 8.x (for simulation only)
- Any serial terminal (RealTerm, PuTTY) at **9600 baud** for UART telemetry

### Build & Flash

```bash
# 1. Open STM32CubeIDE
# 2. File → Import → Existing Projects into Workspace
# 3. Select:  firmware/stm32_f401/
# 4. Project → Build All
# 5. Run → Debug (ST-Link) or flash the .hex directly
```

### Run Simulation

```
1. Open:  simulation/proteus/NTI project.pdsprj
2. The pre-built HEX is already linked — press Play
3. Adjust the ultrasonic distance slider to trigger state transitions
```

---

## Module Reference

| Module | Files | Description |
|---|---|---|
| State machine | `SModes_Program.c`, `SModes_Interface.h` | Idle / LowSpeed / HighSpeed logic |
| System init | `SInit_Program.c`, `SInit_Interface.h` | Peripheral initialization sequence |
| Ultrasonic | `HCSR04_Program.c`, `HCSR04_Config.c` | Timer IC-based distance measurement |
| IR sensors | `IR_Program.c` | Left/right digital lane reading |
| Motor driver | `MotorDriver_Program.c` | L298N PWM speed + direction |
| LCD | `CLCD_program.c` | 16×2 character display |
| UART | `UART_Prog.c` | Telemetry transmit / async receive |
| Buzzer | `Buzzer_Program.c` | Alert control |
| RCC | `MCAL_RCC_Program.c` | Clock configuration |
| GPIO | `MCAL_GPIO_Program.c` | Pin mode, output, input |
| SysTick | `MCAL_STK_Program.c` | 1 ms tick / delay |
| Timer | `MCAL_TIM_Program.c` | PWM output + input capture |
| NVIC | `CORE_NVIC_Program.c` | Interrupt priority and enable |

---

## License

MIT — see [LICENSE](LICENSE).
