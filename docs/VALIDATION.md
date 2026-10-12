# Validation record

This file separates archived evidence from tests that still need to be repeated after repository cleanup.

## Available evidence

- [x] Application and peripheral-driver source recovered
- [x] STM32 startup and linker files recovered
- [x] STM32CubeIDE `.project` and `.cproject` recovered
- [x] Proteus project recovered
- [x] Exported Proteus schematic recovered and rendered for the README
- [x] Archived simulation HEX available

## Re-verification checklist

- [ ] Import successfully into a clean STM32CubeIDE workspace
- [ ] Build with zero compiler errors
- [ ] Open the Proteus project without missing components
- [ ] Link the freshly generated HEX file
- [ ] Verify idle, low-speed, and high-speed transitions
- [ ] Verify all four IR input combinations
- [ ] Capture a screenshot or short recording of the simulation
- [ ] Confirm the UART frame in a virtual terminal

Until these checks are completed, the repository should be described as a recovered simulation archive rather than a newly reproduced result.

