module AHBlite_SPI(
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
    output reg      	 SPI_CS,
    output reg      	 SPI_CLK,
    output reg      	 SPI_MOSI,
	input  wire			 SPI_MISO,
	input  wire			 SPI_IRQ,
	output  wire  	     spi_interrupt
);

reg [11:0]  spi;
wire[11:0]  spi_nxt;

assign  spi_nxt   =   spi + 1'b1;


always@(posedge HCLK or negedge HRESETn) begin
    if(~HRESETn) begin
        spi   <=  12'b0;
    end else if(SPI_IRQ==1'b0) begin
        if(spi != 12'hfff)
           spi   <=  spi_nxt;
    end else 
        spi   <=  12'b0;
end
assign spi_interrupt =   (spi != 12'hfff) & (spi_nxt == 12'hfff);

assign HRESP = 1'b0;
assign HREADYOUT = 1'b1;

wire write_en;
assign write_en = HSEL & HTRANS[1] & HWRITE & HREADY;

reg [1:0] addr_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) addr_reg <= 2'b0;
  else if(HSEL & HREADY & HTRANS[1]) addr_reg <= HADDR[3:2];
end

reg wr_en_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) wr_en_reg <= 1'b0;
  else if(write_en) wr_en_reg <= 1'b1;
  else wr_en_reg <= 1'b0;
end

always@(posedge HCLK or negedge HRESETn) begin
    if(~HRESETn) begin
        	SPI_CS   <= 1'b1;
       		SPI_CLK  <= 1'b1;
			SPI_MOSI <= 1'b0;
    end else if(wr_en_reg && HREADY) begin
        if(addr_reg == 2'd0)         // 0 4(0100) 8(1000) 12(1100)
           	SPI_CS   <= HWDATA[0];
        else if(addr_reg == 2'd1) 
            SPI_CLK  <= HWDATA[0];
        else if(addr_reg == 2'd2) 
            SPI_MOSI <= HWDATA[0];
    end
end

assign HRDATA = {31'b0,SPI_MISO};


endmodule



