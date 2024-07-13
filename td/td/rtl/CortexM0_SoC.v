`include "E:/fpga_cxcy/final_2/final/td/rtl/global_def.v"
module CortexM0_SoC (
        input  wire  System_clk,		//50MHz	
        input  wire  RSTn,		
        inout  wire  SWDIO,  	
        input  wire  SWCLK,


        output wire [7:0] LED,	        //LED

        output wire       TXD_1,	    //串口1
        input  wire       RXD_1,		
        output wire       TXD_2,	    //串口2
        input  wire       RXD_2,
		output wire		  QN_IIC_SCL,		//QN8027
		inout  wire		  QN_IIC_SDA,
		output wire		  QN_REF24MHz,	
		
		output wire		  PAJ_IIC_SCL,		//PAJ
		inout  wire		  PAJ_IIC_SDA,

		output wire [3:0] Row,          //矩阵键盘
		input  wire	[3:0] Col,
		output wire [3:0] Row_beep,          //矩阵键盘
		inout wire [7:0] IO	,        //io
		inout wire [7:0] IO_READ	,  // IO _READ
		output wire 	  SPI_CS,   	//
		output wire 	  SPI_CLK,
		output wire      	 SPI_MOSI,
		input  wire			 SPI_MISO,
		input  wire 		 SPI_IRQ,

    	input           en1,
    	input           en2,
    	input           en3,
		output          beep,

		output  led_test,
		output  led_test_mode,
		input  wire [7:0] key ,          //拨动按键

        output wire       LCD_CS,
        output wire       LCD_RS,
        output wire       LCD_WR,
        output wire       LCD_RD,
        output wire       LCD_RST,
        output wire[15:0] LCD_DATA,
        output wire       LCD_BL_CTR


		
		
);
//beep

wire en;
wire [15: 0] key_pulse;
assign en = key_pulse[0];

wire addr_finish;
wire beat_finish;
wire addr_en;
wire addr_rstn;
wire tune_pwm_en;
wire tune_pwm_rstn;
wire beat_cnt_en;
wire beat_cnt_rstn;
//按键控制蜂鸣器

key_16 keyboard(
     .clk(clk)
    ,.rstn(RSTn)
    ,.col(Col)
    ,.row_beep(row_beep)
    ,.key_pulse(key_pulse)
);

bzmusic_ctrl bzmusic_ctrl(
    .clk             (System_clk)
    ,.en             (en)
    ,.rstn           (RSTn)
    ,.addr_finish    (addr_finish)
    ,.beat_finish    (beat_finish)
    ,.addr_en        (addr_en)
    ,.addr_rstn      (addr_rstn)
    ,.tune_pwm_en    (tune_pwm_en)
    ,.tune_pwm_rstn  (tune_pwm_rstn)
    ,.beat_cnt_en    (beat_cnt_en)
    ,.beat_cnt_rstn  (beat_cnt_rstn) 
);

wire [8:0] addr;
addr_cnt addr_cnt(
     .clk          (System_clk)
    ,.en           (addr_en)
    ,.rstn         (addr_rstn)
    ,.data         (data)
    ,.addr         (addr)
    ,.addr_finish  (addr_finish)
);


wire [11:0] data1;
BlockROM1 #(
     .ADDR_WIDTH(8)
    ,.DATA_WIDTH(12)
) BlockROM1 (
     .en(en1)
    ,.clk(System_clk)
    ,.addr_i(addr)
    ,.data_o(data1)
);

wire [11:0] data2;
BlockROM2 #(
     .ADDR_WIDTH(8)
    ,.DATA_WIDTH(12)
) BlockROM2 (
     .en(en2)
    ,.clk(System_clk)
    ,.addr_i(addr)
    ,.data_o(data2)
);

wire [11:0] data3;
BlockROM3 #(
     .ADDR_WIDTH(8)
    ,.DATA_WIDTH(12)
) BlockROM3 (
     .en(en3)
    ,.clk(System_clk)
    ,.addr_i(addr)
    ,.data_o(data3)
);


wire [11:0] data;
AND AND(
     .data1 (data1)
    ,.data2 (data2)
    ,.data3 (data3)
    ,.en1(en1)
    ,.en2(en2)
    ,.en3(en3)
    ,.data (data)
);

wire [19:0] tune_pwm_parameter;
tune_decoder tune_decoder(
     .tune                (data[11:4])
    ,.tune_pwm_parameter  (tune_pwm_parameter)
);

tune_pwm tune_pwm(     
     .clk            (System_clk)
    ,.en             (tune_pwm_en)
    ,.rst_n          (tune_pwm_rstn)
    ,.pwm_parameter  (tune_pwm_parameter)
    ,.clk_pwm        (beep)
);


wire  [27:0]  beat_cnt_parameter;
beat_decoder beat_decoder(
     .beat                (data[3:0])
    ,.beat_cnt_parameter  (beat_cnt_parameter)
);

beat_cnt beat_cnt(
     .clk                 (System_clk)
    ,.en                  (beat_cnt_en)
    ,.rstn                (beat_cnt_rstn)
    ,.beat_cnt_parameter  (beat_cnt_parameter)
    ,.beat_finish         (beat_finish)   
);
//------------------------------------------------------------------------------
// DEBUG IOBUF 
//------------------------------------------------------------------------------

wire SWDO;
wire SWDOEN;
wire SWDI;

assign SWDI = SWDIO;
assign SWDIO = (SWDOEN) ?  SWDO : 1'bz;

//------------------------------------------------------------------------------
// Interrupt
//------------------------------------------------------------------------------
wire [7:0] key_interrupt;

Keyboard kb
(
   .HCLK(clk),
   .HRESETn(RSTn),
   .col(key),
   .key_interrupt(key_interrupt)
);

wire spi_interrupt;
wire LCD_INI_FINISH;
wire [31:0] IRQ;     			// interrupts output to core
//assign IRQ = {23'b0,key_interrupt,spi_interrupt};
assign IRQ = {30'b0,spi_interrupt,LCD_INI_FINISH};
/*Connect the IRQ with UART*/

/***************************/

wire RXEV;
assign RXEV = 1'b0;

//------------------------------------------------------------------------------
// PLL
wire	Lock;		//输出锁定
wire	Pwm_CLK;	//200MHz
wire	ADC_CLK;	//16 MHz
wire	clk;		//25 MHz
wire		  MSi_REF24MHz;
PLL	CLK_gen
(
	.refclk		(System_clk),
	.reset		(1'b0),
	.stdby		(1'b0),
	.extlock	(Lock),
	.clk0_out	(Pwm_CLK),
	.clk1_out	(QN_REF24MHz),
	.clk2_out	(ADC_CLK),
	.clk3_out	(MSi_REF24MHz),
	.clk4_out	(clk)
);
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// AHB
//------------------------------------------------------------------------------

wire [31:0] HADDR;
wire [ 2:0] HBURST;
wire        HMASTLOCK;
wire [ 3:0] HPROT;
wire [ 2:0] HSIZE;
wire [ 1:0] HTRANS;
wire [31:0] HWDATA;
wire        HWRITE;
wire [31:0] HRDATA;
wire        HRESP;
wire        HMASTER;
wire        HREADY;

//------------------------------------------------------------------------------
// RESET AND DEBUG
//------------------------------------------------------------------------------

wire SYSRESETREQ;
reg  cpuresetn;


always @(posedge System_clk or negedge RSTn)begin
        if (~RSTn) cpuresetn <= 1'b0;
        else if (SYSRESETREQ) cpuresetn <= 1'b0;
        else cpuresetn <= 1'b1;
end

wire CDBGPWRUPREQ;
reg CDBGPWRUPACK;

always @(posedge System_clk or negedge RSTn)begin
        if (~RSTn) CDBGPWRUPACK <= 1'b0;
        else CDBGPWRUPACK <= CDBGPWRUPREQ;
end


//------------------------------------------------------------------------------
// Instantiate Cortex-M0 processor logic level
//------------------------------------------------------------------------------

cortexm0ds_logic u_logic (

        // System inputs
        .FCLK           (System_clk),           //FREE running clock 
        .SCLK           (System_clk),           //system clock
        .HCLK           (System_clk),           //AHB clock
        .DCLK           (System_clk),           //Debug clock
        .PORESETn       (RSTn),          //Power on reset
        .HRESETn        (cpuresetn),     //AHB and System reset
        .DBGRESETn      (RSTn),          //Debug Reset
        .RSTBYPASS      (1'b0),          //Reset bypass
        .SE             (1'b0),          // dummy scan enable port for synthesis

       // Power management inputs
        .SLEEPHOLDREQn  (1'b1),          // Sleep extension request from PMU
        .WICENREQ       (1'b0),          // WIC enable request from PMU
        .CDBGPWRUPACK   (CDBGPWRUPACK),  // Debug Power Up ACK from PMU

        // Power management outputs
        .CDBGPWRUPREQ   (CDBGPWRUPREQ),
        .SYSRESETREQ    (SYSRESETREQ),

        // System bus
        .HADDR          (HADDR[31:0]),
        .HTRANS         (HTRANS[1:0]),
        .HSIZE          (HSIZE[2:0]),
        .HBURST         (HBURST[2:0]),
        .HPROT          (HPROT[3:0]),
        .HMASTER        (HMASTER),
        .HMASTLOCK      (HMASTLOCK),
        .HWRITE         (HWRITE),
        .HWDATA         (HWDATA[31:0]),
        .HRDATA         (HRDATA[31:0]),
        .HREADY         (HREADY),
        .HRESP          (HRESP),

        // Interrupts
        .IRQ            (IRQ),          //Interrupt
        .NMI            (1'b0),         //Watch dog interrupt
        .IRQLATENCY     (8'h0),
        .ECOREVNUM      (28'h0),

        // Systick
        .STCLKEN        (1'b0),
        .STCALIB        (26'h0),

        // Debug - JTAG or Serial wire
        // Inputs
        .nTRST          (1'b1),
        .SWDITMS        (SWDI),
        .SWCLKTCK       (SWCLK),
        .TDI            (1'b0),
        // Outputs
        .SWDO           (SWDO),
        .SWDOEN         (SWDOEN),

        .DBGRESTART     (1'b0),

        // Event communication
        .RXEV           (RXEV),         // Generate event when a DMA operation completed.
        .EDBGRQ         (1'b0)          // multi-core synchronous halt request
);

//------------------------------------------------------------------------------
// AHBlite Interconncet
//------------------------------------------------------------------------------

wire            HSEL_P0;
wire    [31:0]  HADDR_P0;
wire    [2:0]   HBURST_P0;
wire            HMASTLOCK_P0;
wire    [3:0]   HPROT_P0;
wire    [2:0]   HSIZE_P0;
wire    [1:0]   HTRANS_P0;
wire    [31:0]  HWDATA_P0;
wire            HWRITE_P0;
wire            HREADY_P0;
wire            HREADYOUT_P0;
wire    [31:0]  HRDATA_P0;
wire            HRESP_P0;

wire            HSEL_P1;
wire    [31:0]  HADDR_P1;
wire    [2:0]   HBURST_P1;
wire            HMASTLOCK_P1;
wire    [3:0]   HPROT_P1;
wire    [2:0]   HSIZE_P1;
wire    [1:0]   HTRANS_P1;
wire    [31:0]  HWDATA_P1;
wire            HWRITE_P1;
wire            HREADY_P1;
wire            HREADYOUT_P1;
wire    [31:0]  HRDATA_P1;
wire            HRESP_P1;

wire            HSEL_P2;
wire    [31:0]  HADDR_P2;
wire    [2:0]   HBURST_P2;
wire            HMASTLOCK_P2;
wire    [3:0]   HPROT_P2;
wire    [2:0]   HSIZE_P2;
wire    [1:0]   HTRANS_P2;
wire    [31:0]  HWDATA_P2;
wire            HWRITE_P2;
wire            HREADY_P2;
wire            HREADYOUT_P2;
wire    [31:0]  HRDATA_P2;
wire            HRESP_P2;

wire            HSEL_P3;
wire    [31:0]  HADDR_P3;
wire    [2:0]   HBURST_P3;
wire            HMASTLOCK_P3;
wire    [3:0]   HPROT_P3;
wire    [2:0]   HSIZE_P3;
wire    [1:0]   HTRANS_P3;
wire    [31:0]  HWDATA_P3;
wire            HWRITE_P3;
wire            HREADY_P3;
wire            HREADYOUT_P3;
wire    [31:0]  HRDATA_P3;
wire            HRESP_P3;

wire            HSEL_P4;
wire    [31:0]  HADDR_P4;
wire    [2:0]   HBURST_P4;
wire            HMASTLOCK_P4;
wire    [3:0]   HPROT_P4;
wire    [2:0]   HSIZE_P4;
wire    [1:0]   HTRANS_P4;
wire    [31:0]  HWDATA_P4;
wire            HWRITE_P4;
wire            HREADY_P4;
wire            HREADYOUT_P4;
wire    [31:0]  HRDATA_P4;
wire            HRESP_P4;

wire            HSEL_P5;
wire    [31:0]  HADDR_P5;
wire    [2:0]   HBURST_P5;
wire            HMASTLOCK_P5;
wire    [3:0]   HPROT_P5;
wire    [2:0]   HSIZE_P5;
wire    [1:0]   HTRANS_P5;
wire    [31:0]  HWDATA_P5;
wire            HWRITE_P5;
wire            HREADY_P5;
wire            HREADYOUT_P5;
wire    [31:0]  HRDATA_P5;
wire            HRESP_P5;

wire            HSEL_P6;
wire    [31:0]  HADDR_P6;
wire    [2:0]   HBURST_P6;
wire            HMASTLOCK_P6;
wire    [3:0]   HPROT_P6;
wire    [2:0]   HSIZE_P6;
wire    [1:0]   HTRANS_P6;
wire    [31:0]  HWDATA_P6;
wire            HWRITE_P6;
wire            HREADY_P6;
wire            HREADYOUT_P6;
wire    [31:0]  HRDATA_P6;
wire            HRESP_P6;

wire            HSEL_P7;
wire    [31:0]  HADDR_P7;
wire    [2:0]   HBURST_P7;
wire            HMASTLOCK_P7;
wire    [3:0]   HPROT_P7;
wire    [2:0]   HSIZE_P7;
wire    [1:0]   HTRANS_P7;
wire    [31:0]  HWDATA_P7;
wire            HWRITE_P7;
wire            HREADY_P7;
wire            HREADYOUT_P7;
wire    [31:0]  HRDATA_P7;
wire            HRESP_P7;

wire            HSEL_P8;
wire    [31:0]  HADDR_P8;
wire    [2:0]   HBURST_P8;
wire            HMASTLOCK_P8;
wire    [3:0]   HPROT_P8;
wire    [2:0]   HSIZE_P8;
wire    [1:0]   HTRANS_P8;
wire    [31:0]  HWDATA_P8;
wire            HWRITE_P8;
wire            HREADY_P8;
wire            HREADYOUT_P8;
wire    [31:0]  HRDATA_P8;
wire            HRESP_P8;

wire            HSEL_P9;
wire    [31:0]  HADDR_P9;
wire    [2:0]   HBURST_P9;
wire            HMASTLOCK_P9;
wire    [3:0]   HPROT_P9;
wire    [2:0]   HSIZE_P9;
wire    [1:0]   HTRANS_P9;
wire    [31:0]  HWDATA_P9;
wire            HWRITE_P9;
wire            HREADY_P9;
wire            HREADYOUT_P9;
wire    [31:0]  HRDATA_P9;
wire            HRESP_P9;


wire            HSEL_P10;
wire    [31:0]  HADDR_P10;
wire    [2:0]   HBURST_P10;
wire            HMASTLOCK_P10;
wire    [3:0]   HPROT_P10;
wire    [2:0]   HSIZE_P10;
wire    [1:0]   HTRANS_P10;
wire    [31:0]  HWDATA_P10;
wire            HWRITE_P10;
wire            HREADY_P10;
wire            HREADYOUT_P10;
wire    [31:0]  HRDATA_P10;
wire            HRESP_P10;

wire            HSEL_P11;
wire    [31:0]  HADDR_P11;
wire    [2:0]   HBURST_P11;
wire            HMASTLOCK_P11;
wire    [3:0]   HPROT_P11;
wire    [2:0]   HSIZE_P11;
wire    [1:0]   HTRANS_P11;
wire    [31:0]  HWDATA_P11;
wire            HWRITE_P11;
wire            HREADY_P11;
wire            HREADYOUT_P11;
wire    [31:0]  HRDATA_P11;
wire            HRESP_P11;

wire            HSEL_P12;
wire    [31:0]  HADDR_P12;
wire    [2:0]   HBURST_P12;
wire            HMASTLOCK_P12;
wire    [3:0]   HPROT_P12;
wire    [2:0]   HSIZE_P12;
wire    [1:0]   HTRANS_P12;
wire    [31:0]  HWDATA_P12;
wire            HWRITE_P12;
wire            HREADY_P12;
wire            HREADYOUT_P12;
wire    [31:0]  HRDATA_P12;
wire            HRESP_P12;
AHBlite_Interconnect Interconncet(
        .HCLK           (System_clk),
        .HRESETn        (cpuresetn),

        // CORE SIDE
        .HADDR          (HADDR),
        .HTRANS         (HTRANS),
        .HSIZE          (HSIZE),
        .HBURST         (HBURST),
        .HPROT          (HPROT),
        .HMASTLOCK      (HMASTLOCK),
        .HWRITE         (HWRITE),
        .HWDATA         (HWDATA),
        .HRDATA         (HRDATA),
        .HREADY         (HREADY),
        .HRESP          (HRESP),

        // P0
        .HSEL_P0        (HSEL_P0),
        .HADDR_P0       (HADDR_P0),
        .HBURST_P0      (HBURST_P0),
        .HMASTLOCK_P0   (HMASTLOCK_P0),
        .HPROT_P0       (HPROT_P0),
        .HSIZE_P0       (HSIZE_P0),
        .HTRANS_P0      (HTRANS_P0),
        .HWDATA_P0      (HWDATA_P0),
        .HWRITE_P0      (HWRITE_P0),
        .HREADY_P0      (HREADY_P0),
        .HREADYOUT_P0   (HREADYOUT_P0),
        .HRDATA_P0      (HRDATA_P0),
        .HRESP_P0       (HRESP_P0),

        // P1
        .HSEL_P1        (HSEL_P1),
        .HADDR_P1       (HADDR_P1),
        .HBURST_P1      (HBURST_P1),
        .HMASTLOCK_P1   (HMASTLOCK_P1),
        .HPROT_P1       (HPROT_P1),
        .HSIZE_P1       (HSIZE_P1),
        .HTRANS_P1      (HTRANS_P1),
        .HWDATA_P1      (HWDATA_P1),
        .HWRITE_P1      (HWRITE_P1),
        .HREADY_P1      (HREADY_P1),
        .HREADYOUT_P1   (HREADYOUT_P1),
        .HRDATA_P1      (HRDATA_P1),
        .HRESP_P1       (HRESP_P1),

        // P2
        .HSEL_P2        (HSEL_P2),
        .HADDR_P2       (HADDR_P2),
        .HBURST_P2      (HBURST_P2),
        .HMASTLOCK_P2   (HMASTLOCK_P2),
        .HPROT_P2       (HPROT_P2),
        .HSIZE_P2       (HSIZE_P2),
        .HTRANS_P2      (HTRANS_P2),
        .HWDATA_P2      (HWDATA_P2),
        .HWRITE_P2      (HWRITE_P2),
        .HREADY_P2      (HREADY_P2),
        .HREADYOUT_P2   (HREADYOUT_P2),
        .HRDATA_P2      (HRDATA_P2),
        .HRESP_P2       (HRESP_P2),

        // P3
        .HSEL_P3        (HSEL_P3),
        .HADDR_P3       (HADDR_P3),
        .HBURST_P3      (HBURST_P3),
        .HMASTLOCK_P3   (HMASTLOCK_P3),
        .HPROT_P3       (HPROT_P3),
        .HSIZE_P3       (HSIZE_P3),
        .HTRANS_P3      (HTRANS_P3),
        .HWDATA_P3      (HWDATA_P3),
        .HWRITE_P3      (HWRITE_P3),
        .HREADY_P3      (HREADY_P3),
        .HREADYOUT_P3   (HREADYOUT_P3),
        .HRDATA_P3      (HRDATA_P3),
        .HRESP_P3       (HRESP_P3),

        // P4
        .HSEL_P4        (HSEL_P4),
        .HADDR_P4       (HADDR_P4),
        .HBURST_P4      (HBURST_P4),
        .HMASTLOCK_P4   (HMASTLOCK_P4),
        .HPROT_P4       (HPROT_P4),
        .HSIZE_P4       (HSIZE_P4),
        .HTRANS_P4      (HTRANS_P4),
        .HWDATA_P4      (HWDATA_P4),
        .HWRITE_P4      (HWRITE_P4),
        .HREADY_P4      (HREADY_P4),
        .HREADYOUT_P4   (HREADYOUT_P4),
        .HRDATA_P4      (HRDATA_P4),
        .HRESP_P4       (HRESP_P4),

        // P5
        .HSEL_P5        (HSEL_P5),
        .HADDR_P5       (HADDR_P5),
        .HBURST_P5      (HBURST_P5),
        .HMASTLOCK_P5   (HMASTLOCK_P5),
        .HPROT_P5       (HPROT_P5),
        .HSIZE_P5       (HSIZE_P5),
        .HTRANS_P5      (HTRANS_P5),
        .HWDATA_P5      (HWDATA_P5),
        .HWRITE_P5      (HWRITE_P5),
        .HREADY_P5      (HREADY_P5),
        .HREADYOUT_P5   (HREADYOUT_P5),
        .HRDATA_P5      (HRDATA_P5),
        .HRESP_P5       (HRESP_P5),

        // P6
        .HSEL_P6        (HSEL_P6),
        .HADDR_P6       (HADDR_P6),
        .HBURST_P6      (HBURST_P6),
        .HMASTLOCK_P6   (HMASTLOCK_P6),
        .HPROT_P6       (HPROT_P6),
        .HSIZE_P6       (HSIZE_P6),
        .HTRANS_P6      (HTRANS_P6),
        .HWDATA_P6      (HWDATA_P6),
        .HWRITE_P6      (HWRITE_P6),
        .HREADY_P6      (HREADY_P6),
        .HREADYOUT_P6   (HREADYOUT_P6),
        .HRDATA_P6      (HRDATA_P6),
        .HRESP_P6       (HRESP_P6),

        // P7
        .HSEL_P7        (HSEL_P7),
        .HADDR_P7       (HADDR_P7),
        .HBURST_P7      (HBURST_P7),
        .HMASTLOCK_P7   (HMASTLOCK_P7),
        .HPROT_P7       (HPROT_P7),
        .HSIZE_P7       (HSIZE_P7),
        .HTRANS_P7      (HTRANS_P7),
        .HWDATA_P7      (HWDATA_P7),
        .HWRITE_P7      (HWRITE_P7),
        .HREADY_P7      (HREADY_P7),
        .HREADYOUT_P7   (HREADYOUT_P7),
        .HRDATA_P7      (HRDATA_P7),
        .HRESP_P7       (HRESP_P7),

        // P8
        .HSEL_P8        (HSEL_P8),
        .HADDR_P8       (HADDR_P8),
        .HBURST_P8      (HBURST_P8),
        .HMASTLOCK_P8   (HMASTLOCK_P8),
        .HPROT_P8       (HPROT_P8),
        .HSIZE_P8       (HSIZE_P8),
        .HTRANS_P8      (HTRANS_P8),
        .HWDATA_P8      (HWDATA_P8),
        .HWRITE_P8      (HWRITE_P8),
        .HREADY_P8      (HREADY_P8),
        .HREADYOUT_P8   (HREADYOUT_P8),
        .HRDATA_P8      (HRDATA_P8),
        .HRESP_P8       (HRESP_P8),

        // P8
        .HSEL_P9        (HSEL_P9),
        .HADDR_P9       (HADDR_P9),
        .HBURST_P9      (HBURST_P9),
        .HMASTLOCK_P9   (HMASTLOCK_P9),
        .HPROT_P9       (HPROT_P9),
        .HSIZE_P9       (HSIZE_P9),
        .HTRANS_P9      (HTRANS_P9),
        .HWDATA_P9      (HWDATA_P9),
        .HWRITE_P9      (HWRITE_P9),
        .HREADY_P9      (HREADY_P9),
        .HREADYOUT_P9   (HREADYOUT_P9),
        .HRDATA_P9      (HRDATA_P9),
        .HRESP_P9       (HRESP_P9),	

        // P8
        .HSEL_P10        (HSEL_P10),
        .HADDR_P10       (HADDR_P10),
        .HBURST_P10      (HBURST_P10),
        .HMASTLOCK_P10   (HMASTLOCK_P10),
        .HPROT_P10       (HPROT_P10),
        .HSIZE_P10       (HSIZE_P10),
        .HTRANS_P10      (HTRANS_P10),
        .HWDATA_P10      (HWDATA_P10),
        .HWRITE_P10      (HWRITE_P10),
        .HREADY_P10      (HREADY_P10),
        .HREADYOUT_P10   (HREADYOUT_P10),
        .HRDATA_P10      (HRDATA_P10),
        .HRESP_P10       (HRESP_P10),	

        // P11
        .HSEL_P11        (HSEL_P11),
        .HADDR_P11       (HADDR_P11),
        .HBURST_P11      (HBURST_P11),
        .HMASTLOCK_P11   (HMASTLOCK_P11),
        .HPROT_P11       (HPROT_P11),
        .HSIZE_P11       (HSIZE_P11),
        .HTRANS_P11      (HTRANS_P11),
        .HWDATA_P11      (HWDATA_P11),
        .HWRITE_P11      (HWRITE_P11),
        .HREADY_P11      (HREADY_P11),
        .HREADYOUT_P11   (HREADYOUT_P11),
        .HRDATA_P11      (HRDATA_P11),
        .HRESP_P11       (HRESP_P11),	

        // P12
        .HSEL_P12        (HSEL_P12),
        .HADDR_P12       (HADDR_P12),
        .HBURST_P12      (HBURST_P12),
        .HMASTLOCK_P12   (HMASTLOCK_P12),
        .HPROT_P12       (HPROT_P12),
        .HSIZE_P12       (HSIZE_P12),
        .HTRANS_P12      (HTRANS_P12),
        .HWDATA_P12      (HWDATA_P12),
        .HWRITE_P12      (HWRITE_P12),
        .HREADY_P12      (HREADY_P12),
        .HREADYOUT_P12   (HREADYOUT_P12),
        .HRDATA_P12      (HRDATA_P12),
        .HRESP_P12       (HRESP_P12)	
       
);

//------------------------------------------------------------------------------
// AHB RAMCODE
//------------------------------------------------------------------------------

wire [31:0] RAMCODE_RDATA,RAMCODE_WDATA;
wire [11:0] RAMCODE_WADDR;
wire [11:0] RAMCODE_RADDR;
wire [3:0]  RAMCODE_WRITE;

AHBlite_Block_RAM RAMCODE_Interface(
        /* Connect to Interconnect Port 0 */
        .HCLK           (System_clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P0),
        .HADDR          (HADDR_P0),
        .HPROT          (HPROT_P0),
        .HSIZE          (HSIZE_P0),
        .HTRANS         (HTRANS_P0),
        .HWDATA         (HWDATA_P0),
        .HWRITE         (HWRITE_P0),
        .HRDATA         (HRDATA_P0),
        .HREADY         (HREADY_P0),
        .HREADYOUT      (HREADYOUT_P0),
        .HRESP          (HRESP_P0),
        .BRAM_WRADDR    (RAMCODE_WADDR),
        .BRAM_RDADDR    (RAMCODE_RADDR),
        .BRAM_RDATA     (RAMCODE_RDATA),
        .BRAM_WDATA     (RAMCODE_WDATA),
        .BRAM_WRITE     (RAMCODE_WRITE)
        /**********************************/
);

//------------------------------------------------------------------------------
// AHB LED
//------------------------------------------------------------------------------


AHBlite_LED LED_Interface(
        /* Connect to Interconnect Port 2 */
        .HCLK                   (System_clk),
        .HRESETn                (cpuresetn),
        .HSEL                   (HSEL_P2),
        .HADDR                  (HADDR_P2),
        .HPROT                  (HPROT_P2),
        .HSIZE                  (HSIZE_P2),
        .HTRANS                 (HTRANS_P2),
        .HWDATA                 (HWDATA_P2),
        .HWRITE                 (HWRITE_P2),
        .HRDATA                 (HRDATA_P2),
        .HREADY                 (HREADY_P2),
        .HREADYOUT              (HREADYOUT_P2),
        .HRESP                  (HRESP_P2),
        .LED					(LED)
        /**********************************/ 
);

//------------------------------------------------------------------------------
// AHB RAMDATA
//------------------------------------------------------------------------------

wire [31:0] RAMDATA_RDATA;
wire [31:0] RAMDATA_WDATA;
wire [11:0] RAMDATA_WADDR;
wire [11:0] RAMDATA_RADDR;
wire [3:0]  RAMDATA_WRITE;

AHBlite_Block_RAM RAMDATA_Interface(
        /* Connect to Interconnect Port 1 */
        .HCLK           (System_clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P1),
        .HADDR          (HADDR_P1),
        .HPROT          (HPROT_P1),
        .HSIZE          (HSIZE_P1),
        .HTRANS         (HTRANS_P1),
        .HWDATA         (HWDATA_P1),
        .HWRITE         (HWRITE_P1),
        .HRDATA         (HRDATA_P1),
        .HREADY         (HREADY_P1),
        .HREADYOUT      (HREADYOUT_P1),
        .HRESP          (HRESP_P1),
        .BRAM_WRADDR    (RAMDATA_WADDR),
        .BRAM_RDADDR    (RAMDATA_RADDR),
        .BRAM_WDATA     (RAMDATA_WDATA),
        .BRAM_RDATA     (RAMDATA_RDATA),
        .BRAM_WRITE     (RAMDATA_WRITE)
        /**********************************/
);

//------------------------------------------------------------------------------
// AHB UART
//------------------------------------------------------------------------------
wire state_1;
wire [7:0] UART1_RX_data;
wire [7:0] UART1_TX_data;
wire tx1_en;

AHBlite_UART UART_Interface(
        .HCLK           (System_clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P3),
        .HADDR          (HADDR_P3),
        .HPROT          (HPROT_P3),
        .HSIZE          (HSIZE_P3),
        .HTRANS         (HTRANS_P3),
        .HWDATA         (HWDATA_P3),
        .HWRITE         (HWRITE_P3),
        .HRDATA         (HRDATA_P3),
        .HREADY         (HREADY_P3),
        .HREADYOUT      (HREADYOUT_P3),
        .HRESP          (HRESP_P3),
        .UART_RX        (UART1_RX_data),
        .state          (state_1),
        .tx_en          (tx1_en),
        .UART_TX        (UART1_TX_data)
);

//------------------------------------------------------------------------------
// AHB UART2
//------------------------------------------------------------------------------
wire state_2;
wire [7:0] UART2_RX_data;
wire [7:0] UART2_TX_data;
wire tx2_en;

AHBlite_UART UART2_Interface(
        .HCLK           (System_clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P12),
        .HADDR          (HADDR_P12),
        .HPROT          (HPROT_P12),
        .HSIZE          (HSIZE_P12),
        .HTRANS         (HTRANS_P12),
        .HWDATA         (HWDATA_P12),
        .HWRITE         (HWRITE_P12),
        .HRDATA         (HRDATA_P12),
        .HREADY         (HREADY_P12),
        .HREADYOUT      (HREADYOUT_P12),
        .HRESP          (HRESP_P12),
        .UART_RX        (UART2_RX_data),
        .state          (state_2),
        .tx_en          (tx2_en),
        .UART_TX        (UART2_TX_data)
);

//------------------------------------------------------------------------------
// AHB Matrix_Key
//------------------------------------------------------------------------------

AHBlite_Matrix_Key Matrix_Key_Interface(
        // Connect to Interconnect Port 4 
        .HCLK           (clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P4),
        .HADDR          (HADDR_P4),
        .HPROT          (HPROT_P4),
        .HSIZE          (HSIZE_P4),
        .HTRANS         (HTRANS_P4),
        .HWDATA         (HWDATA_P4),
        .HWRITE         (HWRITE_P4),
        .HRDATA         (HRDATA_P4),
        .HREADY         (HREADY_P4),
        .HREADYOUT      (HREADYOUT_P4),
        .HRESP          (HRESP_P4),
        .Row			(Row),
		.Col			(Col)
);

AHBlite_QN8027_IIC QN8027_IIC_Interface(
     
        .HCLK           (System_clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P5),
        .HADDR          (HADDR_P5),
        .HPROT          (HPROT_P5),
        .HSIZE          (HSIZE_P5),
        .HTRANS         (HTRANS_P5),
        .HWDATA         (HWDATA_P5),
        .HWRITE         (HWRITE_P5),
        .HRDATA         (HRDATA_P5),
        .HREADY         (HREADY_P5),
        .HREADYOUT      (HREADYOUT_P5),
        .HRESP          (HRESP_P5),
        .QN_IIC_SCL		(QN_IIC_SCL),
		.QN_IIC_SDA		(QN_IIC_SDA)
		
);

AHBlite_PAJ_IIC PAJ_IIC_Interface(
     
        .HCLK           (System_clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P8),
        .HADDR          (HADDR_P8),
        .HPROT          (HPROT_P8),
        .HSIZE          (HSIZE_P8),
        .HTRANS         (HTRANS_P8),
        .HWDATA         (HWDATA_P8),
        .HWRITE         (HWRITE_P8),
        .HRDATA         (HRDATA_P8),
        .HREADY         (HREADY_P8),
        .HREADYOUT      (HREADYOUT_P8),
        .HRESP          (HRESP_P8),
        .PAJ_IIC_SCL		(PAJ_IIC_SCL),
		.PAJ_IIC_SDA		(PAJ_IIC_SDA)
		
);


AHBlite_SPI SPI_Interface(
        .HCLK           (System_clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P7),
        .HADDR          (HADDR_P7),
        .HPROT          (HPROT_P7),
        .HSIZE          (HSIZE_P7),
        .HTRANS         (HTRANS_P7),
        .HWDATA         (HWDATA_P7),
        .HWRITE         (HWRITE_P7),
        .HRDATA         (HRDATA_P7),
        .HREADY         (HREADY_P7),
        .HREADYOUT      (HREADYOUT_P7),
        .HRESP          (HRESP_P7),
        .SPI_CS			(SPI_CS),
		.SPI_CLK		(SPI_CLK),
		.SPI_MOSI       (SPI_MOSI),
		.SPI_MISO		(SPI_MISO),
		.SPI_IRQ		(SPI_IRQ),
		.spi_interrupt  (spi_interrupt)
		
);
AHBlite_GPIO GPIO_Interface(
        .HCLK           (System_clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P9),
        .HADDR          (HADDR_P9),
        .HPROT          (HPROT_P9),
        .HSIZE          (HSIZE_P9),
        .HTRANS         (HTRANS_P9),
        .HWDATA         (HWDATA_P9),
        .HWRITE         (HWRITE_P9),
        .HRDATA         (HRDATA_P9),
        .HREADY         (HREADY_P9),
        .HREADYOUT      (HREADYOUT_P9),
        .HRESP          (HRESP_P9),
		.IO				(IO)
);
AHBlite_GPIO_READ READ_Interface(
        .HCLK           (System_clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P11),
        .HADDR          (HADDR_P11),
        .HPROT          (HPROT_P11),
        .HSIZE          (HSIZE_P11),
        .HTRANS         (HTRANS_P11),
        .HWDATA         (HWDATA_P11),
        .HWRITE         (HWRITE_P11),
        .HRDATA         (HRDATA_P11),
        .HREADY         (HREADY_P11),
        .HREADYOUT      (HREADYOUT_P11),
        .HRESP          (HRESP_P11),
		.IO_READ				(IO_READ)
);
wire clk_uart1;
wire bps_en1;
wire bps_en1_rx,bps_en1_tx;

assign bps_en1 = bps_en1_rx | bps_en1_tx;

clkuart_pwm clkuart1_pwm(
        .clk(System_clk),
        .RSTn(cpuresetn),
        .clk_uart(clk_uart1),
        .bps_en(bps_en1)
);

UART_RX UART1_RX(
        .clk(System_clk),
        .clk_uart(clk_uart1),
        .RSTn(cpuresetn),
        .RXD(RXD_1),
        .data(UART1_RX_data),
        .interrupt(interrupt_UART1),
        .bps_en(bps_en1_rx)
);

UART_TX UART1_TX(
        .clk(System_clk),
        .clk_uart(clk_uart1),
        .RSTn(cpuresetn),
        .data(UART1_TX_data),
        .tx_en(tx1_en),
        .TXD(TXD_1),
        .state(state_1),
        .bps_en(bps_en1_tx)
);

wire clk_uart2;
wire bps_en2;
wire bps_en2_rx,bps_en2_tx;

assign bps_en2 = bps_en2_rx | bps_en2_tx;

clkuart_pwm_uart2 clkuart2_pwm(
        .clk(System_clk),
        .RSTn(cpuresetn),
        .clk_uart(clk_uart2),
        .bps_en(bps_en2)
);
UART_RX UART2_RX(
        .clk(System_clk),
        .clk_uart(clk_uart2),
        .RSTn(cpuresetn),
        .RXD(RXD_2),
        .data(UART2_RX_data),
        .interrupt(interrupt_UART2),
        .bps_en(bps_en2_rx)
);

UART_TX UART2_TX(
        .clk(System_clk),
        .clk_uart(clk_uart2),
        .RSTn(cpuresetn),
        .data(UART2_TX_data),
        .tx_en(tx2_en),
        .TXD(TXD_2),
        .state(state_2),
        .bps_en(bps_en2_tx)
);
//------------------------------------------------------------------------------
// RAM
//------------------------------------------------------------------------------

Block_RAM RAM_CODE(
        .clka           (System_clk),
        .addra          (RAMCODE_WADDR),
        .addrb          (RAMCODE_RADDR),
        .dina           (RAMCODE_WDATA),
        .doutb          (RAMCODE_RDATA),
        .wea            (RAMCODE_WRITE)
);

Block_RAM RAM_DATA(
        .clka           (System_clk),
        .addra          (RAMDATA_WADDR),
        .addrb          (RAMDATA_RADDR),
        .dina           (RAMDATA_WDATA),
        .doutb          (RAMDATA_RDATA),
        .wea            (RAMDATA_WRITE)
);


//------------------------------------------------------------------------------
// AHB LCD
//------------------------------------------------------------------------------

    wire            LCD_CS_run;
    wire            LCD_RS_run;
    wire            LCD_WR_run;
    wire            LCD_RD_run;
    wire            LCD_RST_run;
    wire   [15:0]   LCD_DATA_run;
    wire            LCD_BL_CTR_run;

    wire     LCD_INI_en;
    wire     LCD_MODE;



    AHBlite_LCD LCD_Interface(
            .HCLK           (System_clk),
            .HRESETn        (cpuresetn),
            .HSEL           (HSEL_P10),
            .HADDR          (HADDR_P10),
            .HTRANS         (HTRANS_P10),
            .HSIZE          (HSIZE_P10),
            .HPROT          (HPROT_P10),
            .HWRITE         (HWRITE_P10),
            .HWDATA         (HWDATA_P10),
            .HREADY         (HREADY_P10),
            .HREADYOUT      (HREADYOUT_P10),

            .LCD_CS         (LCD_CS_run),
            .LCD_RS         (LCD_RS_run),
            .LCD_WR         (LCD_WR_run),
            .LCD_RD         (LCD_RD_run),
            .LCD_RST        (LCD_RST_run),
            .LCD_DATA       (LCD_DATA_run),
            .LCD_BL_CTR     (LCD_BL_CTR_run),
			.led_test (led_test),
            .LCD_INI_en     (LCD_INI_en),
            .LCD_MODE       (LCD_MODE)
    );

//------------------------------------------------------------------------------
// LCD
//------------------------------------------------------------------------------
assign led_test_mode      = ( LCD_MODE == 1'b1 ) ? 1'b1     : 1'b0     ;
    wire            LCD_CS_INI;
    wire            LCD_RS_INI;
    wire            LCD_WR_INI;
    wire            LCD_RD_INI;
    wire            LCD_RST_INI;
    wire   [15:0]   LCD_DATA_INI;
    wire            LCD_BL_CTR_INI;

    LCD_INI LCD_INI(
            .clk             (System_clk)
            ,.rstn            (cpuresetn)
            ,.en              (LCD_INI_en)
            ,.Initial_finish  (LCD_INI_FINISH)
            ,.LCD_CS          (LCD_CS_INI)
            ,.LCD_RST         (LCD_RST_INI)
            ,.LCD_RS          (LCD_RS_INI)
            ,.LCD_WR          (LCD_WR_INI)
            ,.LCD_RD          (LCD_RD_INI)
            ,.LCD_DATA        (LCD_DATA_INI)
            ,.LCD_BL_CTR      (LCD_BL_CTR_INI)
    );
	assign led_test_mode      = ( LCD_MODE == 1'b1 ) ? 1'b1     : 1'b0     ;
    assign LCD_CS      = ( LCD_MODE == 1'b1 ) ? LCD_CS_run     : LCD_CS_INI     ;
    assign LCD_RST     = ( LCD_MODE == 1'b1 ) ? LCD_RST_run    : LCD_RST_INI    ;
    assign LCD_RS      = ( LCD_MODE == 1'b1 ) ? LCD_RS_run     : LCD_RS_INI     ;
    assign LCD_WR      = ( LCD_MODE == 1'b1 ) ? LCD_WR_run     : LCD_WR_INI     ;
    assign LCD_RD      = ( LCD_MODE == 1'b1 ) ? LCD_RD_run     : LCD_RD_INI     ;
    assign LCD_DATA    = ( LCD_MODE == 1'b1 ) ? LCD_DATA_run   : LCD_DATA_INI   ;
    assign LCD_BL_CTR  = ( LCD_MODE == 1'b1 ) ? LCD_BL_CTR_run : LCD_BL_CTR_INI ;
endmodule
