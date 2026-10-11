# Architecture

## Layer Overview

The firmware is divided into three strict layers. Upper layers call lower layers only — no reverse dependencies.

```
Application  →  HAL/Services  →  MCAL  →  Hardware registers
```

### Application Layer

| File | Role |
|---|---|
| `main.c` | Entry point. Initializes system, runs the state machine loop. |
| `SInit_Program.c` | Orchestrates peripheral init in correct order (clocks first, then peripherals). |
| `SModes_Program.c` | Implements the three driving states. Each call returns the next state. |
| `APP_GUI.c` | Packages speed + distance into a UART frame and sends it each cycle. |

### HAL / Service Layer

Abstracts physical components behind clean function interfaces. No register access here.

| Module | Purpose |
|---|---|
| `HCSR04` | Triggers ultrasonic pulse, measures echo duration via TIM2 input capture, converts to cm. |
| `IR` | Reads digital HIGH/LOW from two IR obstacle sensors mapped to GPIO pins. |
| `MotorDriver` | Wraps L298N: sets IN1/IN2/IN3/IN4 direction pins and adjusts PWM duty via TIM. |
| `Buzzer` | Single GPIO output, toggled on/off at application request. |
| `CLCD` | 4-bit mode 16×2 LCD — init, cursor positioning, string and integer output. |
| `UART` | Blocking transmit, async receive with callback, ISR handler. |
| `Timer_Delay` | ms/us blocking delays using hardware timer (not SysTick). |

### MCAL Layer

Direct register-level drivers for STM32F401CC peripherals.

| Driver | Peripheral | Key responsibility |
|---|---|---|
| `MCAL_RCC` | RCC | System clock init, AHB/APB peripheral clock enable |
| `MCAL_GPIO` | GPIOA–GPIOH | Pin mode, output type, speed, pull, read, write |
| `MCAL_STK` | SysTick | 1 ms system tick, blocking delay |
| `MCAL_TIM` | TIM1–TIM5 | PWM output, input capture, overflow ISR |
| `CORE_NVIC` | NVIC | Interrupt enable, priority set |

## Execution Flow

```
main()
 └── SInit_VoidINIT()
      ├── RCC clock + peripheral enables
      ├── SysTick init
      ├── Timer delay init
      ├── HC-SR04 init   (TIM2 input capture)
      ├── Motor driver init
      ├── IR sensor init
      ├── LCD init
      ├── Buzzer init
      └── UART init (GUI telemetry)

 └── while(1)
      └── switch(Current_State)
           ├── Idle_State
           │    ├── Stop motors, buzzer ON
           │    ├── Read distance → decide next state
           │    ├── Read IR → override if both black
           │    └── Send telemetry
           ├── MovingHighSpeed_State
           │    ├── Ramp speed up to 100
           │    ├── Read distance → downgrade or stop
           │    ├── Read IR → steer or stop
           │    └── Send telemetry
           └── MovingLowSpeed_State
                ├── Converge speed to 50
                ├── Read distance → upgrade or stop
                ├── Read IR → steer or stop
                └── Send telemetry
```

## Interrupt Sources

| ISR | Source | Handler |
|---|---|---|
| `TIM2_IRQHandler` | HC-SR04 echo input capture + overflow | `HCSR04_Program.c` |
| `USART1_IRQHandler` | UART async receive | `UART_Prog.c` |
