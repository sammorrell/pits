#include "k30co2.h"

int connect_k30(void)
{
    int status = 0;

    k30_filestream = open( K30_PORT, K30_OPT );

    if ( k30_filestream == -1 )
    {
        status = k30_filestream;
        printf("Error: Unable to open connection to CO2 sensor on %s. \n", K30_PORT);
    } else {
        // We have a connection. Let's configure it.
        struct termios options;
        tcgetattr(k30_filestream, &options);
        options.c_cflag = K30_BAUD_RATE_MODE | CS8 | CLOCAL | CREAD;		// Set baud rate
        options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
        tcflush(k30_filestream, TCIFLUSH);
        tcsetattr(k30_filestream, TCSANOW, &options);
        printf("Connected to CO2 sensor on %s \n", K30_PORT);
        sleep( 1000 ); // Sleep for 1 second
    }

    return status;
}

// Sends the TX bytes required to read out the CO2 reading.
void request_co2_reading()
{
    unsigned char tx_buff[7];
    unsigned char* p_tx_buff = NULL;
    int count;

    p_tx_buff = &tx_buff[0];
    fprintf( tx_buff, "\xFE\x44\x00\x08\x02\x9F\x25" );

    if ( k30_filestream != -1 )
    {
        tcflush(k30_filestream, TCIFLUSH); // Flush the stream
        count = write( k30_filestream, tx_buff, 7 );

        if ( count < 0 )
        {
            printf( "Error writing CO2 sensor request to TX.\n" );
        }
    }
}

// Returns an integer containing the CO2 reading.
int get_co2_reading()
{
    int co2 = 0;
    unsigned char rx_buff[K30_RX_LENGTH + 1];
    int rx_length;
    float high, low;

    request_co2_reading();
    sleep(500);

    // Now read the bytes from the sensor
    rx_length = read( k30_filestream, (void*) rx_buff, K30_RX_LENGTH );
    rx_buff[rx_length] = "\0"; // Terminate buffer with a null char

    if ( rx_length == 0 )
    {
        // No bytes read
    } else {
        low = (int) rx_buff[3];
        high = (int) rx_buff[4];
        co2 = ( high * 256 ) + low;
    }

    return co2;
}
