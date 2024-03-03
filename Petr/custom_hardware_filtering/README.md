# Lab 3: Integrating an Accelerometer with a NIOS System

## Introduction

The primary goal of lab 3 was to design a system capable of interfacing with the accelerometer on the DE10-lite board, understand the SPI interface, and apply digital signal processing to the acquired data.


## Task 1: Interface an accelerometer to a NIOS II system

Task 1 was to set up the DE10 board for the experiment, Using the NIOS II architecture and a serial peripheral interface to create the system.

### NIOS II

- NIOS II is a soft processor core developed by Intel for FPGA devices. 
- It is instantiated in the programable logic of an FPGA.
- This means it is ideal for prototyping and understanding embedded system design (like for this course). 
- It can also be updated or modified without the need for physical changes to the hardware, allowing for easy iteration and upgrades to a system's design.
- It is highly configurable, meaning it can be seamlessly integrated with other IP cores and custom logic within an FPGA, 
    - This enables the development of complex, integrated systems on a single chip.



###  Serial Peripheral Interface
- The accelerometer in this system is a peripheral device that communicates with  NIOS II via a Serial Peripheral Interface (SPI).
- SPI is a synchronous serial communication protocol used for short-distance communication, primarily in embedded systems.
- SPI is a Master-Slave Architecture, meaning the master device (NIOS II) initiates and controls the communication with the slave device (accelerometer)
- SPI has four logic signals 
    - SCLK (Serial Clock)
        - The clock signal from NIOS II
    - MOSI (Master Out Slave In)
        - The data output from NIOS II
    - MISO  (Master In Slave Out)
        - The data output from the accelerometer
    - SS (Slave Select)
        - Decides which peripheral to talk to (in this case, it is only used to start and finish communications)
     
<p align="center">
  <img src="Lab_3_Images/SPI.jpg" alt="SPI Diagram">
</p>

### Platform Designer
"Platform Designer is used to design the system as a whole and then generate the FPGA programmable logic to implement the system on the DE10 board.

<p align="center">
  <img src="Lab_3_Images/Qsys.jpg" alt="QSYS System">
</p>

- The system uses on_built memory as this is the fastest and has sufficient storage to run our simple program
- Accelerometer_SPI to effectively communicate with the peripheral accelerometer on the DE-10 Board

## Task 2: Understanding the Code

Task 2 was to understand the code behind the 'spirit level' program that ran on the system created in task 1.

### Global Variables

- `OFFSET` and `PWM_PERIOD` are constants for adjusting the accelerometer's readout and defining the PWM (Pulse Width Modulation) period, respectively.

- `pwm` is a variable used to control the LED brightness (via PWM).
- `led` and `level` are used to store the LED pattern to display, and the tilt level is derived from the accelerometer's data.

### Function `led_write`

This function writes a given LED pattern to the LEDs. It uses the `IOWR` macro (I/O write) to send the pattern to the LED base address, controlling which LEDs are turned on.

### Function `convert_read`
```C
void convert_read(alt_32 acc_read, int * level, alt_u8 * led) {
    acc_read += OFFSET;
    alt_u8 val = (acc_read >> 6) & 0x07;
    *led = (8 >> val) | (8 << (8 - val));
    *level = (acc_read >> 1) & 0x1f;
}
```

This function processes the raw accelerometer data:

- Adjusts the read value by an offset.
- Calculates a value 'val' used to determine the LED pattern and the tilt level.
    - Accelerometer data is right-shifted by 6 bits and then bitwise-ANDed with 0x07.
    - This isolates the 3MSB as a value in the range of 0 to 7, which tells us which LED to turn on.
- Updates the LED pattern based on `value`.
    - `val` is used to to determine which light to turn on
    -  For example, if 'val' is 6 we want the 7th light to turn on.
        -  In this case, the code uses `8<<(8 - val);`
        -  Which shifts `0b1000` left by 2 to give `0b100000`, which will turn on the 7th LED
- Calculates the tilt
    - using the [6:1] bits to give a coarse measurement of the accelerometer's tilt. 

### Function `sys_timer_isr`
```C
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
```

This is the timer interrupt service routine (ISR) that adjusts the LED pattern based on the PWM value and the tilt level:

- Clears the timer interrupt to acknowledge that the interrupt has been serviced.
- Shifts the LED pattern left or right based on the tilt direction.
- Resets or increments the PWM counter to adjust the LED brightness.
      - PWM controls the proportion of time the signal is high (ON) compared to the total time period of the cycle.
      - This effectively controls the brightness of the LEDs with the max being a PWM of 16
      - By incrementing by one it created a smooth transition across the states.

### Function `timer_init`

- Initializes the system timer for generating periodic interrupts:
- Sets up the timer with a specific period.
- Registers the timer ISR.
- Starts the timer with interrupts enabled.


### Main Function
```C
int main() {
    alt_32 x_read;
    alt_up_accelerometer_spi_dev *acc_dev;
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
    if (acc_dev == NULL) { // if return 1, check if the spi ip name is "accelerometer_spi"
        return 1;
    }

    timer_init(sys_timer_isr);
    while (1) {
        alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
        // alt_printf("raw data: %x\n", x_read);
        convert_read(x_read, &level, &led);
    }

    return 0;
}
```
Initializes the accelerometer device.
    
- Sets up the timer to periodically update the LED pattern based on the accelerometer's orientation.
- Continuously reads the x-axis data from the accelerometer and processes it to determine the LED pattern and brightness.


## Task 3: Implement an FIR filter to process the data

Task 3 was to process the accelerometer data using an FIR filter.

### FIR Filter Data Processing

- The raw data from the accelerometer is prone to noise and sudden fluctuations.
- An FIR filter helps to reduce this by smoothing out transitions and removing anomalous readings (i.e. noise).
- Put simply, an FIR filter takes specified parts of the input signal and multiplies them by coefficients. 
- If all coefficients are the same, it effectively averages the signal, resulting in a smoother output.
- Taps in FIR filters refer to each component of the input signal that is multiplied by its corresponding coefficient, allowing for precise control over the filter's response.

### FIR Filter Design

To determine what parameters to use in the filter, the Matlab filter design plugin was used to give the following FIR filter.
<p align="center">
  <img src="Lab_3_Images/FIR.png" alt="FIR Filter">
</p>

The chosen parameters strike a balance between effectively filtering out noise and preserving the essential characteristics of the signal. 

The visualization in MATLAB (shown above) confirmed that the selected design minimizes the signal's delay and distortion, making it suitable for real-time applications.

## **Challenge:** Implement the FIR Using Fixed Point Operations 
The challenge involved optimizing the FIR filter to enhance its performance without sacrificing data quality.

### Floating-Point VS Fixed Point

- **Floating Point** is a way of implementing almost all numbers whereby the decimal point and precision move depending on the number.

- **Fixed-point** representation is a method of storing real numbers in binary where a specific number of bits is reserved for the integer part and the fractional part, with a fixed number of decimal places.

Fixed point arithmetic has native support on NIOS II and so is significantly more efficient for this system.


### FIR Filter Implementation 

```C
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

```

- The `queue` data type stores all of the required readings in an array of chronological order
    - Queue contains a `start` index for the most recent element in the queue.
    - The start index moves around in a circular pattern, overwriting the oldest data with new data as it goes.
    - This method eliminates the need for shifting elements to maintain the most recent set of samples necessary for the FIR filter calculations.
          - This brings the complexity of this part of the code down from O(N) to O(1). 

- The filtering function works simply by multiplying an array of samples with the corresponding value in a global array of coefficients. 
- Each time the function runs, the newest reading replaces the oldest reading, and the `start` index is moved to that place.

- The function uses fixed-point operations through the use of integer arithmetic while implicitly treating the data as having 4 decimal places
    - The function returns the code divided by 10000, which converts it back to a floating point representation.

### What is the impact on the performance of the application?

- Execution Time: The switch to fixed-point arithmetic typically reduces the execution time of the filtering function since fixed-point operations are generally faster than floating-point operations, especially on processors without native floating-point support.

- Efficiency: Fixed-point arithmetic is more hardware-efficient, allowing the NIOS II processor to execute the filter code using fewer clock cycles, potentially increasing the application's overall throughput.

- Predictability: Fixed-point operations have deterministic execution times, which is beneficial for real-time processing and leads to a more predictable application performance.

### What is the impact on the transfer function?

- Quantization Noise: Fixed-point representation introduces quantization noise, which might slightly alter the filter's frequency response, especially if the number of bits for the fractional part is not sufficiently large.
    
- Precision and Resolution: The precision and resolution of the filter coefficients are limited compared to floating-point, which can affect the accuracy of the filter's transfer function, potentially leading to a less accurate approximation of the intended filter response.

- Stability: The stability of the FIR filter is not inherently affected by the switch to fixed-point arithmetic (since FIR filters are inherently stable).

### Measuring the Sampling Frequency of the Accelerometer to Compare Efficiency

To see how effective the new code was, we can measure the sampling frequency of the accelerometer. 

To do this, we use a modified version of the following provided code.

```C
# include “alt_types.h”
# include “sys/times.h”
…
clock_t exec_t1, exec_t2;
exec_t1 = times(NULL); // get system time before starting the process

// The code that you want to time goes here
// some code…
// till here
exec_t2 = times(NULL); // get system time after finishing the process
printf(" proc time = %d ticks \n", int(exec_t2-exec_t1)); % print the number of ticks on the screen
```
> [!NOTE]
> In order to set up the clock correctly:
> - A second timer was added in the QSYS design.
> - The `sys/alt_timestamp.h` header was used instead of `sys/times.h`
> - `alt_timestamp_start()` and `alt_timestamp()` were used instead to measure time.

From the testing, we found a significant improvement in efficiency. 
This could also seen on the board itself as the LEDs were significantly more reactive with than without fixed point operations.

## Final Code

```C
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


int main() {

    alt_32 x_read;
    alt_32 x_filtered;
    queue_t prev_x_vals = {0};    // Buffer for the samples

    alt_up_accelerometer_spi_dev *acc_dev;
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
    if (acc_dev == NULL) {
        return 1; // Accelerometer not found
    }

    timer_init(sys_timer_isr);

    // Start timer
    alt_timestamp_start();
    alt_32 loop_timer = 0;
    alt_32 loop_counter = 0;

    while (1) {
        alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
        prev_x_vals.start = (prev_x_vals.start + 1) % TAPS;
        prev_x_vals.elems[prev_x_vals.start] = x_read;

        /* Include fixed-point FIR filter processing here */
        x_filtered = fixed_point_filter(&prev_x_vals);

        convert_read(x_filtered, &level, &led);

        loop_timer += alt_timestamp(); // Get system time after finishing the process
        /* Timer processing functions hidden outside timing window to improve accuracy */
        loop_counter++;
        if (!(loop_counter % 100)) {
            /* Print the number of ticks on the screen */
            printf("Average loop processing time is %d ticks\n", (int) (loop_timer/100));
            loop_counter = 0;
            loop_timer = 0;
        }
        alt_timestamp_start();  // Restart system timer
    }

}
```
