# Bouncing Balls Skirt

Für Seeed Studio XIAO nRF52840

## Udev Rules

/etc/udev/rules.d/99-platformio-udev.rules

```
# Seeed Studio
ATTRS{idVendor}=="2886", ATTRS{idProduct}=="[08]045", MODE:="0666", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"
```

## Benötigte Libraries

- Seeed Studio XIAO nRF52840 Board
- Adafruit Neopixel Strip (via Library Manager installieren)
- https://github.com/Seeed-Studio/Seeed_Arduino_LSM6DS3 (ZIP downloaden und in Arduino IDE als Library hinzufügen)
- https://github.com/ArminiusM/Neopixel (ZIP runterladen und in ~/Arduino/libraries/ Ordner legen)
