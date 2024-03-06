#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_timer_regs.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "sys/alt_irq.h"
#include "sys/alt_timestamp.h"
#include "system.h"
#include <stdio.h>
#include <stdlib.h>


#define OFFSET -32
#define PWM_PERIOD 16

#define TAPS 20 // Can be adjusted based on filter
const alt_32 fixed_coeffs[TAPS] = {52, -529, 102, 264, -495,
                                   151, 657, -1146, 164, 5423,
                                   5423, 164, -1146, 657, 151,
                                   -495, 264, 102, -529, 52};

alt_8 pwm = 0;
alt_u16 led;
int level;

/* Fixed length queue of 32-bit integers */
typedef struct queue {
    alt_32 elems[TAPS];
    alt_32 start;  // Index of the most recent element in the queue
} queue_t;

/* N-tap low-pass filter using 4dp fixed-point arithmetic */
static inline alt_32 fixed_point_filter(queue_t *prev_x_vals) {
    alt_32 filtered = 0;
    for (int i = 0; i < TAPS; i++) {
        alt_32 queue_index = ((prev_x_vals->start - i) + TAPS) % TAPS;
        filtered += fixed_coeffs[i] * prev_x_vals->elems[queue_index];
    }
    return filtered / 10000;
}

void timer_init(void *isr) {
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x0900);
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0000);
    alt_irq_register(TIMER_IRQ, 0, isr);
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);
}

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

int main() {

    alt_32 x_read;
    alt_32 y_read;
    alt_32 x_filtered;
    alt_32 y_filtered;
    alt_32 x_hardware_filtered;
    alt_32 y_hardware_filtered;
    queue_t prev_x_vals = {0};    // Buffer for the samples
    queue_t prev_y_vals = {0};    // Buffer for the samples

    alt_up_accelerometer_spi_dev *acc_dev;
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
    if (acc_dev == NULL) {
        return 1; // Accelerometer not found
    }

    // Start timer
    alt_timestamp_start();
    alt_32 loop_timer = 0;
    alt_32 loop_counter = 0;

    while (1) {
        /* read accelerometer values */
        alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
        alt_up_accelerometer_spi_read_y_axis(acc_dev, &y_read);

        /* hardware test */

        write_to_fir(x_read, y_read); //send measured vals to hardware fir
        read_from_fir(&x_hardware_filtered, &y_hardware_filtered);
        printf("%x   %x\n", x_hardware_filtered, y_hardware_filtered);

        /* software test */

		prev_x_vals.start = (prev_x_vals.start + 1) % TAPS;
		prev_x_vals.elems[prev_x_vals.start] = x_read;
        x_filtered = fixed_point_filter(&prev_x_vals);

        prev_y_vals.start = (prev_y_vals.start + 1) % TAPS;
		prev_y_vals.elems[prev_y_vals.start] = y_read;
        y_filtered = fixed_point_filter(&prev_y_vals);

        printf("%x   %x\n", x_filtered, y_filtered);

		/* end of software test */

        printf("\n");



        /* Timer processing functions hidden outside timing window to improve accuracy */
        loop_counter++;
        if (!(loop_counter % 1000)) {
        	loop_timer = alt_timestamp(); // Get system time after finishing the process
            /* Print the number of ticks on the screen */
            printf("\n\nAverage loop processing time is %d ticks\n\n\n\n\n", (int) (loop_timer/1000));
            loop_counter = 0;
            loop_timer = 0;
        }
        alt_timestamp_start();  // Restart system timer
    }

}
