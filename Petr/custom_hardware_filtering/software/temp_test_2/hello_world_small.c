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

#include "altera_avalon_pio_regs.h"


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
    alt_32 x_filtered = 0;
    for (int i = 0; i < TAPS; i++) {
        alt_32 queue_index = ((prev_x_vals->start - i) + TAPS) % TAPS;
        x_filtered += fixed_coeffs[i] * prev_x_vals->elems[queue_index];
    }
    return x_filtered / 10000;
}

/* Write pattern to LEDs */
static inline void led_write(alt_u16 led_pattern) {
    /*  Only 10 least significant bits are used (each corresponds to an LED) */
    IOWR(LED_BASE, 0, (led_pattern & 0x3ff));
}

/* Convert accelerometer data to LED pattern - modified to extend to 10 bits
 * LED mapping for val=
 * b  1011    0b0010 0000 0000
 * c  1100    0b0001 0000 0000
 * d  1101    0b0000 1000 0000
 * e  1110    0b0000 0100 0000
 * f  1111    0b0000 0010 0000
 * Take 512, then right shift by (val - 11)
 *
 * 0  0000    0b0000 0001 0000
 * 1  0001    0b0000 0000 1000
 * 2  0010    0b0000 0000 0100
 * 3  0011    0b0000 0000 0010
 * 4  0100	  0b0000 0000 0001
 * Take 16, then right shift by val
 */
static inline void convert_read(alt_32 acc_read, int *level, alt_u16 *led) {
    acc_read += OFFSET;
    alt_u8 val = (acc_read >> 6) & 0x0f;
    *led = (val > 4) ? (512 >> (val - 11))
                     : (16 >> val);
    *level = (acc_read >> 1) & 0x1f;
}

void sys_timer_isr() {
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

    if (pwm < abs(level)) {
        if (level < 0) {
            led_write(led << 1);
        } else {
            led_write(led >> 1);
        }
    } else {
        led_write(led);
    }

    if (pwm > PWM_PERIOD) {
        pwm = 0;
    } else {
        pwm++;
    }
}

void timer_init(void *isr) {
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x0900);
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0000);
    alt_irq_register(TIMER_IRQ, 0, isr);
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);
}

int fir_x_read(){
	return IORD_ALTERA_AVALON_PIO_DATA(FIR_OUT_X_BASE);
}

void fir_x_write(int x_val){
	IOWR(FIR_IN_X_BASE, 0, x_val);
}




int main() {
	int temp = 2;
	alt_printf("init val is: %x\n", temp);
	fir_x_write(temp);
	int res = fir_x_read();
	alt_printf("value read is: %x\n", res);
	res = fir_x_read();
	alt_printf("value read is: %x\n", res);
	res = fir_x_read();
	alt_printf("value read is: %x\n", res);
	res = fir_x_read();
	alt_printf("value read is: %x\n", res);
	res = fir_x_read();
	alt_printf("value read is: %x\n", res);
	res = fir_x_read();
	alt_printf("value read is: %x\n", res);

	res = IORD_ALTERA_AVALON_PIO_DATA(FIR_IN_X_BASE);
	alt_printf("value read is: %x\n", res);
	res = IORD_ALTERA_AVALON_PIO_DATA(FIR_IN_X_BASE);
	alt_printf("value read is: %x\n", res);




    // alt_32 x_read;
    // alt_32 x_filtered;
    // queue_t prev_x_vals = {0};    // Buffer for the samples

    // alt_up_accelerometer_spi_dev *acc_dev;
    // acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
    // if (acc_dev == NULL) {
    //     return 1; // Accelerometer not found
    // }

    // timer_init(sys_timer_isr);

    // // Start timer
    // alt_timestamp_start();
    // alt_32 loop_timer = 0;
    // alt_32 loop_counter = 0;

    // while (1) {
    //     alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
    //     prev_x_vals.start = (prev_x_vals.start + 1) % TAPS;
    //     prev_x_vals.elems[prev_x_vals.start] = x_read;

    //     /* Include fixed-point FIR filter processing here */
    //     x_filtered = fixed_point_filter(&prev_x_vals);

    //     convert_read(x_filtered, &level, &led);

    //     loop_timer += alt_timestamp(); // Get system time after finishing the process
    //     /* Timer processing functions hidden outside timing window to improve accuracy */
    //     loop_counter++;
    //     if (!(loop_counter % 100)) {
    //         /* Print the number of ticks on the screen */
    //         printf("Average loop processing time is %d ticks\n", (int) (loop_timer/100));
    //         loop_counter = 0;
    //         loop_timer = 0;
    //     }
    //     alt_timestamp_start();  // Restart system timer
    // }

}
