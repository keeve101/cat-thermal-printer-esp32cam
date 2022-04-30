# cat_thermal_printer_esp32cam

Side project from Term 2 DTI, utilizing ESP32 cam, MIT App Inventor and 'Cat Thermal Printer'<br> 
Using libraries - JPEGDEC, Thermal_Printer by Larry Banks; Github bitbank2, esp_camera by Espressif; Github espressif

Tested using ESP32-CAM and 'Cat' series BLE Thermal Printer<br><br>
To directly decode image from image captured to frame buffer to 1-bpp dithered image sent to thermal printer. Included functions such as message printing from BLE App to Thermal Printer and images embedded in FLASH memory.

Camera framesize is tested at QVGA, potentially able to go up to CIF. Unsure if it is possible to configure custom frame sizes, but it is probably possible to resize image from frame buffer (see bilinear interpolation algorithm https://rosettacode.org/wiki/Bilinear_interpolation) to fit larger frame sizes such as VGA to resize to 384x??? to fit the Thermal Printer's resolution (cat thermal printer resolution is 203 dpi (384), very well possible to use other supported BLE Thermal Printers included in Larry Bank's Thermal Printer library that may fit up to 576x???).
