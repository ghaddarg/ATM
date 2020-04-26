/* 
 * Libusb file
 */

#include <stdio.h>
#include <string.h>

#include "inc/libusb/libusb.h"
#include "inc/atm_usb.h"

static libusb_device ** devices;
static unsigned char account_info_buf[ MAX_BUFF_LEN ];
static uint8_t usb_idx;
/********************************************************************************/
/*                               PRIVATE FUNCTIONS                              */
/********************************************************************************/
static int atm_libusb_init( void )
{
	int ret;
	ssize_t count;

	ret = libusb_init( NULL );
	if (ret < 0) {

		printf("Failed to initialize libusb\n");
		goto out;
	}

	//XXX: TODO: Make sure only 1 USB at a time
	count = libusb_get_device_list( NULL, &devices );
	if (count <= 0) {

		printf("No attached USB devices found\n");
		ret = (int)count;
	}

out:
	return ret;
}

static void atm_libusb_fini( void )
{
	usb_idx = 0;
	memset( account_info_buf, MAX_BUFF_LEN, 0 );

	libusb_free_device_list( devices, 1 );
	libusb_exit( NULL );
}
/*
 * This function will also rule out internal USB(s)
 * Internal USB device apparently do not have serial number
 */
static bool is_valid_card( libusb_device * dev )
{
	struct libusb_device_descriptor desc;
	libusb_device_handle * handle = NULL;
	int r;
	bool ret = false;

	r = libusb_get_device_descriptor( dev, &desc );
	if ( r < 0 ) {
		printf("failed to get device descriptor with error %d\n", r);
		goto out;
	}

	r = libusb_open( dev, &handle );
	if ( LIBUSB_SUCCESS != r ) {
		printf("failed to open usb device with error %d\n", r);
		goto out;
	}

	if ( desc.iSerialNumber ) {

		r = libusb_get_string_descriptor_ascii( handle, desc.iSerialNumber, account_info_buf, MAX_BUFF_LEN );
		if ( r <=0 ) {
			printf("failed get usb device serial number with error %d\n", r);
			goto out;
		}

		ret = true;
		printf("Found USB/account #: %s\n", ( char * )account_info_buf );
	}

out:
	if ( handle )
		libusb_close( handle );

	return ret;
}
/********************************************************************************/
/*                               PUBLIC FUNCTIONS                               */
/********************************************************************************/
bool is_card_inserted( void )
{
	bool card_inserted = false;

	if ( atm_libusb_init() < 0 )
		goto out;

	for ( usb_idx = 0; devices[ usb_idx ]; usb_idx++ ) {

		/* Find first valid USB device and break */
		card_inserted = is_valid_card( devices[ usb_idx ] );
		if ( card_inserted )
			break;
	}

	//atm_libusb_fini();

out:	
	return card_inserted;
}

size_t get_account_info( char * buf )
{
	if ( NULL == buf )
		return -1;

	size_t size = strlen( ( char * )account_info_buf );

	memcpy( buf, account_info_buf, size );

	return size;
}

bool is_usb_removed( void )
{
	int r;
	bool ret = false;
	libusb_device_handle * handle = NULL;

	r = libusb_open( devices[ usb_idx ], &handle );
	if ( LIBUSB_SUCCESS != r ) {
		printf("failed to open usb device with error %d\n", r);
		ret = true;
		atm_libusb_fini();
	}

	if ( handle )
		libusb_close( handle );

	return ret;
}
