# Hardware Setup Guide

## Bill of Materials (BOM)

| Component | Part Number | Quantity | Purpose | Notes |
|-----------|-------------|----------|---------|-------|
| Microcontroller LaunchPad | TM4C123GH6PM | 1 | Main processor | Texas Instruments |
| Ultrasonic Sensor | HC-SR04 | 1 | Distance measurement | 5V operation |
| Temperature Sensor | LM35 or Internal ADC | 1 | Heat detection | Can use internal sensor |
| 7-Segment Display | Grove 4-Digit Display | 1 | Visual output | I2C interface |
| Passive Buzzer | Generic PWM buzzer | 1 | Audio alerts | 5V, 2-4 kHz optimal |
| Grove Base BoosterPack | (Optional) | 1 | Easy connections | Seeed Studio compatible |
| Jumper Wires | Various | 10-15 | Connections | Male-to-female preferred |
| USB Cable | Micro-B | 1 | Power & programming | Standard USB 2.0 |

**Estimated Total Cost**: $60-80 USD (LaunchPad typically $20-25)

---

## Pin Connections

### Tiva C LaunchPad Pinout Reference

```
                    TM4C123GH6PM LaunchPad
                    ┌─────────────────┐
                    │   [USB PORT]    │
     ┌──────────────┤                 ├──────────────┐
     │  GND    PB5  │                 │  PE0    GND  │
     │  PB0    PB4  │                 │  PE1    PD0  │
     │  PB1    PA5  │   J1       J2   │  PE2    PD1  │◄── Ultrasonic
     │  PE4    PA6  │                 │  PE3    PD2  │
     │  PE5    PA7  │                 │  PF1    PD3  │
     │  PB4    PF2  │                 │  PF2    PE5  │
     │  PA5    PF3  │                 │  PF3    3.3V │
     │  PA6    PC4  │                 │  PC4    GND  │
     │  PA7    PC5  │◄── Buzzer       │  PC5    RST  │
     │              ├─────────────────┤              │
     │  3.3V   PD6  │   J3       J4   │  PC6    PF0  │◄── SW2
     │  PD6    PD7  │                 │  PC7    PF4  │◄── SW1
     │  PD7    PF4  │                 │  PD6    PB3  │
     │  PF0    5V   │                 │  PD7    PB2  │◄── I2C SCL
     │  RST    GND  │                 │  PE1    PC7  │
     │              │                 │  PE2         │
     │   [LEDs]     │                 │  PE4         │
     │  R  B  G     │                 │              │
     └──────────────┴─────────────────┴──────────────┘
     PF1 PF2 PF3 ◄── Onboard RGB LED
```

### Detailed Connection Table

#### Ultrasonic Sensor (HC-SR04)
| HC-SR04 Pin | Tiva C Pin | Function | Notes |
|-------------|------------|----------|-------|
| VCC | 5V (J3) | Power | Can also use 3.3V |
| TRIG | PD1 (J2) | Trigger/Echo | Shared pin mode |
| ECHO | PD1 (J2) | Echo pulse | Timer WT2CCP1 |
| GND | GND | Ground | Any GND pin |

**Wiring Notes**:
- If using 5V operation, ensure echo voltage is safe for 3.3V GPIO
- Consider adding 1kΩ + 2kΩ voltage divider on echo line for 5V sensors

#### Temperature Sensor (LM35)

**Option 1: External LM35**
| LM35 Pin | Tiva C Pin | Function |
|----------|------------|----------|
| VCC | 3.3V | Power |
| VOUT | PE3 (ADC0) | Analog output |
| GND | GND | Ground |

**Option 2: Internal Temperature Sensor**
- No external connections required
- Configured via ADC_CTL_TS in software
- Less accurate but convenient for testing

#### 7-Segment Display (I2C)
| Display Pin | Tiva C Pin | Function |
|-------------|------------|----------|
| VCC | 5V or 3.3V | Power |
| GND | GND | Ground |
| SDA | PB3 (J4) | I2C Data |
| SCL | PB2 (J4) | I2C Clock |

#### Buzzer (Passive)
| Buzzer Pin | Tiva C Pin | Function |
|------------|------------|----------|
| + (Signal) | PC5 (J1/J2) | PWM output |
| - (GND) | GND | Ground |

**Note**: Active buzzers won't work correctly - must use passive piezo buzzer

#### Push Buttons (Onboard)
| Button | Tiva C Pin | Function |
|--------|------------|----------|
| SW1 | PF4 | Mode: Distance |
| SW2 | PF0 | Mode: Temperature |

**Note**: Buttons are onboard - no external connections needed

#### RGB LED (Onboard)
| LED Color | Tiva C Pin | Notes |
|-----------|------------|-------|
| Red | PF1 | PWM capable |
| Blue | PF2 | PWM capable |
| Green | PF3 | PWM capable |

---

## Assembly Instructions

### Step 1: Install LaunchPad Headers (if needed)
If your LaunchPad came without headers soldered:
1. Insert 40-pin headers into J1-J4
2. Solder headers from bottom of board
3. Trim excess pin length if necessary

### Step 2: Connect Ultrasonic Sensor
1. Connect sensor VCC to Tiva 5V (or 3.3V for 3.3V sensors)
2. Connect sensor GND to any Tiva GND
3. Connect sensor TRIG and ECHO both to PD1
   - Some HC-SR04 modules have separate TRIG/ECHO - bridge them together
4. If using 5V sensor, add voltage divider on echo line:
   ```
   Echo ──┬── 1kΩ ──┬── PD1
          │         │
         GND      2kΩ
   ```

### Step 3: Connect Temperature Sensor (LM35)
1. Identify LM35 pins (flat side facing you, left to right: VCC, VOUT, GND)
2. Connect VCC to 3.3V
3. Connect VOUT to PE3
4. Connect GND to GND
5. *Optional*: Add 0.1µF capacitor between VOUT and GND for stability

**Breadboard Layout**:
```
LM35        Breadboard      Tiva C
 │                          ┌────┐
 ├─[VCC]────────────────────┤3.3V│
 ├─[VOUT]───────────────────┤PE3 │
 └─[GND]────────────────────┤GND │
                            └────┘
```

### Step 4: Connect 7-Segment Display
1. Connect display VCC to 5V (check display voltage rating)
2. Connect display GND to GND
3. Connect display SDA to PB3
4. Connect display SCL to PB2
5. *Note*: Some I2C displays have built-in pull-up resistors
   - If communication fails, add external 4.7kΩ pull-ups on SDA/SCL

### Step 5: Connect Buzzer
1. Connect buzzer positive (+) terminal to PC5
2. Connect buzzer negative (-) terminal to GND
3. *Optional*: Add 100Ω series resistor to limit current

**Circuit**:
```
PC5 ──┬── 100Ω ──┬── Buzzer (+)
      │          │
     GND ────────┴── Buzzer (-)
```

### Step 6: Power Connection
1. Connect USB cable to LaunchPad
2. Verify power LED illuminates
3. Ensure all connections are secure

---

## Hardware Testing

### Pre-Flight Checklist
- [ ] All VCC connections to correct voltage rails
- [ ] All GND connections properly made
- [ ] No short circuits between VCC and GND
- [ ] Sensor orientations correct
- [ ] Buzzer polarity correct (if polarized)
- [ ] USB cable securely connected

### Component Testing

#### Test 1: LED Functionality
```c
// Upload simple LED blink program
while(1) {
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // Red ON
    delay(500);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // Red OFF
    delay(500);
}
```

#### Test 2: Ultrasonic Sensor
1. Upload ranger test code
2. Place object 6-12 inches from sensor
3. Verify pulse width readings in terminal
4. Expected: Pulse width ~1000-2000 µs for 6-12 inches

#### Test 3: Temperature Sensor
1. Upload ADC test code
2. Read ADC values at room temperature
3. Expected: ~500-700 ADC counts for LM35 at 72°F
4. Apply heat source (hand) and verify increase

#### Test 4: Display
1. Upload 7-segment test code showing "8888"
2. Verify all segments illuminate
3. Test colon functionality

#### Test 5: Buzzer
1. Upload PWM tone generator
2. Should hear clear tone (not just click)
3. Test multiple frequencies (261Hz - 494Hz)

---

## Troubleshooting Hardware Issues

### No Power
- ✓ Check USB cable and port
- ✓ Try different USB port
- ✓ Verify power LED on LaunchPad

### Ultrasonic Sensor Not Working
- ✓ Verify 5V/3.3V compatibility
- ✓ Check TRIG/ECHO pin connections
- ✓ Ensure sensor faces open space (not wall)
- ✓ Measure echo pulse with oscilloscope

### Temperature Reading Zero or Constant
- ✓ Check LM35 pin orientation
- ✓ Verify PE3 ADC channel configuration
- ✓ Test with known-good sensor
- ✓ Check for solder bridges on LM35 pins

### Display Not Showing Data
- ✓ Verify I2C address (usually 0x70 or 0x71)
- ✓ Check SDA/SCL connections
- ✓ Add 4.7kΩ pull-up resistors if needed
- ✓ Verify display voltage rating

### No Buzzer Sound
- ✓ Verify passive (not active) buzzer type
- ✓ Check buzzer polarity
- ✓ Ensure PWM frequency in audible range (200Hz - 5kHz)
- ✓ Test with different frequencies

### Buttons Not Responding
- ✓ Buttons are active-low with pull-ups
- ✓ Check for solder issues on LaunchPad
- ✓ Verify GPIO port configuration in code

---

## Safety Considerations

### Electrical Safety
- ⚠️ Do not connect sensors to wrong voltage rails
- ⚠️ Verify polarity before powering on
- ⚠️ Keep liquids away from electronics
- ⚠️ Disconnect power when making wiring changes

### Component Protection
- Use current-limiting resistors where appropriate
- Add bypass capacitors for analog sensors
- Avoid hot-plugging sensors (power off first)
- Don't exceed 25mA per GPIO pin

### ESD Protection
- Touch grounded metal before handling LaunchPad
- Work on anti-static mat if available
- Store board in anti-static bag when not in use

---

## Photos and Diagrams

*Placeholder for hardware setup photos*

**Recommended photos to add**:
1. Complete assembled system
2. Close-up of sensor connections
3. Breadboard wiring diagram
4. Final enclosure (if applicable)

---

## Enclosure Design

### Optional: 3D Printed Case
- STL files for LaunchPad case: (to be added)
- Sensor mounting holes: 2mm diameter
- Display cutout: 40mm × 15mm
- Buzzer vent holes for sound output

### Mounting Suggestions
- Use M3 standoffs for LaunchPad
- Hot glue for sensor mounting (non-permanent)
- Velcro strips for breadboard attachment
- Cable management with zip ties

---

## Revision History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2024-11 | Initial hardware documentation |
