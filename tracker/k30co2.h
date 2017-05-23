// This is very helpful:
// http://www.raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart

// NOTE: THIS IS COMPLETELY UNTESTED. USE AT YOUR OWN RISK.
// I DON'T EVEN KNOW IF IT WILL COMPILE.

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

int k30_filestream = -1

#define K30_PORT "/dev/ttyS0"
#define K30_BAUD_RATE_MODE B9600
#define K30_TIMEOUT 0.5
#define K30_OPT O_RDWR | O_NOCTTY | O_NDELAY
#define K30_RX_LENGTH 255

// This is used to connect the CO2 sensor
int connect_k30(void);
void request_co2_reading();
int get_co2_reading();
void close_k30();
