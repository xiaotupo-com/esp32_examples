![alt text](image.jpg)
<h1 align="center">🏳️‍🌈 ESP32 WS2812 Control</h1>

<p align="center">
ws2812_control is a WS2812 LED control component designed for ESP32<br/>
Supports multiple LED effects and two working modes: matrix and strip<br/>
Provides rich API interfaces and configurable parameters
</p>

<p align="center">
English
· <a href="./README.md">简体中文</a>
· <a href="https://github.com/NingZiXi/ws2812_control/releases">Changelog</a>
· <a href="https://github.com/NingZiXi/ws2812_control/issues">Report Issues</a>
</p>

<p align="center">
  <a href="LICENSE">
    <img alt="License" src="https://img.shields.io/badge/License-MIT-blue.svg" />
  </a>
  <a href="https://www.espressif.com/">
    <img alt="ESP32" src="https://img.shields.io/badge/ESP32-ESP32S3-77216F?logo=espressif" />
  </a>
  <a href="https://docs.espressif.com/projects/esp-idf/">
    <img alt="ESP-IDF" src="https://img.shields.io/badge/ESP--IDF-v5.3+-orange.svg" />
  </a>
  <a href="https://www.espressif.com/">
    <img alt="Platform" src="https://img.shields.io/badge/Platform-ESP32-green.svg" />
  </a>
  <a href="">
    <img alt="Version" src="https://img.shields.io/badge/Version-v1.3.0-brightgreen.svg" />
  </a>
  <a href="https://github.com/NingZiXi/ws2812_control/stargazers">
    <img alt="GitHub Stars" src="https://img.shields.io/github/stars/NingZiXi/ws2812_control.svg?style=social&label=Stars" />
  </a>
</p>

---

## Overview

This project provides a library for controlling WS2812 LED strips, compatible with ESP32 series microcontrollers. The library includes various effects such as breathing, fading, blinking, and rainbow effects, as well as basic on/off functionality. Designed with modularity and easy integration in mind, it can be conveniently used in other projects.

## Features
- **Supports WS2812 Matrix**: In addition to regular WS2812 drivers, it also supports creating LED matrices of various arrangements and sizes.
- **HSV to RGB Conversion**: Provides a helper function for converting HSV color space to RGB color space.
- **Multiple LED Effects**: Supports always-on, off, breathing, fade-in, slow blink, fast blink, and rainbow effects.
- **Individual LED Control**: Can set the color of individual LEDs.
- **Color Macros**: Predefined common colors such as white, green, red, blue, etc.
- **Modular Design**: Clear code structure, easy to extend and maintain.

## Configuration

Configure WS2812 parameters in menuconfig. Navigate to the ws2812 config menu to set the signal pin and working mode (Strip mode or Matrix mode). Depending on the selected mode, you may need to configure additional parameters:

1. **Matrix Dimensions**: Set the number of LEDs per row and column in the matrix.
2. **Matrix Layout Type**: Specify the matrix layout type and direction to support different installation methods.

After configuration, save and exit `menuconfig` to control WS2812 LEDs according to the set mode and parameters. Below is an example using Strip mode.

## Strip Mode Usage
### 1. Create WS2812 Handle

First, create a WS2812 control handle for subsequent LED operations.

```c
ws2812_strip_t* WS2812=ws2812_create();
```

### 2. Set LED Effects

Use the `ws2812_set` function to set LED effects. For example, set all LEDs to always-on with red color:

```c
// Method 1
ws2812_set(WS2812, COLOR_RED, LED_EFFECT_ON);
// Method 2
led_set_on(WS2812,COLOR_RED);
// Or use COLOR_RGB(255,0,0) to set red or custom colors
led_set_on(WS2812,COLOR_RGB(255,0,0));
```

### 3. Common Functions

- **Set Individual LED Color**:

  ```c
  led_set_pixel(WS2812, 0, COLOR_GREEN); // Set LED 0 to green
  ```

- **Turn Off All LEDs**:

  ```c
  led_set_off(WS2812);
  ```

- **Breathing Effect**:

  ```c
  // Method 1
  ws2812_set(WS2812, COLOR_BLUE, LED_EFFECT_BREATH);
  // Method 2
  led_set_breath(strip, COLOR_RGB(255, 0, 0), 10); // Red breathing
  ```

- **Rainbow Effect**:

  ```c
  // Method 1
  ws2812_set(WS2812, COLOR_BLUE, LED_EFFECT_RAINBOW);
  // Method 2
  led_set_rainbow(strip, 20); // Rainbow effect with 20ms interval
  ```

#### Available Effects for `ws2812_set()`
>
    LED_EFFECT_ON             // Always-on effect
    LED_EFFECT_BREATH,        // Breathing effect
    LED_EFFECT_FADE_IN,       // Fade-in effect
    LED_EFFECT_BLINK_SLOW,    // Slow blink effect
    LED_EFFECT_BLINK_FAST,    // Fast blink effect
    LED_EFFECT_RAINBOW        // Rainbow effect

Note: `ws2812_set()` applies to all LEDs.

For more APIs, please refer to [ws2812_control.h](include\ws2812_control.h).

### Example Code

```c
#include <stdio.h>
#include <ws2812_control.h>

void app_main(void){
    // Create a WS2812 strip
    ws2812_strip_t* WS2812=ws2812_create();
    
    // Turn on strip with color (255,0,0)
    led_set_on(WS2812,COLOR_RGB(255,0,0));
}
```

## Matrix Mode Usage

Matrix mode usage is similar to Strip mode.

```c
// Create WS2812 matrix handle
ws2812_matrix_t* WS2812 = ws2812_matrix_create();

// Set LED at row 2, column 2 to red
led_matrix_set_pixel(WS2812,2,2,COLOR_RGB(255,0,0));

// Update matrix display
led_matrix_show(WS2812);
```

### Example Code

```c
#include <stdio.h>
#include <ws2812_control.h>

void app_main(void){
    // Create WS2812 matrix handle
    ws2812_matrix_t* WS2812 = ws2812_matrix_create();

    // Set LEDs to red
    led_matrix_set_pixel(WS2812,0,0,COLOR_RGB(255,0,0));
    led_matrix_set_pixel(WS2812,1,1,COLOR_RGB(255,0,0));
    led_matrix_set_pixel(WS2812,2,2,COLOR_RGB(255,0,0));
    led_matrix_set_pixel(WS2812,3,3,COLOR_RGB(255,0,0));

    // Update matrix display
    led_matrix_show(WS2812);
}
```
![alt text](59333df79fe9b88a5f6df7c52084de0.jpg)

## Dependencies
- ESP-IDF >= 4.4.7 (Tested with latest IDF 5.3.1)
- led_strip driver library

## License
This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Contributing
Contributions of any kind are welcome, including but not limited to code improvements, documentation updates, and issue reports. Please submit Pull Requests or Issues via GitHub.

## Author
- Ning ZiXi (1589326497@qq.com)

## Version History
- 1.0.0 (2024-08-31)
  - Initial release
- 1.1.0 (2024-10-31)
  - Added running light effect
- 1.2.0 (2024-11-26)
  - Added matrix mode
- 1.3.0 (2025-4-27)
  - Improved rainbow effect implementation
  - Animation effects now use timer

## 🤝 Contributing
This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

<p align="center">
Thank you for using ESP32 WS2812 Control! 🌈<br/>
If you find this project helpful, please give it a ⭐ Star!
</p>
