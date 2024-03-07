
//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================

module DE10_LITE_Golden_Top(

	//////////// CLOCK //////////
	input 		          		ADC_CLK_10,
	input 		          		MAX10_CLK1_50,
	input 		          		MAX10_CLK2_50,

	//////////// SDRAM //////////
	output		    [12:0]		DRAM_ADDR,
	output		     [1:0]		DRAM_BA,
	output		          		DRAM_CAS_N,
	output		          		DRAM_CKE,
	output		          		DRAM_CLK,
	output		          		DRAM_CS_N,
	inout 		    [15:0]		DRAM_DQ,
	output		          		DRAM_LDQM,
	output		          		DRAM_RAS_N,
	output		          		DRAM_UDQM,
	output		          		DRAM_WE_N,

	//////////// SEG7 //////////
	output		     [7:0]		HEX0,
	output		     [7:0]		HEX1,
	output		     [7:0]		HEX2,
	output		     [7:0]		HEX3,
	output		     [7:0]		HEX4,
	output		     [7:0]		HEX5,

	//////////// KEY //////////
	input 		     [1:0]		KEY,

	//////////// LED //////////
	output		     [9:0]		LEDR,

	//////////// SW //////////
	input 		     [9:0]		SW,

	//////////// VGA //////////
	output		     [3:0]		VGA_B,
	output		     [3:0]		VGA_G,
	output		          		VGA_HS,
	output		     [3:0]		VGA_R,
	output		          		VGA_VS,

	//////////// Accelerometer //////////
	output		          		GSENSOR_CS_N,
	input 		     [2:1]		GSENSOR_INT,
	output		          		GSENSOR_SCLK,
	inout 		          		GSENSOR_SDI,
	inout 		          		GSENSOR_SDO,

	//////////// Arduino //////////
	inout 		    [15:0]		ARDUINO_IO,
	inout 		          		ARDUINO_RESET_N,

	//////////// GPIO, GPIO connect to GPIO Default //////////
	inout 		    [35:0]		GPIO
	
);



//======================================================
//  REG/WIRE declarations
//======================================================

//Hardware FIR for x
wire 				[31:0]		FIR_X_IN;
wire				[31:0]		FIR_X_OUT;

wire							SAMPLE_TICK;


//=======================================================
//  Structural coding
//=======================================================
fir filter_x(
	.clk													(MAX10_CLK1_50),
	.value_in												(FIR_X_IN),
	.value_out												(FIR_X_OUT),
	.sample_tick											(SAMPLE_TICK)
);




nios_accelerometer u0 (
		.clk_clk                                            (MAX10_CLK1_50),                                            //                                  clk.clk
		.reset_reset_n                                      (1'b1),                                      //                                reset.reset_n
		.led_external_connection_export                     (LEDR[9:0]),                     //              led_external_connection.export
		.accelerometer_spi_external_interface_I2C_SDAT      (GSENSOR_SDI),      // accelerometer_spi_external_interface.I2C_SDAT
		.accelerometer_spi_external_interface_I2C_SCLK      (GSENSOR_SCLK),      //                                     .I2C_SCLK
		.accelerometer_spi_external_interface_G_SENSOR_CS_N (GSENSOR_CS_N), //                                     .G_SENSOR_CS_N
		.accelerometer_spi_external_interface_G_SENSOR_INT  (GSENSOR_INT[1]),   //                                     .G_SENSOR_INT
		.fir_in_x_external_connection_export                (FIR_X_IN),                //         fir_in_x_external_connection.export
		.fir_out_x_external_connection_export               (FIR_X_OUT),                //        fir_out_x_external_connection.export
		.sample_tick_external_connection_export             (SAMPLE_TICK)              //      sample_tick_external_connection.export
);



endmodule