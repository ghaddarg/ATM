#ifndef ATM_USB_H
#define ATM_USB_H

/*
Dev (bus 20, device 7): 0001 - 7778
  Manufacturer:              Generic
  Product:                   Mass Storage
  Serial Number:             B30C3D2566732E3BC0FF - 256 - 20
Dev (bus 20, device 2): 05AC - 8290
  Manufacturer:              Broadcom Corp.
  Product:                   Bluetooth USB Host Controller
Dev (bus 20, device 0): 05AC - 8007
*/

//http://libusb.sourceforge.net/api-1.0/structlibusb__device__descriptor.html
//https://github.com/libusb/libusb/wiki

#include <stdint.h>
#include <stdbool.h>

#define MAX_BUFF_LEN			256

/********************************************************************************/
/*                               PUBLIC FUNCTIONS                               */
/********************************************************************************/
bool is_card_inserted( void );
size_t get_account_info( char * buf );
bool is_usb_removed( void );

#endif /* ATM_USB_H */