module AHBlite_GPIO( 
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
    inout  wire   [7:0]  IO
);

assign HRESP = 1'b0;
assign HREADYOUT = 1'b1;

wire write_en;
assign write_en = HSEL & HTRANS[1] & HWRITE & HREADY;


reg [2:0] addr_reg;         // 0 1 2 3 4 5 6 7
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) addr_reg <= 3'b0;
  else if(write_en) addr_reg <= HADDR[2:0];   // 0 1 2 3 4 5 6 7
end


reg wr_en_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) wr_en_reg <= 1'b0;
  else if(write_en) wr_en_reg <= 1'b1;
  else wr_en_reg <= 1'b0;
end
reg [7:0] IO_reg;
always@(posedge HCLK or negedge HRESETn) begin
    if(~HRESETn) begin
		IO_reg  <= 3'b0;
    end else if(wr_en_reg && HREADY) begin
		case(addr_reg)
				3'b000: IO_reg[0]<= HWDATA[0];
				3'b001: IO_reg[1]<= HWDATA[0];
				3'b010: IO_reg[2]<= HWDATA[0];
				3'b011: IO_reg[3]<= HWDATA[0];
				3'b100: IO_reg[4]<= HWDATA[0];
				3'b101: IO_reg[5]<= HWDATA[0];
				3'b110: IO_reg[6]<= HWDATA[0];
				3'b111: IO_reg[7]<= HWDATA[0];
		endcase 
    end
end
assign IO[7:0] = IO_reg[7:0];
assign HRDATA = {24'b0,IO};
endmodule
