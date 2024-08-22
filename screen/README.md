# Screen

This module is responsible for displaying the PayNow QR code.

### Overview

The code in `record.cpp` is responsible for recording new scans. The prices are stored on a SD card in a file called `p.csv`. This SD card can be inserted into the TFT LCD screen.

Memory constraints posed a significant challenge when developing this module. While highly inefficient, each scan re-reads the `.csv` file in order to save memory.

##### QR Code

The PayNow QR code is based on a formatted string, and the implementation borrows from [this repo](https://github.com/poonchuanan/Python-PayNow-QR-Code-Generator).

### Hardware

- Arduino UNO
- [ILI9486 3.5" TFT LCD Screen](https://shopee.sg/product/56539845/18721075107?gsht=OmjaohBUCnFZW9tL&gad_source=1&gclid=CjwKCAjwp4m0BhBAEiwAsdc4aBF5uQzZYgldxb45fw23RTaleK9006bwnlYIzpuqdDeBRyexxf7DdBoCvJ0QAvD_BwE)

### Libraries

- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [MCUFRIEND_kbv](https://github.com/prenticedavid/MCUFRIEND_kbv)
- [QRCode](https://github.com/ricmoo/qrcode/)

Go into `MCUFRIEND_kbv.cpp` to comment out unnecessary `#define`s and `if` statements in order to save memory.