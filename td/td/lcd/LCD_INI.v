module LCD_INI(
    input              clk,
    input              rstn,
    input              en,
    output             Initial_finish,
    /*output    reg      LCD_CS,
    output    reg      LCD_RST,
    output    reg      LCD_RS,
    output    reg      LCD_WR,
    output    reg      LCD_RD,
    output    reg  [15:0]  LCD_DATA,
    output    reg      LCD_BL_CTR*/
    output          LCD_CS,
    output          LCD_RST,
    output          LCD_RS,
    output          LCD_WR,
    output          LCD_RD,
    output      [15:0]  LCD_DATA,
    output          LCD_BL_CTR
);

localparam  ADDR_WIDTH = 17;

assign LCD_RST    = rstn;
assign LCD_RD     = 1'b1;
assign LCD_BL_CTR = 1'b1;


wire addr_en;
WriteCtrl WriteCtrl(
     .clk               (clk)
    ,.rstn              (rstn)
    ,.en                (en)
    ,.data_stop         (Initial_finish)
    ,.addr_en           (addr_en)
    ,.LCD_CS            (LCD_CS)
    ,.LCD_WR            (LCD_WR)
);

/*****************************************/
//addr
/*****************************************/
wire [ADDR_WIDTH-1:0] addr;
AddrIni # (
     .ADDR_WIDTH  (ADDR_WIDTH)
)     AddrIni (
     .clk             (clk)
    ,.rstn            (rstn)
    ,.cnt_en          (addr_en)
    ,.Initial_finish  (Initial_finish)
    ,.addr            (addr)
);


/*****************************************/
//BROM
/*****************************************/
BlockROM16 # (
     .ADDR_WIDTH  (8) 
    ,.DATA_WIDTH  (16)
)     BlockROM_Data (
     .clk         (clk)
    ,.addr        (addr[7:0])
    ,.data        (LCD_DATA)
);

BlockROM9 # (
     .ADDR_WIDTH  (8)
    ,.DATA_WIDTH  (1)
)     BlockROM_Flag (
     .clk         (clk)
    ,.addr        (addr[7:0])
    ,.data        (LCD_RS)// rs = 0 -> Command Addr
);

endmodule
