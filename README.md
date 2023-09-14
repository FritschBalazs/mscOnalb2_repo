# CMSIS-DAP compatible HighSpeed USB debugger

This repo contains all the softwares related to my MsC "Önlab 2" course, and some experiments for the countinuation "Diplomaterv 1"

main branch: currently contains a working demo with CMSIS-DAP v1 runing in HID mode.
st_composite branch: is currently the same, just as a composite device with added CDC class for a serial port.
st_custom_class: development branch for creating a custom class to implement CMSIS-DAP v2, with custom bulk endpoints.

This is just proof of concept repo, so it's a bit messy.

## F746G-DIOSCO pinout

-  D2 - PG6 - RESET
-  D4 - PG7 - SWDIO
-  D5 - PI0 - SWCLK

### Demo board (f412 disco) pinout
-  PA14 - STLINK jumper 2 - SWCLK
-  PA13 - STLINK jumper 4 - SWDIO
