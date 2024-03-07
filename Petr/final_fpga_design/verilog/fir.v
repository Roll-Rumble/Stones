module fir(
	input 							clk,
	input 							sample_tick,
	input signed					[31:0] 	value_in,
	output reg signed					[31:0] 	value_out

);


reg signed [31:0] coeff_arr [0:19];
reg signed [31:0] sample_arr [0:19];

integer j;

initial begin
	coeff_arr[0] = 52; coeff_arr[1] = -529; coeff_arr[2] = 102; coeff_arr[3] = 264; coeff_arr[4] = -495;
	coeff_arr[5] = 151; coeff_arr[6] = 657; coeff_arr[7] = -1146; coeff_arr[8] = 164; coeff_arr[9] = 5423;
	coeff_arr[10] = 5423; coeff_arr[11] = 164; coeff_arr[12] = -1146; coeff_arr[13] = 657; coeff_arr[14] = 151;
	coeff_arr[15] = -495; coeff_arr[16] = 264; coeff_arr[17] = 102; coeff_arr[18] = -529; coeff_arr[19] = 52;
	
	
	for(j = 0; j < 20; j = j+1) begin
		sample_arr[j] = 0;
	end
end


always @ (posedge clk) begin
	value_out <= sample_arr[0] * coeff_arr[0] +
                sample_arr[1] * coeff_arr[1] +
                sample_arr[2] * coeff_arr[2] +
                sample_arr[3] * coeff_arr[3] +
                sample_arr[4] * coeff_arr[4] +
                sample_arr[5] * coeff_arr[5] +
                sample_arr[6] * coeff_arr[6] +
                sample_arr[7] * coeff_arr[7] +
                sample_arr[8] * coeff_arr[8] +
                sample_arr[9] * coeff_arr[9] +
                sample_arr[10] * coeff_arr[10] +
                sample_arr[11] * coeff_arr[11] +
                sample_arr[12] * coeff_arr[12] +
                sample_arr[13] * coeff_arr[13] +
                sample_arr[14] * coeff_arr[14] +
                sample_arr[15] * coeff_arr[15] +
                sample_arr[16] * coeff_arr[16] +
                sample_arr[17] * coeff_arr[17] +
                sample_arr[18] * coeff_arr[18] +
                sample_arr[19] * coeff_arr[19];
end

integer i;

always @ (posedge sample_tick) begin
	 sample_arr[19] <= sample_arr[18];
    sample_arr[18] <= sample_arr[17];
    sample_arr[17] <= sample_arr[16];
    sample_arr[16] <= sample_arr[15];
    sample_arr[15] <= sample_arr[14];
    sample_arr[14] <= sample_arr[13];
    sample_arr[13] <= sample_arr[12];
    sample_arr[12] <= sample_arr[11];
    sample_arr[11] <= sample_arr[10];
    sample_arr[10] <= sample_arr[9];
    sample_arr[9] <= sample_arr[8];
    sample_arr[8] <= sample_arr[7];
    sample_arr[7] <= sample_arr[6];
    sample_arr[6] <= sample_arr[5];
    sample_arr[5] <= sample_arr[4];
    sample_arr[4] <= sample_arr[3];
    sample_arr[3] <= sample_arr[2];
    sample_arr[2] <= sample_arr[1];
    sample_arr[1] <= sample_arr[0];
    sample_arr[0] <= value_in;
end

endmodule