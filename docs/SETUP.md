# Setup and reproduction guide

## Current verification level

The repository contains recovered STM32CubeIDE metadata, source code, a linker script, startup assembly, and a Proteus project. The archive has not yet been rebuilt on a clean machine after the 2026 repository maintenance.

## Requirements

- STM32CubeIDE with STM32F4 toolchain support
- Proteus 8.x to open the simulation
- A serial terminal if adapting the project to hardware

## Import the firmware

1. Open STM32CubeIDE.
2. Choose **File > Import > General > Existing Projects into Workspace**.
3. Select `firmware/stm32_f401/`.
4. Confirm that the project named `Final` is detected.
5. Build the Debug configuration.
6. Record the exact IDE version and any compiler diagnostics in `VALIDATION.md`.

The repository includes `.project` and `.cproject`. Generated `Debug/` and `Release/` directories should not be committed.

## Open the simulation

1. Open `simulation/proteus/adas_simulation.pdsprj` in Proteus.
2. Confirm that the configured MCU binary resolves correctly.
3. If the archived path is missing, rebuild the firmware and select the generated HEX file.
4. Start the simulation and exercise the ultrasonic-distance and IR inputs.

Expected state logic:

| Input | Expected state |
| --- | --- |
| distance greater than 20 cm | high-speed state |
| distance from 10 cm to 20 cm | low-speed state |
| distance below 10 cm | stopped/idle state |
| both IR sensors on the stop condition | stopped/idle state |

## UART telemetry

The firmware initializes USART1 on PA9/PA10 and sends an ASCII frame containing speed and distance. The current frame format is:

```text
<speed> <distance>\r\n
```

The archived configuration uses 9600 baud. Confirm data bits, parity, and stop bits from the UART driver before connecting external hardware.

## Hardware warning

This repository documents a simulation project. Pin mappings, voltage levels, motor power, sensor timing, and emergency-stop behavior must be reviewed before any physical build. Do not treat the implementation as an automotive or safety controller.
