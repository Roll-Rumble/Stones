	nios_accelerometer u0 (
		.accelerometer_spi_external_interface_I2C_SDAT      (<connected-to-accelerometer_spi_external_interface_I2C_SDAT>),      // accelerometer_spi_external_interface.I2C_SDAT
		.accelerometer_spi_external_interface_I2C_SCLK      (<connected-to-accelerometer_spi_external_interface_I2C_SCLK>),      //                                     .I2C_SCLK
		.accelerometer_spi_external_interface_G_SENSOR_CS_N (<connected-to-accelerometer_spi_external_interface_G_SENSOR_CS_N>), //                                     .G_SENSOR_CS_N
		.accelerometer_spi_external_interface_G_SENSOR_INT  (<connected-to-accelerometer_spi_external_interface_G_SENSOR_INT>),  //                                     .G_SENSOR_INT
		.clk_clk                                            (<connected-to-clk_clk>),                                            //                                  clk.clk
		.fir_in_x_external_connection_export                (<connected-to-fir_in_x_external_connection_export>),                //         fir_in_x_external_connection.export
		.fir_out_x_external_connection_export               (<connected-to-fir_out_x_external_connection_export>),               //        fir_out_x_external_connection.export
		.led_external_connection_export                     (<connected-to-led_external_connection_export>),                     //              led_external_connection.export
		.reset_reset_n                                      (<connected-to-reset_reset_n>),                                      //                                reset.reset_n
		.sample_tick_external_connection_export             (<connected-to-sample_tick_external_connection_export>)              //      sample_tick_external_connection.export
	);

