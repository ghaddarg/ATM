# ATM
This is a simple finite state machine ATM system that was designed. 
The design of the system is as close as possible to an actual ATM bank machine.
Instead of cards we are using usb to signify when a card has been entered.
I'm using libusb1.0 inorder to get access to connected USBs

## OS
This SW was designed on Mac OSX 10.13

## Dependecies
- libusb1.0: http://libusb.sourceforge.net/api-1.0/
- gtest: https://github.com/google/googletest

## Version:
This is version 1

## TODO:
 
1. Add file save/load for NVM <DONE>
2. Add Cmake
3. Add unit tests <DONE>
4. Maybe use usb as bank cards <DONE>
5. Change PIN ability <DONE>
6. Put LOCK into accuont that has been locked <DONE>
7. Update nvm values of pin and balance before returning card <DONE>
