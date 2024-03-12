module fir(
	input 							clk,
	input 							sample_tick,
	input signed					[31:0] 	value_in,
	output reg signed					[31:0] 	value_out

);


reg signed [31:0] coeff_arr [0:48];
reg signed [31:0] sample_arr [0:48];

integer j;

initial begin
    coeff_arr[0] = 46; coeff_arr[1] = 74; coeff_arr[2] = -24; coeff_arr[3] = -71; coeff_arr[4] = 33;
    coeff_arr[5] = 1; coeff_arr[6] = -94; coeff_arr[7] = 40; coeff_arr[8] = 44; coeff_arr[9] = -133;
    coeff_arr[10] = 30; coeff_arr[11] = 114; coeff_arr[12] = -179; coeff_arr[13] = -11; coeff_arr[14] = 223;
    coeff_arr[15] = -225; coeff_arr[16] = -109; coeff_arr[17] = 396; coeff_arr[18] = -263; coeff_arr[19] = -338;
    coeff_arr[20] = 752; coeff_arr[21] = -289; coeff_arr[22] = -1204; coeff_arr[23] = 2879; coeff_arr[24] = 6369;
    coeff_arr[25] = 2879; coeff_arr[26] = -1204; coeff_arr[27] = -289; coeff_arr[28] = 752; coeff_arr[29] = -338;
    coeff_arr[30] = -263; coeff_arr[31] = 396; coeff_arr[32] = -109; coeff_arr[33] = -225; coeff_arr[34] = 223;
    coeff_arr[35] = -11; coeff_arr[36] = -179; coeff_arr[37] = 114; coeff_arr[38] = 30; coeff_arr[39] = -133;
    coeff_arr[40] = 44; coeff_arr[41] = 40; coeff_arr[42] = -94; coeff_arr[43] = 1; coeff_arr[44] = 33;
    coeff_arr[45] = -71; coeff_arr[46] = -24; coeff_arr[47] = 74; coeff_arr[48] = 46;

    for(j = 0; j < 49; j = j + 1) begin
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
                 sample_arr[19] * coeff_arr[19] +
                 sample_arr[20] * coeff_arr[20] +
                 sample_arr[21] * coeff_arr[21] +
                 sample_arr[22] * coeff_arr[22] +
                 sample_arr[23] * coeff_arr[23] +
                 sample_arr[24] * coeff_arr[24] +
                 sample_arr[25] * coeff_arr[25] +
                 sample_arr[26] * coeff_arr[26] +
                 sample_arr[27] * coeff_arr[27] +
                 sample_arr[28] * coeff_arr[28] +
                 sample_arr[29] * coeff_arr[29] +
                 sample_arr[30] * coeff_arr[30] +
                 sample_arr[31] * coeff_arr[31] +
                 sample_arr[32] * coeff_arr[32] +
                 sample_arr[33] * coeff_arr[33] +
                 sample_arr[34] * coeff_arr[34] +
                 sample_arr[35] * coeff_arr[35] +
                 sample_arr[36] * coeff_arr[36] +
                 sample_arr[37] * coeff_arr[37] +
                 sample_arr[38] * coeff_arr[38] +
                 sample_arr[39] * coeff_arr[39] +
                 sample_arr[40] * coeff_arr[40] +
                 sample_arr[41] * coeff_arr[41] +
                 sample_arr[42] * coeff_arr[42] +
                 sample_arr[43] * coeff_arr[43] +
                 sample_arr[44] * coeff_arr[44] +
                 sample_arr[45] * coeff_arr[45] +
                 sample_arr[46] * coeff_arr[46] +
                 sample_arr[47] * coeff_arr[47] +
                 sample_arr[48] * coeff_arr[48];
end


integer i;

always @ (posedge sample_tick) begin
    sample_arr[48] <= sample_arr[47];
    sample_arr[47] <= sample_arr[46];
    sample_arr[46] <= sample_arr[45];
    sample_arr[45] <= sample_arr[44];
    sample_arr[44] <= sample_arr[43];
    sample_arr[43] <= sample_arr[42];
    sample_arr[42] <= sample_arr[41];
    sample_arr[41] <= sample_arr[40];
    sample_arr[40] <= sample_arr[39];
    sample_arr[39] <= sample_arr[38];
    sample_arr[38] <= sample_arr[37];
    sample_arr[37] <= sample_arr[36];
    sample_arr[36] <= sample_arr[35];
    sample_arr[35] <= sample_arr[34];
    sample_arr[34] <= sample_arr[33];
    sample_arr[33] <= sample_arr[32];
    sample_arr[32] <= sample_arr[31];
    sample_arr[31] <= sample_arr[30];
    sample_arr[30] <= sample_arr[29];
    sample_arr[29] <= sample_arr[28];
    sample_arr[28] <= sample_arr[27];
    sample_arr[27] <= sample_arr[26];
    sample_arr[26] <= sample_arr[25];
    sample_arr[25] <= sample_arr[24];
    sample_arr[24] <= sample_arr[23];
    sample_arr[23] <= sample_arr[22];
    sample_arr[22] <= sample_arr[21];
    sample_arr[21] <= sample_arr[20];
    sample_arr[20] <= sample_arr[19];
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