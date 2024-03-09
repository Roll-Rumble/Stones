#include "altera_up_avalon_accelerometer_spi.h"
#include "alt_types.h"
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_timestamp.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_timer_regs.h"


#include <stdio.h>

void read_from_fir(alt_32 *x_read, alt_32 *y_read){
	*x_read = IORD_ALTERA_AVALON_PIO_DATA(FIR_OUT_X_BASE)/10000;
	*y_read = IORD_ALTERA_AVALON_PIO_DATA(FIR_OUT_Y_BASE)/10000;
}

void write_to_fir(alt_32 x_sample, alt_32 y_sample){
	IOWR(FIR_IN_X_BASE,0, x_sample);
	IOWR(FIR_IN_Y_BASE,0, y_sample);
	IOWR(SAMPLE_TICK_BASE, 0, 0);
	IOWR(SAMPLE_TICK_BASE, 0, 1); //tick
	IOWR(SAMPLE_TICK_BASE, 0, 0);
}

void bound(alt_32 *read_val){
	*read_val = (*read_val > 255) ? 255 : *read_val;
	*read_val = (*read_val < -255) ? -255 : *read_val;
}

int main() {

    // Instantiate accelerometer object for taking readings
    alt_up_accelerometer_spi_dev *acc_dev;
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");


    alt_32 x_read, y_read; //raw read values
    alt_32 x_hardware_filtered, y_hardware_filtered; //values read from FIR
    alt_32 button_input;

    /* variables used for printing on UART */
    char x_msb, x_lsb;
	char y_msb, y_lsb;
	char buttons;
	char end_token_1 = -128;
	char end_token_2 = 0;

	alt_timestamp_start();
    while (1) {
    	if(alt_timestamp() > 49500){
    		alt_timestamp_start();
			/* read raw values from accelerometer and buttons */
			alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
			alt_up_accelerometer_spi_read_y_axis(acc_dev, &y_read);
			button_input = IORD_ALTERA_AVALON_PIO_DATA(BUTTON_BASE);

			write_to_fir(x_read, y_read); //send measured values to hardware fir
			read_from_fir(&x_hardware_filtered, &y_hardware_filtered); //fir processes values

			//adjusting values
			x_hardware_filtered += 7;
			y_hardware_filtered += -1;

			/* make sure both are within -255 to 255 */
			bound(&x_hardware_filtered);
			bound(&y_hardware_filtered);


			x_lsb = x_hardware_filtered & 0x000000FF;
			x_msb = (x_hardware_filtered >> 8) & 0x000000FF;
			y_lsb = y_hardware_filtered & 0x000000FF;
			y_msb = (y_hardware_filtered >> 8) & 0x000000FF;
			buttons = button_input & 0b11;

			printf("%c%c%c%c%c%c%c", x_msb, x_lsb, y_msb, y_lsb, buttons, end_token_1, end_token_2);
    	}
    }
}
