module AHBlite_QN8027_IIC(
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
    output reg      	 QN_IIC_SCL,
    inout  wire      	 QN_IIC_SDA
);

assign HRESP = 1'b0;
assign HREADYOUT = 1'b1;

wire write_en;
assign write_en = HSEL & HTRANS[1] & HWRITE & HREADY;

reg [1:0] addr_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) addr_reg <= 2'b0;
  else if(HSEL & HREADY & HTRANS[1]) addr_reg <= HADDR[3:2];  // 3 2 1 0 
end

reg wr_en_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) wr_en_reg <= 1'b0;
  else if(write_en) wr_en_reg <= 1'b1;
  else wr_en_reg <= 1'b0;
end

reg ACK;			//应答使能

reg QN_IIC_SDA_reg;
always@(posedge HCLK or negedge HRESETn) begin
    if(~HRESETn) begin
        QN_IIC_SCL     <= 1'b1;
        QN_IIC_SDA_reg <= 1'b1;
		ACK			   <= 1'b0;
    end else if(wr_en_reg && HREADY) begin
        if(addr_reg == 2'd0)
            QN_IIC_SCL     <= HWDATA[0];
        else if(addr_reg == 2'd1)   
            QN_IIC_SDA_reg <= HWDATA[0];
		else if(addr_reg == 2'd2)
			ACK			   <= HWDATA[0];
    end
end

assign QN_IIC_SDA = (ACK == 1'b1) ? 1'bz : QN_IIC_SDA_reg;

assign HRDATA = {31'b0,QN_IIC_SDA};

endmodule


