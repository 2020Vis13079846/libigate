// Based on libusb 1.0 and Chronic Dev Team work

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libusb-1.0/libusb.h>
#include <usb.h>

/*
BootROM -> LLB -> iBoot -> SEP -> Kernel -> Applications
*/

#define VENDOR_ID       (int)0x05AC
#define NORM_MODE       (int)0x1290 // Normal Mode
#define RECV_MODE       (int)0x1281 // Recovery Mode
#define WTF_MODE        (int)0x1227 // What The System (WTF) Mode
#define DFU_MODE        (int)0x1222 // DFU Mode
#define BUF_SIZE        (int)0x10000

struct libusb_device_handle *device = NULL; /* initalize device variable with NULL value */

int send_command(char* command) 
{
    size_t length = strlen(command);

    if (length >= 0x200) {
	return 0;
    }

    /* now check succeed or not */
	
    if (! libusb_control_transfer(device, 0x40, 0, 0, 0, command, (length + 1), 1000)) {
	printf("Failed to execute command!\n")
    }

    return 1
}

void connect() 
{
    if ((device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, RECV_MODE)) == NULL) {
	if ((device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, WTF_MODE)) == NULL) {
	    if ((device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, DFU_MODE)) == NULL) {
		__asm__("nop"); // do nothing
	    }
	}
    }
}

void disconnect() 
{
    if (device != NULL) {
	libusb_close(device);
    }
}

void reset() 
{
    if (device != NULL) {
	libusb_reset_device(device);
    }
}

void init() 
{
    libusb_init(NULL);
}
