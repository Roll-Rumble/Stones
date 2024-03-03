module fir(
	input logic clk,
	input logic value_in,
	
	output logic value_out

);

always @ (posedge clk) begin
//	value_out <= value_in << 2;
	value_out <= 1'b1;
end

//assign value_out = 3;

endmodule