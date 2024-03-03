
module nios_accelerometer (
	accelerometer_spi_external_interface_I2C_SDAT,
	accelerometer_spi_external_interface_I2C_SCLK,
	accelerometer_spi_external_interface_G_SENSOR_CS_N,
	accelerometer_spi_external_interface_G_SENSOR_INT,
	clk_clk,
	fir_in_x_external_connection_export,
	fir_out_x_external_connection_export,
	led_external_connection_export,
	reset_reset_n);	

	inout		accelerometer_spi_external_interface_I2C_SDAT;
	output		accelerometer_spi_external_interface_I2C_SCLK;
	output		accelerometer_spi_external_interface_G_SENSOR_CS_N;
	input		accelerometer_spi_external_interface_G_SENSOR_INT;
	input		clk_clk;
	input	[30:0]	fir_in_x_external_connection_export;
	output	[30:0]	fir_out_x_external_connection_export;
	output	[9:0]	led_external_connection_export;
	input		reset_reset_n;
endmodule
