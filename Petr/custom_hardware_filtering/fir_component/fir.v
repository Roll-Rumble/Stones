module fir(
	input logic 							clk,
	input logic 					[31:0] 	value_in,
	input logic 							sample_tick,
	
	output logic 					[31:0] 	value_out

);

logic [31:0] coeff_arr [4];
logic [31:0] sample_arr [4];

initial begin
	coeff_arr[0] = 1;
	coeff_arr[1] = 2;
	coeff_arr[2] = 4;
	coeff_arr[3] = 8;
	sample_arr[0] = 0;
	sample_arr[1] = 0;
	sample_arr[2] = 0;
	sample_arr[3] = 0;
end


always @ (posedge clk) begin
	value_out <= sample_arr[0] * coeff_arr[0] +
	sample_arr[1] * coeff_arr[1] +
	sample_arr[2] * coeff_arr[2] +
	sample_arr[3] * coeff_arr[3];
end

logic[31:0] i;

always @ (posedge sample_tick) begin
	sample_arr[0] <= value_in;
	for(i = 0; i < 3; i = i + 1) begin
		sample_arr[i+1] <= sample_arr[i];
	end

end

endmodule