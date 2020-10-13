// Based on libusb 1.0 and Chronic Dev Team work

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <libusb-1.0/libusb.h>
#include <usb.h>

#include <readline/readline.h>
#include <readline/history.h>

/*
BootROM -> LLB -> iBoot -> SEP -> Kernel -> Applications
*/

#define VENDOR_ID       (int)0x05AC
#define NORM_MODE       (int)0x1290 // Normal Mode
#define RECV_MODE       (int)0x1281 // Recovery Mode
#define WTF_MODE        (int)0x1227 // What The System (WTF) Mode
#define DFU_MODE        (int)0x1222 // DFU Mode
#define BUF_SIZE        (int)0x10000

#define CMD_LOG	".orbit_history" /* history file for readline/history.h */

struct libusb_device_handle *device = NULL; /* initalize device variable with NULL value */

void show_commands(bool is_global)
{
    if (is_global) printf("Usage: orbit [option] <argumants>\n\n  -s, --shell  Interact with iDevice.\n");
    else printf("  reboot  Reboot iDevice.\n  go <address>  Jump to the specified memory address.\n help  Show available commands.\n");
}

int send_command(char* argv[]) 
{
    char* command = argv[0];
    size_t length = strlen(command);

    if (length >= 0x200) {
	return false;
    }

    /* now check succeed or not */
	
    if (! libusb_control_transfer(device, 0x40, 0, 0, 0, command, (length + 1), 1000)) {
	return false;
    }

    // okk
    return true;
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

// DANGEROUS: Do not use on new iPhones (only iPhone Original - iPhone 3GS)

void reset() 
{
    if (device != NULL) {
	libusb_reset_device(device);
    }
}

int shell() 
{
    if (libusb_set_configuration(device, 1) < 0) {	
	return 101; // setting consiguration error
    }
	
    if (libusb_claim_interface(device, 1) < 0) {
	return 102; // interface error
    }
	
    if (libusb_set_interface_alt_setting(device, 1, 1) < 0) {
	return 103; // alt interface error
    }
	
    char* buffer = malloc(BUF_SIZE);
    if (buffer == NULL) {
	return 104; // memory allocation error
    }
	
    FILE* fd = NULL;
    for (;;) {	
	int bytes = 0;
	memset(buffer, 0, BUF_SIZE);
	libusb_bulk_transfer(device, 0x81, buffer, BUF_SIZE, &bytes, 500);
		
	if (bytes > 0) {
	    int i;
            for(i = 0; i < bytes; ++i) {
		fprintf(stdout, "%c", buffer[i]);
		if (fd) fprintf(fd, "%c", buffer[i]);
	    }
	}
	char *command = readline("(console)> ");
		
	if (command != NULL) {
	    add_history(command);
	    if (fd) fprintf(fd, ">%s\n", command);
	    send_command(&command);
				
	    char* action = strtok(strdup(command), " ");
	    if (! strcmp(action, "getenv")) {
	        char response[0x200];
		libusb_control_transfer(device, 0xC0, 0, 0, 0, response, 0x200, 1000);
		printf("%s\r\n", response);		
	    }
				
	    if (! strcmp(action, "reboot") || ! strcmp(action, "poweroff"))
		return 105; // terminated
	}	
        free(command);
    }
    free(buffer);
    if (fd) fclose(fd);
    libusb_release_interface(device, 1);
    return 100; // okk
}

void init() 
{
    libusb_init(NULL);
}

int main(int argc, char *argv[])
{
    if (argc >= 2) {
        if (! strcmp(argv[1], "-s") || ! strcmp(argv[1], "--shell")) {
            shell();
        } else show_commands(true);
    } else show_commands(true);
}
