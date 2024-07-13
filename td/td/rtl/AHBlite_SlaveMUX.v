module AHBlite_SlaveMUX (

    input HCLK,
    input HRESETn,
    input HREADY,

    //port 0
    input P0_HSEL,
    input P0_HREADYOUT,
    input P0_HRESP,
    input [31:0] P0_HRDATA,

    //port 1
    input P1_HSEL,
    input P1_HREADYOUT,
    input P1_HRESP,
    input [31:0] P1_HRDATA,

    //port 2
    input P2_HSEL,
    input P2_HREADYOUT,
    input P2_HRESP,
    input [31:0] P2_HRDATA,

    //port 3
    input P3_HSEL,
    input P3_HREADYOUT,
    input P3_HRESP,
    input [31:0] P3_HRDATA,
	
	//port 4
    input P4_HSEL,
    input P4_HREADYOUT,
    input P4_HRESP,
    input [31:0] P4_HRDATA,
	
    //port 5
    input P5_HSEL,
    input P5_HREADYOUT,
    input P5_HRESP,
    input [31:0] P5_HRDATA,	

    //port 6
    input P6_HSEL,
    input P6_HREADYOUT,
    input P6_HRESP,
    input [31:0] P6_HRDATA,	

    //port 7
    input P7_HSEL,
    input P7_HREADYOUT,
    input P7_HRESP,
    input [31:0] P7_HRDATA,		

    //port 8
    input P8_HSEL,
    input P8_HREADYOUT,
    input P8_HRESP,
    input [31:0] P8_HRDATA,			

    //port 8
    input P9_HSEL,
    input P9_HREADYOUT,
    input P9_HRESP,
    input [31:0] P9_HRDATA,	


    //port 10
    input P10_HSEL,
    input P10_HREADYOUT,
    input P10_HRESP,
    input [31:0] P10_HRDATA,	
    //port 11
    input P11_HSEL,
    input P11_HREADYOUT,
    input P11_HRESP,
    input [31:0] P11_HRDATA,

    //port 12
    input P12_HSEL,
    input P12_HREADYOUT,
    input P12_HRESP,
    input [31:0] P12_HRDATA,

    //output
    output wire HREADYOUT,
    output wire HRESP,
    output wire [31:0] HRDATA
);

//reg the hsel
reg [12:0] hsel_reg;

always@(posedge HCLK or negedge HRESETn) begin
    if(~HRESETn) hsel_reg <= 11'b0;
    else if(HREADY) hsel_reg <= {P0_HSEL,P1_HSEL,P2_HSEL,P3_HSEL,P4_HSEL,P5_HSEL,P6_HSEL,P7_HSEL,P8_HSEL,P9_HSEL,P10_HSEL,P11_HSEL,P12_HSEL};
end

//hready mux
reg hready_mux;

always@(*) begin
    case(hsel_reg)
   13'b0000000000001: begin hready_mux = P12_HREADYOUT;end
   13'b0000000000010: begin hready_mux = P11_HREADYOUT;end
   13'b0000000000100: begin hready_mux = P10_HREADYOUT;end
   13'b0000000001000: begin hready_mux = P9_HREADYOUT;end
   13'b0000000010000: begin hready_mux = P8_HREADYOUT;end
   13'b0000000100000: begin hready_mux = P7_HREADYOUT;end
   13'b0000001000000: begin hready_mux = P6_HREADYOUT;end
   13'b0000010000000: begin hready_mux = P5_HREADYOUT;end
   13'b0000100000000: begin hready_mux = P4_HREADYOUT;end
   13'b0001000000000: begin hready_mux = P3_HREADYOUT;end
   13'b0010000000000: begin hready_mux = P2_HREADYOUT;end
   13'b0100000000000: begin hready_mux = P1_HREADYOUT;end
   13'b1000000000000: begin hready_mux = P0_HREADYOUT;end
    default : begin hready_mux = 1'b1;end
    endcase
end

assign HREADYOUT = hready_mux;

//hresp mux
reg hresp_mux;

always@(*) begin
    case(hsel_reg)
   13'b0000000000001: begin hresp_mux = P12_HRESP;end
   13'b0000000000010: begin hresp_mux = P11_HRESP;end
   13'b0000000000100: begin hresp_mux = P10_HRESP;end
   13'b0000000001000: begin hresp_mux = P9_HRESP;end
   13'b0000000010000: begin hresp_mux = P8_HRESP;end
   13'b0000000100000: begin hresp_mux = P7_HRESP;end
   13'b0000001000000: begin hresp_mux = P6_HRESP;end
   13'b0000010000000: begin hresp_mux = P5_HRESP;end
   13'b0000100000000: begin hresp_mux = P4_HRESP;end
   13'b0001000000000: begin hresp_mux = P3_HRESP;end
   13'b0010000000000: begin hresp_mux = P2_HRESP;end
   13'b0100000000000: begin hresp_mux = P1_HRESP;end
   13'b1000000000000: begin hresp_mux = P0_HRESP;end
    default : begin hresp_mux = 1'b0;end
    endcase
end

assign HRESP = hresp_mux;

//hrdata mux
reg [31:0] hrdata_mux;

always@(*) begin
    case(hsel_reg)
   13'b0000000000001: begin hrdata_mux = P12_HRDATA;end
   13'b0000000000010: begin hrdata_mux = P11_HRDATA;end
   13'b0000000000100: begin hrdata_mux = P10_HRDATA;end
   13'b0000000001000: begin hrdata_mux = P9_HRDATA;end
   13'b0000000010000: begin hrdata_mux = P8_HRDATA;end
   13'b0000000100000: begin hrdata_mux = P7_HRDATA;end
   13'b0000001000000: begin hrdata_mux = P6_HRDATA;end
   13'b0000010000000: begin hrdata_mux = P5_HRDATA;end
   13'b0000100000000: begin hrdata_mux = P4_HRDATA;end
   13'b0001000000000: begin hrdata_mux = P3_HRDATA;end
   13'b0010000000000: begin hrdata_mux = P2_HRDATA;end
   13'b0100000000000: begin hrdata_mux = P1_HRDATA;end
   13'b1000000000000: begin hrdata_mux = P0_HRDATA;end
    default : begin hrdata_mux = 32'b0;end
    endcase
end

assign HRDATA = hrdata_mux;

endmodule 