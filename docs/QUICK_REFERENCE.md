# Ultra Heat Sensor - Quick Reference

## System States at a Glance

### Distance Mode (Press SW1)

| Range | Display Format | LED Color | Buzzer |
|-------|---------------|-----------|--------|
| ≥ 5 ft | `XX:YY` feet:inches | 🟢 Green | Silent |
| 2-5 ft | `XX:YY` feet:inches | 🟡 Yellow | Intermittent Low |
| < 2 ft | `XX:YY` feet:inches | 🔴 Red | Continuous High |

### Temperature Mode (Press SW2)

| Condition | Display Format | LED Color | Buzzer |
|-----------|---------------|-----------|--------|
| < 90°F | `XXX.Y` degrees | 🟢 Green | Silent |
| ≥ 90°F | `XXX.Y` degrees | 🔴 Red | Continuous High |

## Pin Quick Reference

```
PD1  - Ultrasonic Sensor (Echo/Trigger)
PE3  - Temperature Sensor (ADC)
PC5  - Buzzer (PWM)
PB2  - I2C SCL (Display)
PB3  - I2C SDA (Display)
PF0  - SW2 Button (Temp Mode)
PF4  - SW1 Button (Distance Mode)
PF1  - Red LED
PF2  - Blue LED
PF3  - Green LED
```

## Key Functions

```c
// Main Application (LabProj.c)
void main(void)
void TriggerRangerReading(Event *event)
void TriggerTemperatureReading(Event *event)
void ProcessTemperatureReading(Event *event)
void CheckPushButton(Event *event)
void ToggleBuzzer(Event *event)

// Ranger Module (ranger.c)
void RangerInit(void)
void RangerTriggerReading(void)
uint32_t RangerGetData(void)
void RangerEventRegister(Event *event)

// Temperature Sensor (temp_sensor.c)
void TsInit(void)
void TsTriggerReading(void)
float TsDataRead(void)
void TsEventRegister(Event *event)

// Display (seg7digit.c)
void Seg7Init(void)
void Seg7Update(Seg7Display *display)

// Buzzer (buzzer.c, pwm_buzzer.c, music_buzzer.c)
void BuzzerInit(void)
void BuzzerOn(void)
void BuzzerOff(void)
void MusicInitBuzzer(void)
void MusicSetBuzzer(int pitch, int volume)
void MusicTurnOffBuzzer(void)
```

## Timing Parameters

```c
Distance Reading:     Every 500 ms
Temperature Reading:  Every 4000 ms (4 seconds)
Button Check:         Continuous (interrupt-driven)
Buzzer Toggle:        Every 500 ms (in yellow zone)
Display Update:       Every sensor reading
```

## Memory Map (Key Peripherals)

```
GPIO_PORTF_BASE:  0x40025000  (LEDs, Buttons)
GPIO_PORTD_BASE:  0x40007000  (Ultrasonic)
GPIO_PORTC_BASE:  0x40006000  (Buzzer)
GPIO_PORTB_BASE:  0x40005000  (I2C)
ADC0_BASE:        0x40038000  (Temperature)
WTIMER0_BASE:     0x40036000  (Buzzer PWM)
WTIMER2_BASE:     0x4004C000  (Ultrasonic Timing)
```

## Serial Terminal Commands

**Baud Rate**: 115200, 8N1, No flow control

**Sample Output**:
```
Ranger reading: 45231 
Temperature Reading Sensor: 72.3 
Ranger reading: 89657 
Temperature Reading Sensor: 72.4
```

## Calculations

### Distance Calculation
```c
pulse_width = falling_edge_time - rising_edge_time;
distance_inches = (pulse_width * 12) / 89657;
feet = distance_inches / 12;
inches = distance_inches % 12;
```

### Temperature Calculation
```c
// For external LM35
voltage = ADC_raw * 3.3 / 4096;
temp_C = (voltage - 0.5) / 0.01;
temp_F = temp_C * 1.8 + 32;

// For internal sensor (different formula)
temp_C = 147.5 - ((75.0 * 3.3 * ADC_raw) / 4096.0);
temp_F = temp_C * 1.8 + 32;
```

### PWM Calculations
```c
// For musical note frequency
period = 50000000 / frequency_Hz;

// For volume (duty cycle)
duty_cycle = period * (volume_percent / 100);
```

## Musical Notes (C4-B4 Scale)

| Note | Frequency | Period (50MHz) | Pitch Index |
|------|-----------|----------------|-------------|
| C4 | 261.63 Hz | 191110 | 0 |
| D4 | 293.66 Hz | 170265 | 1 |
| E4 | 329.63 Hz | 151685 | 2 |
| F4 | 349.23 Hz | 143172 | 3 |
| G4 | 392.00 Hz | 127551 | 4 |
| A4 | 440.00 Hz | 113636 | 5 |
| B4 | 493.88 Hz | 101239 | 6 |

## Volume Levels

| Level | Percentage | Duty Cycle |
|-------|------------|------------|
| Low | 10% | 0.1 × period |
| Medium | 20% | 0.2 × period |
| High | 40% | 0.4 × period |

## Event System

```c
// Event structure
typedef struct {
    void (*callback)(Event *event);
    uint32_t time;
} Event;

// Event lifecycle
EventInit(&event, callback_function);        // Initialize
EventSchedule(&event, time_ms);              // Schedule
EventExecute();                              // Execute (in main loop)
```

## Common Debug Commands

```bash
# View memory usage
arm-none-eabi-size Debug/Ultra_Heat_Sensor.out

# Disassemble specific function
arm-none-eabi-objdump -d Debug/Ultra_Heat_Sensor.out | grep TriggerRangerReading -A 20

# Check symbol table
arm-none-eabi-nm Debug/Ultra_Heat_Sensor.out | grep RangerInit

# View sections
arm-none-eabi-objdump -h Debug/Ultra_Heat_Sensor.out
```

## Troubleshooting Checklist

### No sensor readings
- [ ] Check power connections (VCC, GND)
- [ ] Verify pin assignments in code
- [ ] Confirm peripheral initialization
- [ ] Enable UART debug output

### Incorrect values
- [ ] Verify calculation formulas
- [ ] Check ADC reference voltage
- [ ] Calibrate against known values
- [ ] Review timer clock configuration

### System hangs
- [ ] Check for infinite loops
- [ ] Verify interrupt priorities
- [ ] Ensure WFI is used correctly
- [ ] Monitor stack usage

### Build errors
- [ ] Set TivaWare path correctly
- [ ] Check all source files included
- [ ] Verify include paths
- [ ] Clean and rebuild project

## Performance Benchmarks

| Metric | Typical Value |
|--------|---------------|
| Boot time | ~100 ms |
| First sensor reading | ~1 second |
| ISR execution time | <50 μs |
| Event dispatch time | <10 μs |
| Display refresh rate | 60 Hz |
| Power consumption (idle) | ~30 mA |
| Power consumption (active) | ~200 mA |

## File Size Reference

```
LabProj.c:         ~300 lines
ranger.c:          ~170 lines
temp_sensor.c:     ~95 lines
buzzer.c:          ~25 lines
seg7digit.c:       ~55 lines

Total code size:   ~12-15 KB
Total RAM usage:   ~2-3 KB
Stack size:        1 KB
```

## Git Workflow

```bash
# Clone
git clone https://github.com/YOUR_USERNAME/Ultra_Heat_Sensor.git

# Create feature branch
git checkout -b feature/new-sensor

# Commit changes
git add .
git commit -m "feat: add humidity sensor support"

# Push
git push origin feature/new-sensor

# Create PR on GitHub
```

## Resources

- [TM4C123 Datasheet](https://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf)
- [TivaWare API](https://www.ti.com/tool/SW-TM4C)
- [ARM Cortex-M4 Guide](https://developer.arm.com/documentation/dui0553/latest)
- [Project Repository](https://github.com/YOUR_USERNAME/Ultra_Heat_Sensor)

---

**Version**: 1.0  
**Last Updated**: November 2024
