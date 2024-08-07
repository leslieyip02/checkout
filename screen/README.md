# Screen

This module is responsible for displaying the PayNow QR code.

### TODO

- Implement SD card storage with the 3.5" TFT LCD Screen

### Hardware

- Arduino UNO
- [ILI9486 3.5" TFT LCD Screen](https://shopee.sg/product/56539845/18721075107?gsht=OmjaohBUCnFZW9tL&gad_source=1&gclid=CjwKCAjwp4m0BhBAEiwAsdc4aBF5uQzZYgldxb45fw23RTaleK9006bwnlYIzpuqdDeBRyexxf7DdBoCvJ0QAvD_BwE)

### Libraries

- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [MCUFRIEND_kbv](https://github.com/prenticedavid/MCUFRIEND_kbv)
- [QRCode](https://github.com/ricmoo/qrcode/)

Go into `MCUFRIEND_kbv.cpp` to comment out unnecessary `#define`s and `if` statements.