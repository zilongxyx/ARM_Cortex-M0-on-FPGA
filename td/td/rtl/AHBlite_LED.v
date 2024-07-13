module AHBlite_LED(
    input  wire          HCLK,    
    input  wire          HRESETn, 
    input  wire          HSEL,    
    input  wire   [31:0] HADDR,   
    input  wire    [1:0] HTRANS,  
    input  wire    [2:0] HSIZE,   
    input  wire    [3:0] HPROT,   
    input  wire          HWRITE,  
    input  wire   [31:0] HWDATA,  
    input  wire          HREADY,  
    output wire          HREADYOUT, 
    output wire   [31:0] HRDATA,  
    output wire          HRESP,
	output reg    [7:0]  LED
);

assign HRESP = 1'b0;
assign HREADYOUT = 1'b1;

reg	[7:0]	WaterLight_mode; 	//模式改变

wire write_en;
assign write_en = HSEL & HTRANS[1] & HWRITE & HREADY;

reg wr_en_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) wr_en_reg <= 1'b0;
  else if(write_en) wr_en_reg <= 1'b1;
  else wr_en_reg <= 1'b0;
end

always@(posedge HCLK or negedge HRESETn) begin
    if(~HRESETn)
		WaterLight_mode <= 8'd0;
    else if(wr_en_reg && HREADY)
        WaterLight_mode <= HWDATA[7:0];
	else if(mode1 == 8'h00 || mode2 == 8'h00 || mode3 == 8'h00)
		WaterLight_mode <= 8'h00;		


    
end

assign HRDATA = {24'b0,WaterLight_mode};


//------------------------------------------------------
//  PWM
//------------------------------------------------------

reg [31:0] pwm_cnt;

always@(posedge HCLK or negedge HRESETn) begin
    if(~HRESETn) pwm_cnt <= 32'b0;
    else if(pwm_cnt == 20'd781250) pwm_cnt <= 32'b0;
    else pwm_cnt <= pwm_cnt + 1'b1;
end

reg light_clk;

always@(posedge HCLK or negedge HRESETn) begin
    if(~HRESETn) light_clk <= 1'b0;
    else if(pwm_cnt == 20'd781250) light_clk <= ~light_clk;
end



//------------------------------------------------------
//  OFF MODE
//------------------------------------------------------

wire [7:0] mode0;

assign mode0 = 8'h00;


//------------------------------------------------------
//  LEFT MODE
//------------------------------------------------------

reg [7:0] mode1;

always@(posedge light_clk or negedge HRESETn) begin
    if(~HRESETn) mode1 <= 8'h01;
    else if(WaterLight_mode == 8'h01)		//模式1
	  begin
        case(mode1)
        8'h01 : mode1 <= 8'h02;
        8'h02 : mode1 <= 8'h04;
        8'h04 : mode1 <= 8'h08;
        8'h08 : mode1 <= 8'h10;
        8'h10 : mode1 <= 8'h20;
        8'h20 : mode1 <= 8'h40;
        8'h40 : mode1 <= 8'h80;
        default : mode1 <= 8'h00;
        endcase
      end
	else
		mode1 <= 8'h01;
end

//------------------------------------------------------
//  RIGHT MODE
//------------------------------------------------------

reg [7:0] mode2;

always@(posedge light_clk or negedge HRESETn) begin
    if(~HRESETn) mode2 <= 8'h80;
    else if(WaterLight_mode == 8'h02)		//模式2
	  begin
        case(mode2)
		8'h80 : mode2 <= 8'h40;
        8'h40 : mode2 <= 8'h20;
        8'h20 : mode2 <= 8'h10;
        8'h10 : mode2 <= 8'h08;
        8'h08 : mode2 <= 8'h04;
        8'h04 : mode2 <= 8'h02;
        8'h02 : mode2 <= 8'h01;
        default : mode2 <= 8'h00;
        endcase
	  end
	else
		mode2 <= 8'h80;
end

//------------------------------------------------------
//  DANCE MODE
//------------------------------------------------------

reg [7:0] mode3;

always@(posedge light_clk or negedge HRESETn) begin
    if(~HRESETn) mode3 <= 8'h18;
    else if(WaterLight_mode == 8'h03)		//模式3
	  begin
        case(mode3)
        8'h18 : mode3 <= 8'h24;
        8'h24 : mode3 <= 8'h42;
        8'h42 : mode3 <= 8'h81;
        8'h81 : mode3 <= 8'h00;
        default : mode3 <= 8'h00;
        endcase
      end
	else
		mode3 <= 8'h18;
end

//------------------------------------------------------
//  OUTPUT MUX
//------------------------------------------------------

always@(*) begin
    case(WaterLight_mode)
    8'h00 : begin LED = mode0;end  	
    8'h01 : begin LED = mode1;end  
    8'h02 : begin LED = mode2;end
    8'h03 : begin LED = mode3;end
    default : begin LED = 8'h00;end
    endcase
end


endmodule


