# BMP180 temp example

I wasn't happy with the Adafruit BMP180 sensor library due to the flash space it took up. I wrote this in order to see if I can make a more lightweight implementation and also teach myself how to read from the sensor's registers directly, rather than rely on existing libraries. Without UART debug messages, it only takes up around 2.5KB of flash on an AVR.
