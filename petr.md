## Design decisions
- Use hardware filtering on DE-10 lite (Petr)
We have decided to make our own FIR filter. We wrote a custom verilog file and connected it using PIO. This way all the multiplications and additions are happenning in paralell. 

- 1kHz polling rate for controller input (client multithreading) (Petr)
Because of our custom FIR hardware filtering, the response from the FPGA is very fast. However, at some point a further increase in the sampling rate becomes unnoticeable and no longer practical. Therefore, we have decided to set our controller polling rate to 1 kHz. This results in a latency of 1 ms and it is better than for example an Xbox controller which has a polling rate of 125 Hz. 

## Performance metrics
- processing for FIR filter vs software (Petr)

We have tested our implementation of the custom hardware FIR against our heavily optimised software implementation. The table below is the average of 3000 measurements. 

| Filter Type             | Clock Cycles | Resulting delay (ms)|
|-------------------------|--------------| -----|
| Optimised Software FIR     | 136,631      | 2.7|
| Custom Hardware FIR     | 1,592        | 0.0 |

The resulting delay was calculated in mind with the 50 MHz clock associated with Nios II and kept to a precision to one decimal digit. 
This custom FIR is 86 times faster than our already optimised software implemenataion. Therefore, the delay stemming from the FPGA controller is a non-issue. 







