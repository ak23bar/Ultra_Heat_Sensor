# Contributing to Ultra Heat Sensor

Thank you for your interest in contributing to this project! While this was originally a solo academic project, contributions are welcome to enhance functionality, improve documentation, or add new features.

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue with:
- Clear description of the problem
- Steps to reproduce
- Expected vs actual behavior
- Hardware setup (if relevant)
- Serial terminal output or error messages
- Code Composer Studio version

### Suggesting Enhancements

Enhancement suggestions are welcome! Please include:
- Clear description of the feature
- Use case/motivation
- Potential implementation approach
- Impact on existing functionality

### Pull Requests

1. **Fork the repository**
2. **Create a feature branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Make your changes** following the coding standards below
4. **Test thoroughly** on actual hardware
5. **Commit with clear messages**:
   ```bash
   git commit -m "feat: add wireless temperature reporting via UART"
   ```
6. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```
7. **Open a Pull Request** with:
   - Description of changes
   - Testing performed
   - Any breaking changes
   - Screenshots/videos if applicable

## Coding Standards

### C Code Style

```c
// Use descriptive variable names
uint32_t distance_in_inches;  // Good
uint32_t d;                    // Bad

// Constants in UPPERCASE
#define MAX_TEMPERATURE_F 150
#define BUZZER_HIGH_VOLUME 40

// Functions: CamelCase for peripheral init, snake_case for application logic
void AdcInit(void);           // Peripheral initialization
void process_sensor_data();   // Application logic

// Comments for complex logic
/**
 * Calculate distance from ultrasonic pulse width
 * Formula: distance = (pulse_width * 12) / 89657
 * where 89657 = (50MHz / speed_of_sound) * 2 (round trip)
 */
```

### File Organization

- **Headers (.h)**: Function prototypes, type definitions, constants
- **Source (.c)**: Implementation
- **Assembly (.asm)**: Time-critical or hardware-specific routines only
- One module per peripheral (e.g., `ranger.c`, `temp_sensor.c`)

### Documentation

- Add comments for non-obvious code
- Update README.md for feature additions
- Document new hardware connections in `docs/HARDWARE.md`
- Include function headers with parameters and return values:

```c
/**
 * Trigger ultrasonic sensor reading
 * 
 * @param event Pointer to event structure for rescheduling
 * @return void
 * 
 * @note Schedules next reading 500ms in the future
 */
void TriggerRangerReading(Event *event);
```

## Testing Requirements

### Before Submitting PR

- [ ] Code compiles without warnings
- [ ] Tested on actual TM4C123GH6PM hardware
- [ ] No regressions in existing functionality
- [ ] Serial output verified
- [ ] All modes tested (distance and temperature)
- [ ] Memory usage within limits (check .map file)

### Hardware Test Checklist

- [ ] Power-on self-test passes
- [ ] Sensor readings accurate
- [ ] Display shows correct values
- [ ] LED colors correct for each state
- [ ] Buzzer frequencies/volumes correct
- [ ] Button presses responsive
- [ ] No unexpected resets or hangs

## Project Structure

```
src/
├── core/          # Main application and startup code
├── sensors/       # Sensor drivers (ultrasonic, temperature)
├── display/       # 7-segment display drivers
└── audio/         # Buzzer and PWM audio

docs/              # Documentation
hardware/          # Schematics, datasheets
```

When adding new modules:
1. Create appropriate subdirectory
2. Add header to `src/<module>/<module>.h`
3. Add implementation to `src/<module>/<module>.c`
4. Update project build configuration

## Areas for Contribution

### High Priority
- [ ] Wireless connectivity (ESP8266/ESP32 UART bridge)
- [ ] Data logging to SD card
- [ ] Mobile app interface (BLE or WiFi)
- [ ] Unit test framework
- [ ] Power optimization (sleep modes)

### Medium Priority
- [ ] OLED display upgrade
- [ ] Multiple sensor support
- [ ] Web dashboard
- [ ] Configuration via serial commands
- [ ] Alarm history logging

### Documentation
- [ ] Video tutorial
- [ ] Fritzing wiring diagram
- [ ] 3D printed enclosure STL files
- [ ] Performance benchmarking data

## Development Setup

See `docs/BUILD.md` for complete setup instructions.

Quick start:
```bash
git clone https://github.com/YOUR_USERNAME/Ultra_Heat_Sensor.git
cd Ultra_Heat_Sensor/Project

# Open in Code Composer Studio
# File → Import → Existing CCS Project
```

## Communication

- **Issues**: Use GitHub Issues for bugs and feature requests
- **Discussions**: Use GitHub Discussions for questions
- **Email**: [YOUR_EMAIL] for sensitive matters

## Code of Conduct

- Be respectful and constructive
- Focus on technical merit
- Help others learn
- Give credit where due

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for helping improve this project!**
