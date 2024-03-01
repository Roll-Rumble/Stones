#include "altera_up_avalon_accelerometer_spi.h"
#include "alt_types.h"

#include <stdio.h>


int main() {

    // Instantiate accelerometer object for taking readings
    alt_up_accelerometer_spi_dev *acc_dev;
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");

    // alt_timestamp_start();

    // alt_u32 accel_readings[3];     // [0] for x, [1] for y, [2] for z
    // char start_char = 0b10111111;   // Not possible value for two of these to be in a row
    // char *x0 = &accel_readings[0];
    // char *x1 = &accel_readings[0] + 1;
    // char *y0 = &accel_readings[1];
    // char *y1 = &accel_readings[1] + 1;
    // char *z0 = &accel_readings[2];
    // char *z1 = &accel_readings[2] + 1;

    // Take readings and print result on UART
    while (1) {
        alt_32 x_read, y_read, z_read;
        alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
        alt_up_accelerometer_spi_read_y_axis(acc_dev, &y_read);
        alt_up_accelerometer_spi_read_z_axis(acc_dev, &z_read);

        printf("X: %i, Y: %i, Z: %ie", x_read, y_read, z_read);

        // Magic constant to slow transmission down for now
        // Custom PC implementation unable to keep up with max data
        // transfer rate (working on fix)
        for (int i = 0; i < 1000; i++) {
            // Waste some time!
            alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
        }

        //printf("X: %ld, Y: %ld, Z: %ld\n", x_read, y_read, z_read);
        // printf("XYZ123");
    }

}
