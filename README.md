# cat-thermal-printer-esp32cam
## Introduction
Side project from Term 2, 3.007 Design Thinking and Innovation course.

## Project Overview
To directly decode image from image captured to frame buffer to 1-bpp dithered image sent to thermal printer. Included functions such as message printing from BLE App to Thermal Printer and images embedded in FLASH memory.

Camera framesize is tested at QVGA, potentially able to go up to CIF. Unsure if it is possible to configure custom frame sizes, but it is probably possible to resize the image from frame buffer (see [bilinear interpolation algorithm](https://rosettacode.org/wiki/Bilinear_interpolation)) to fit larger frame sizes such as VGA to resize to fit the thermal printer's resolution as the cat thermal printer resolution is 203 dpi (384), very well possible to use other supported thermal printers included in the [Thermal_Printer](https://github.com/bitbank2/Thermal_Printer) library that may fit higher resolutions.

## Tools used
- [ESP32-CAM Module](https://randomnerdtutorials.com/esp32-cam-video-streaming-face-recognition-arduino-ide/)
- ['Cat' series BLE Thermal Printer](https://hackspace.raspberrypi.com/articles/bluetooth-cat-thermal-printer-review)
- [MIT App Inventor](https://appinventor.mit.edu/)

## Resources/Libraries used
- [JPEGDEC](https://github.com/bitbank2/JPEGDEC)
- [Thermal_Printer](https://github.com/bitbank2/Thermal_Printer)
- [esp-camera](https://github.com/espressif/esp32-camera)