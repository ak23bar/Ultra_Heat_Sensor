# Building and Deploying the Ultra Heat Sensor System

This guide walks you through setting up your development environment, building the project, and deploying it to the Tiva C LaunchPad.

---

## Prerequisites

### Required Software

1. **Code Composer Studio (CCS)**
   - Version: 12.0 or later recommended
   - Download: https://www.ti.com/tool/CCSTUDIO
   - Platforms: Windows, Linux, macOS
   - License: Free (no license key required)

2. **TivaWare™ for C Series**
   - Version: 2.2.0 or later
   - Download: https://www.ti.com/tool/SW-TM4C
   - Required for peripheral drivers and startup code
   - Installation path (default): `C:/ti/TivaWare_C_Series-2.2.0.295/`

3. **USB Drivers**
   - ICDI (In-Circuit Debug Interface) drivers
   - Usually installed automatically with CCS
   - Manual download: https://www.ti.com/tool/STELLARIS_ICDI_DRIVERS

### Required Hardware

- TM4C123GH6PM LaunchPad
- Micro-USB cable
- See `docs/HARDWARE.md` for complete BOM

---

## Installation Steps

### Step 1: Install Code Composer Studio

**Windows**:
```powershell
# Download installer from TI website
# Run ccs_setup_<version>.exe
# Follow installation wizard
# Select "Tiva C Series" in device support options
```

**Linux**:
```bash
# Download installer
chmod +x ccs_setup_<version>.run
sudo ./ccs_setup_<version>.run

# Add udev rules for LaunchPad
sudo cp ~/ti/ccs1200/ccs/install_scripts/99-stellaris-launchpad.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
```

**macOS**:
```bash
# Download .dmg installer
# Mount and run installer
# Grant permissions in System Preferences → Security
```

### Step 2: Install TivaWare

1. Download TivaWare installer from TI website
2. Run installer (accept default installation path)
3. Note installation directory - you'll need this path later
4. Typical paths:
   - Windows: `C:/ti/TivaWare_C_Series-2.2.0.295/`
   - Linux: `~/ti/TivaWare_C_Series-2.2.0.295/`
   - macOS: `/Applications/ti/TivaWare_C_Series-2.2.0.295/`

### Step 3: Verify LaunchPad Connection

1. Connect LaunchPad to PC via USB
2. Power LED should illuminate
3. **Windows**: Device Manager should show "Stellaris ICDI" under Ports
4. **Linux**: Run `lsusb` - should see Texas Instruments device
5. **macOS**: System Information → USB should list LaunchPad

---

## Importing the Project

### Method 1: Import Existing CCS Project

1. Open Code Composer Studio
2. **File → Import → Code Composer Studio → CCS Projects**
3. Click **Browse** and navigate to project directory
4. Select `Ultra_Heat_Sensor/Project`
5. Check "Copy projects into workspace" (recommended)
6. Click **Finish**

### Method 2: Clone from Git

```bash
# Clone repository
git clone https://github.com/YOUR_USERNAME/Ultra_Heat_Sensor.git
cd Ultra_Heat_Sensor/Project

# Open in CCS
# File → Import → General → Existing Projects into Workspace
# Select root directory: <path-to-repo>/Project
```

---

## Configuring Build Settings

### Step 1: Set TivaWare Path

1. Right-click project in **Project Explorer**
2. **Properties → Build → ARM Compiler → Include Options**
3. Click **Add** and browse to TivaWare installation
4. Add the following include paths:
   ```
   ${TIVAWARE_INSTALL_DIR}
   ${TIVAWARE_INSTALL_DIR}/driverlib
   ${TIVAWARE_INSTALL_DIR}/inc
   ${TIVAWARE_INSTALL_DIR}/utils
   ```
5. **Properties → Build → ARM Linker → File Search Path**
6. Add library path:
   ```
   ${TIVAWARE_INSTALL_DIR}/driverlib/ccs/Debug/driverlib.lib
   ```

### Step 2: Set Compiler Options

**ARM Compiler → Optimization**:
- Optimization level: **Off (for debugging)** or **-O2 (for release)**
- Enable optimization for size: **Checked**

**ARM Compiler → Advanced Options → Language Options**:
- C dialect: **C99**
- Allow C++ style comments: **Checked**

### Step 3: Configure Debugger

1. **Run → Debug Configurations**
2. Double-click **Code Composer Studio - Device Debugging**
3. **Target Configuration**: Select `Tiva TM4C123GH6PM.ccxml`
4. **Program/Memory**: Point to `Debug/Ultra_Heat_Sensor.out`
5. Click **Apply** and **Close**

---

## Building the Project

### Command Line Build (Optional)

```bash
cd /home/neo/Ultra_Heat_Sensor/Project

# Clean project
make clean

# Build project
make all

# Check for build artifacts
ls -lh Debug/*.out
```

### GUI Build

1. Select project in **Project Explorer**
2. **Project → Build Project** (or press Ctrl+B / Cmd+B)
3. Monitor **Console** window for build output
4. Successful build shows:
   ```
   Finished building target: Ultra_Heat_Sensor.out
   ```
5. Build artifacts are in `Debug/` directory:
   - `Ultra_Heat_Sensor.out` - ELF executable
   - `Ultra_Heat_Sensor.map` - Memory map
   - `*.obj` - Object files

### Troubleshooting Build Issues

#### Error: "cannot find driverlib.lib"
```bash
# Solution: Rebuild TivaWare driverlib
cd ${TIVAWARE_INSTALL_DIR}/driverlib
make

# Or specify correct path in linker settings
```

#### Error: "undefined reference to 'LaunchPadInit'"
```bash
# Solution: Add missing source files to project
# Right-click src/ → Add Files to Project
# Ensure all .c files are included
```

#### Error: "unknown type name 'uint32_t'"
```c
// Solution: Add stdint.h include to file
#include <stdint.h>
#include <stdbool.h>
```

---

## Flashing the LaunchPad

### Method 1: Debug Mode (Recommended for Development)

1. **Run → Debug** (or press F11)
2. CCS will:
   - Flash the program to LaunchPad
   - Reset the device
   - Halt at `main()`
3. **Run → Resume** (or press F8) to start execution
4. Use **Terminate** button to stop debugging

### Method 2: Run Without Debugging

1. **Run → Run** (or press Ctrl+F11)
2. Program flashes and runs immediately
3. To reflash, click **Terminate** then **Run** again

### Method 3: Standalone Flashing (LM Flash Programmer)

```bash
# Download LM Flash Programmer from TI
# Command line:
LMFlash.exe -f Debug/Ultra_Heat_Sensor.out -v

# GUI: Load .out file and click "Program"
```

---

## Serial Terminal Setup

### Viewing Debug Output

The system outputs debug information via UART. To view:

1. **Connect**: LaunchPad USB provides virtual COM port
2. **Find COM Port**:
   - Windows: Device Manager → Ports (COM & LPT)
   - Linux: `ls /dev/ttyACM*`
   - macOS: `ls /dev/tty.usbmodem*`

3. **Configure Terminal**:
   - Baud rate: **115200**
   - Data bits: **8**
   - Parity: **None**
   - Stop bits: **1**
   - Flow control: **None**

### Terminal Software Options

**Windows**:
- PuTTY: https://www.putty.org/
- Tera Term: https://ttssh2.osdn.jp/
- CCS built-in terminal

**Linux/macOS**:
```bash
# Using screen
screen /dev/ttyACM0 115200

# Using minicom
minicom -D /dev/ttyACM0 -b 115200

# Exit screen: Ctrl+A, then K
```

**CCS Built-in Terminal**:
1. **View → Terminal**
2. Click **Open a Terminal**
3. Select **Serial Terminal**
4. Choose COM port and set baud to 115200

### Expected Output

```
Ranger reading: 45231
Temperature Reading Sensor: 72.3
Ranger reading: 89657
Temperature Reading Sensor: 72.4
```

---

## Testing the Deployed System

### Basic Functionality Tests

1. **Power-On Test**:
   - RGB LED should light up (color depends on sensor state)
   - 7-segment display shows `00:00` or temperature
   - No errors in serial terminal

2. **Distance Mode Test**:
   - Press **SW1** button
   - Wave hand in front of ultrasonic sensor
   - Display should show changing feet:inches values
   - LED changes color based on distance
   - Buzzer activates when distance < 5 feet

3. **Temperature Mode Test**:
   - Press **SW2** button
   - Display shows current temperature (e.g., `072.3`)
   - Place finger on temp sensor
   - Temperature should increase
   - LED turns red if temp ≥ 90°F

4. **Alert System Test**:
   - In distance mode, move object < 2 feet away
   - LED should turn RED
   - Buzzer sounds continuously at high volume
   - Move to 2-5 feet: LED turns YELLOW, buzzer intermittent

### Advanced Testing

**Accuracy Calibration**:
```bash
# Measure known distances with ruler
# Compare to displayed values
# Calculate error percentage
error_percent = |measured - displayed| / measured × 100%
```

**Performance Monitoring**:
```c
// Add timing code to measure ISR execution time
uint32_t start_time = SysTickValueGet();
// ... ISR code ...
uint32_t end_time = SysTickValueGet();
uint32_t execution_time = start_time - end_time;
```

---

## Debugging Tips

### Using CCS Debugger

**Breakpoints**:
- Click left margin in editor to set breakpoint
- F5: Step Into
- F6: Step Over
- F7: Step Return
- F8: Resume

**Watch Expressions**:
1. **View → Expressions**
2. Add variables: `seg7.digit[0]`, `sys.display_mode`, etc.
3. Values update in real-time during debugging

**Memory Browser**:
1. **View → Memory Browser**
2. Enter peripheral address (e.g., `0x40025000` for GPIO Port F)
3. Watch register values change

**Halt Debugging**:
- **Run → Suspend** (pause execution)
- **View → Registers** to inspect CPU state

### Common Issues

#### LaunchPad Not Detected
```bash
# Windows: Reinstall ICDI drivers
# Linux: Check udev rules
sudo usermod -a -G dialout $USER
# Logout and login again

# macOS: Reset SMC
# Shutdown, disconnect USB, wait 30s, reconnect
```

#### Program Runs Then Crashes
```c
// Check stack size in linker script
// Increase if needed:
__STACK_SIZE = 0x00000400; // Increase from default
```

#### Sensor Data Incorrect
```c
// Add debug output in ISR
uprintf("ADC Raw: %u\n", ADC_raw_value);

// Verify peripheral initialization
assert(ADCIntStatus(ADC0_BASE, 0, false));
```

---

## Performance Optimization

### Release Build Configuration

1. **Project → Properties → Build → ARM Compiler**
2. Set Optimization: **-O2** or **-O3**
3. Enable: **Optimize for speed**
4. **Advanced → Inline functions**: **Automatic**

### Code Size Reduction

```bash
# Check section sizes
arm-none-eabi-size Debug/Ultra_Heat_Sensor.out

# Output:
   text    data     bss     dec     hex filename
  12345     120    4567   17032    428a Ultra_Heat_Sensor.out
```

Optimize:
- Remove unused printf() calls (saves ~4KB)
- Use `--opt_for_speed=0` for size optimization
- Enable linker garbage collection

---

## Continuous Integration (Optional)

### GitHub Actions Example

```yaml
name: Build Tiva C Project

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Install ARM GCC
        run: sudo apt-get install gcc-arm-none-eabi
      - name: Build Project
        run: make -C Project/
```

---

## Additional Resources

- [TM4C123GH6PM Datasheet](https://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf)
- [Tiva C Launchpad User Guide](https://www.ti.com/lit/ug/spmu296/spmu296.pdf)
- [TivaWare API Reference](https://www.ti.com/tool/SW-TM4C)
- [CCS User Guide](https://software-dl.ti.com/ccs/esd/documents/users_guide/index.html)

---

## Support

If you encounter issues:
1. Check `docs/HARDWARE.md` for wiring verification
2. Review error messages in CCS Console
3. Enable verbose build output: **Project → Properties → Build → ARM Compiler → Diagnostics**
4. Post issues on GitHub repository

---

**Last Updated**: November 2024
