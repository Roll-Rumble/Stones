#include "altera_up_avalon_accelerometer_spi.h"
#include "alt_types.h"

#include <stdio.h>

int main() {
    // Instantiate accelerometer object for taking readings
    alt_up_accelerometer_spi_dev *acc_dev;
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");

    alt_32 x_read;
    alt_32 y_read;
    unsigned char buffer[6];

    // Set final two chars of each msg to -65536 (unique terminator)
    buffer[4] = 128;
    buffer[5] = 0;

    // Take readings and print result on UART
    while (1) {
        alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
        alt_up_accelerometer_spi_read_y_axis(acc_dev, &y_read);

        buffer[0] = (x_read >> 8) & 0xff;   // MSB of x
        buffer[1] = x_read & 0xff;          // LSB of x
        buffer[2] = (y_read >> 8) & 0xff;   // MSB of y
        buffer[3] = y_read & 0xff;          // LSB of y
        // buffer[5] and buffer[6] constant terminator values

        // Output accel vals as bytes
        printf("%c%c%c%c%c%c", buffer[0], buffer[1], buffer[2],
               buffer[3], buffer[4], buffer[5]);

        // Magic constant to slow transmission down for now
        // Custom PC implementation unable to keep up with max data
        // transfer rate (working on fix)
        for (int i = 0; i < 1000; i++) {
            // Waste some time!
            alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
        }
    }
}
