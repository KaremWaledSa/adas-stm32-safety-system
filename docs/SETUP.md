# Setup Guide

## Requirements

| Tool | Version | Link |
|---|---|---|
| STM32CubeIDE | 1.13 or later | https://www.st.com/en/development-tools/stm32cubeide.html |
| ST-Link V2 driver | Latest | Bundled with CubeIDE |
| Proteus | 8.x | https://www.labcenter.com/ (simulation only) |
| Serial terminal | Any | RealTerm / PuTTY / CoolTerm |

## Build from Source

```
1. Launch STM32CubeIDE
2. File → Import → General → Existing Projects into Workspace
3. Browse to:  firmware/stm32_f401/
4. Select the project and click Finish
5. Project → Build All   (Ctrl+B)
6. Output HEX/ELF appears in:  firmware/stm32_f401/Debug/
```

## Flash to Hardware

```
1. Connect ST-Link V2 to the STM32F401CC Black Pill:
   ST-Link  →  Black Pill
   SWDIO    →  DIO
   SWCLK    →  CLK
   GND      →  GND
   3.3V     →  3.3V

2. In STM32CubeIDE: Run → Debug Configuration → ST-Link
3. Or flash directly:  STM32_Programmer_CLI -c port=SWD -d Debug/stm32_f401.hex
```

## Run the Proteus Simulation

```
1. Open:  simulation/proteus/NTI project.pdsprj
2. The HEX file (InfraredSensorsTEP.HEX) is already configured
3. Press the Play button to start simulation
4. Use the HC-SR04 distance slider to trigger state transitions:
   - > 20 cm  →  High-speed mode
   - 10–20 cm →  Low-speed mode
   - < 10 cm  →  Idle / stop
```

## UART Telemetry

The firmware transmits speed and distance every state cycle on **USART1**.

| Setting | Value |
|---|---|
| Baud rate | 9600 |
| Data bits | 8 |
| Stop bits | 1 |
| Parity | None |

Connect a USB-to-UART adapter to the STM32 TX pin and open any serial terminal.

## Tuning Parameters

All thresholds are in `firmware/stm32_f401/Inc/SMode_Config.h`:

```c
#define HighDistance   20   // cm — transition to high-speed above this
#define LowDistance    10   // cm — emergency stop below this
#define HighSpeed     100   // PWM duty (0–100)
#define LowSpeed       50   // PWM duty (0–100)
```

Change these values and rebuild to tune behavior without touching logic code.
