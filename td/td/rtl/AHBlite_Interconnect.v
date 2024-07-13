module AHBlite_Interconnect(

    // CLK & RST
    input   wire    HCLK,
    input   wire    HRESETn,

    // CORE SIDE
    input   wire    [31:0]  HADDR,
    input   wire    [2:0]   HBURST,
    input   wire            HMASTLOCK,
    input   wire    [3:0]   HPROT,
    input   wire    [2:0]   HSIZE,
    input   wire    [1:0]   HTRANS,
    input   wire    [31:0]  HWDATA,
    input   wire            HWRITE,
    output  wire            HREADY,
    output  wire    [31:0]  HRDATA,
    output  wire            HRESP,

    // Peripheral 0
    output  wire            HSEL_P0,
    output  wire    [31:0]  HADDR_P0,
    output  wire    [2:0]   HBURST_P0,
    output  wire            HMASTLOCK_P0,
    output  wire    [3:0]   HPROT_P0,
    output  wire    [2:0]   HSIZE_P0,
    output  wire    [1:0]   HTRANS_P0,
    output  wire    [31:0]  HWDATA_P0,
    output  wire            HWRITE_P0,
    output  wire            HREADY_P0,
    input   wire            HREADYOUT_P0,
    input   wire    [31:0]  HRDATA_P0,
    input   wire            HRESP_P0,

    // Peripheral 1
    output  wire            HSEL_P1,
    output  wire    [31:0]  HADDR_P1,
    output  wire    [2:0]   HBURST_P1,
    output  wire            HMASTLOCK_P1,
    output  wire    [3:0]   HPROT_P1,
    output  wire    [2:0]   HSIZE_P1,
    output  wire    [1:0]   HTRANS_P1,
    output  wire    [31:0]  HWDATA_P1,
    output  wire            HWRITE_P1,
    output  wire            HREADY_P1,
    input   wire            HREADYOUT_P1,
    input   wire    [31:0]  HRDATA_P1,
    input   wire            HRESP_P1,

    // Peripheral 2
    output  wire            HSEL_P2,
    output  wire    [31:0]  HADDR_P2,
    output  wire    [2:0]   HBURST_P2,
    output  wire            HMASTLOCK_P2,
    output  wire    [3:0]   HPROT_P2,
    output  wire    [2:0]   HSIZE_P2,
    output  wire    [1:0]   HTRANS_P2,
    output  wire    [31:0]  HWDATA_P2,
    output  wire            HWRITE_P2,
    output  wire            HREADY_P2,
    input   wire            HREADYOUT_P2,
    input   wire    [31:0]  HRDATA_P2,
    input   wire            HRESP_P2,

    // Peripheral 3
    output  wire            HSEL_P3,
    output  wire    [31:0]  HADDR_P3,
    output  wire    [2:0]   HBURST_P3,
    output  wire            HMASTLOCK_P3,
    output  wire    [3:0]   HPROT_P3,
    output  wire    [2:0]   HSIZE_P3,
    output  wire    [1:0]   HTRANS_P3,
    output  wire    [31:0]  HWDATA_P3,
    output  wire            HWRITE_P3,
    output  wire            HREADY_P3,
    input   wire            HREADYOUT_P3,
    input   wire    [31:0]  HRDATA_P3,
    input   wire            HRESP_P3,

// Peripheral 4
    output  wire            HSEL_P4,
    output  wire    [31:0]  HADDR_P4,
    output  wire    [2:0]   HBURST_P4,
    output  wire            HMASTLOCK_P4,
    output  wire    [3:0]   HPROT_P4,
    output  wire    [2:0]   HSIZE_P4,
    output  wire    [1:0]   HTRANS_P4,
    output  wire    [31:0]  HWDATA_P4,
    output  wire            HWRITE_P4,
    output  wire            HREADY_P4,
    input   wire            HREADYOUT_P4,
    input   wire    [31:0]  HRDATA_P4,
    input   wire            HRESP_P4,

    // Peripheral 5
    output  wire            HSEL_P5,
    output  wire    [31:0]  HADDR_P5,
    output  wire    [2:0]   HBURST_P5,
    output  wire            HMASTLOCK_P5,
    output  wire    [3:0]   HPROT_P5,
    output  wire    [2:0]   HSIZE_P5,
    output  wire    [1:0]   HTRANS_P5,
    output  wire    [31:0]  HWDATA_P5,
    output  wire            HWRITE_P5,
    output  wire            HREADY_P5,
    input   wire            HREADYOUT_P5,
    input   wire    [31:0]  HRDATA_P5,
    input   wire            HRESP_P5,
	

    // Peripheral 6
    output  wire            HSEL_P6,
    output  wire    [31:0]  HADDR_P6,
    output  wire    [2:0]   HBURST_P6,
    output  wire            HMASTLOCK_P6,
    output  wire    [3:0]   HPROT_P6,
    output  wire    [2:0]   HSIZE_P6,
    output  wire    [1:0]   HTRANS_P6,
    output  wire    [31:0]  HWDATA_P6,
    output  wire            HWRITE_P6,
    output  wire            HREADY_P6,
    input   wire            HREADYOUT_P6,
    input   wire    [31:0]  HRDATA_P6,
    input   wire            HRESP_P6,

    // Peripheral 7
    output  wire            HSEL_P7,
    output  wire    [31:0]  HADDR_P7,
    output  wire    [2:0]   HBURST_P7,
    output  wire            HMASTLOCK_P7,
    output  wire    [3:0]   HPROT_P7,
    output  wire    [2:0]   HSIZE_P7,
    output  wire    [1:0]   HTRANS_P7,
    output  wire    [31:0]  HWDATA_P7,
    output  wire            HWRITE_P7,
    output  wire            HREADY_P7,
    input   wire            HREADYOUT_P7,
    input   wire    [31:0]  HRDATA_P7,
    input   wire            HRESP_P7,

    // Peripheral 8
    output  wire            HSEL_P8,
    output  wire    [31:0]  HADDR_P8,
    output  wire    [2:0]   HBURST_P8,
    output  wire            HMASTLOCK_P8,
    output  wire    [3:0]   HPROT_P8,
    output  wire    [2:0]   HSIZE_P8,
    output  wire    [1:0]   HTRANS_P8,
    output  wire    [31:0]  HWDATA_P8,
    output  wire            HWRITE_P8,
    output  wire            HREADY_P8,
    input   wire            HREADYOUT_P8,
    input   wire    [31:0]  HRDATA_P8,
    input   wire            HRESP_P8,

    // Peripheral 9
    output  wire            HSEL_P9,
    output  wire    [31:0]  HADDR_P9,
    output  wire    [2:0]   HBURST_P9,
    output  wire            HMASTLOCK_P9,
    output  wire    [3:0]   HPROT_P9,
    output  wire    [2:0]   HSIZE_P9,
    output  wire    [1:0]   HTRANS_P9,
    output  wire    [31:0]  HWDATA_P9,
    output  wire            HWRITE_P9,
    output  wire            HREADY_P9,
    input   wire            HREADYOUT_P9,
    input   wire    [31:0]  HRDATA_P9,
    input   wire            HRESP_P9,


    // Peripheral 9
    output  wire            HSEL_P10,
    output  wire    [31:0]  HADDR_P10,
    output  wire    [2:0]   HBURST_P10,
    output  wire            HMASTLOCK_P10,
    output  wire    [3:0]   HPROT_P10,
    output  wire    [2:0]   HSIZE_P10,
    output  wire    [1:0]   HTRANS_P10,
    output  wire    [31:0]  HWDATA_P10,
    output  wire            HWRITE_P10,
    output  wire            HREADY_P10,
    input   wire            HREADYOUT_P10,
    input   wire    [31:0]  HRDATA_P10,
    input   wire            HRESP_P10,

    // Peripheral 11
    output  wire            HSEL_P11,
    output  wire    [31:0]  HADDR_P11,
    output  wire    [2:0]   HBURST_P11,
    output  wire            HMASTLOCK_P11,
    output  wire    [3:0]   HPROT_P11,
    output  wire    [2:0]   HSIZE_P11,
    output  wire    [1:0]   HTRANS_P11,
    output  wire    [31:0]  HWDATA_P11,
    output  wire            HWRITE_P11,
    output  wire            HREADY_P11,
    input   wire            HREADYOUT_P11,
    input   wire    [31:0]  HRDATA_P11,
    input   wire            HRESP_P11,

    // Peripheral 12
    output  wire            HSEL_P12,
    output  wire    [31:0]  HADDR_P12,
    output  wire    [2:0]   HBURST_P12,
    output  wire            HMASTLOCK_P12,
    output  wire    [3:0]   HPROT_P12,
    output  wire    [2:0]   HSIZE_P12,
    output  wire    [1:0]   HTRANS_P12,
    output  wire    [31:0]  HWDATA_P12,
    output  wire            HWRITE_P12,
    output  wire            HREADY_P12,
    input   wire            HREADYOUT_P12,
    input   wire    [31:0]  HRDATA_P12,
    input   wire            HRESP_P12
);

// Public signals--------------------------------
//-----------------------------------------------

// HADDR
assign  HADDR_P0    =   HADDR;
assign  HADDR_P1    =   HADDR;
assign  HADDR_P2    =   HADDR;
assign  HADDR_P3    =   HADDR;
assign  HADDR_P4    =   HADDR;
assign  HADDR_P5    =   HADDR;
assign  HADDR_P6    =   HADDR;
assign  HADDR_P7    =   HADDR;
assign  HADDR_P8    =   HADDR;
assign  HADDR_P9    =   HADDR;
assign  HADDR_P10    =   HADDR;
assign  HADDR_P11    =   HADDR;
assign  HADDR_P12    =   HADDR;
// HBURST
assign  HBURST_P0   =   HBURST;
assign  HBURST_P1   =   HBURST;
assign  HBURST_P2   =   HBURST;
assign  HBURST_P3   =   HBURST;
assign  HBURST_P4   =   HBURST;
assign  HBURST_P5   =   HBURST;
assign  HBURST_P6   =   HBURST;
assign  HBURST_P7   =   HBURST;
assign  HBURST_P8   =   HBURST;
assign  HBURST_P9   =   HBURST;
assign  HBURST_P10   =   HBURST;
assign  HBURST_P11   =   HBURST;
assign  HBURST_P12   =   HBURST;
// HMASTLOCK
assign HMASTLOCK_P0 =   HMASTLOCK;
assign HMASTLOCK_P1 =   HMASTLOCK;
assign HMASTLOCK_P2 =   HMASTLOCK;
assign HMASTLOCK_P3 =   HMASTLOCK;
assign HMASTLOCK_P4 =   HMASTLOCK;
assign HMASTLOCK_P5 =   HMASTLOCK;
assign HMASTLOCK_P6 =   HMASTLOCK;
assign HMASTLOCK_P7 =   HMASTLOCK;
assign HMASTLOCK_P8 =   HMASTLOCK;
assign HMASTLOCK_P9 =   HMASTLOCK;
assign HMASTLOCK_P10 =   HMASTLOCK;
assign HMASTLOCK_P11 =   HMASTLOCK;
assign HMASTLOCK_P12 =   HMASTLOCK;
// HPROT
assign HPROT_P0     =   HPROT;
assign HPROT_P1     =   HPROT;
assign HPROT_P2     =   HPROT;
assign HPROT_P3     =   HPROT;
assign HPROT_P4     =   HPROT;
assign HPROT_P5     =   HPROT;
assign HPROT_P6     =   HPROT;
assign HPROT_P7     =   HPROT;
assign HPROT_P8     =   HPROT;
assign HPROT_P9     =   HPROT;
assign HPROT_P10     =   HPROT;
assign HPROT_P11     =   HPROT;
assign HPROT_P12     =   HPROT;
// HSIZE
assign HSIZE_P0     =   HSIZE;
assign HSIZE_P1     =   HSIZE;
assign HSIZE_P2     =   HSIZE;
assign HSIZE_P3     =   HSIZE;
assign HSIZE_P4     =   HSIZE;
assign HSIZE_P5     =   HSIZE;
assign HSIZE_P6     =   HSIZE;
assign HSIZE_P7     =   HSIZE;
assign HSIZE_P8     =   HSIZE;
assign HSIZE_P9     =   HSIZE;
assign HSIZE_P10     =   HSIZE;
assign HSIZE_P11     =   HSIZE;
assign HSIZE_P12     =   HSIZE;
// HTRANS
assign HTRANS_P0     =   HTRANS;
assign HTRANS_P1     =   HTRANS;
assign HTRANS_P2     =   HTRANS;
assign HTRANS_P3     =   HTRANS;
assign HTRANS_P4     =   HTRANS;
assign HTRANS_P5     =   HTRANS;
assign HTRANS_P6     =   HTRANS;
assign HTRANS_P7     =   HTRANS;
assign HTRANS_P8     =   HTRANS;
assign HTRANS_P9     =   HTRANS;
assign HTRANS_P10     =   HTRANS;
assign HTRANS_P11     =   HTRANS;
assign HTRANS_P12     =   HTRANS;
// HWDATA
assign HWDATA_P0     =   HWDATA;
assign HWDATA_P1     =   HWDATA;
assign HWDATA_P2     =   HWDATA;
assign HWDATA_P3     =   HWDATA;
assign HWDATA_P4     =   HWDATA;
assign HWDATA_P5     =   HWDATA;
assign HWDATA_P6     =   HWDATA;
assign HWDATA_P7     =   HWDATA;
assign HWDATA_P8     =   HWDATA;
assign HWDATA_P9     =   HWDATA;
assign HWDATA_P10     =   HWDATA;
assign HWDATA_P11     =   HWDATA;
assign HWDATA_P12     =   HWDATA;
// HWRITE
assign HWRITE_P0     =   HWRITE;
assign HWRITE_P1     =   HWRITE;
assign HWRITE_P2     =   HWRITE;
assign HWRITE_P3     =   HWRITE;
assign HWRITE_P4     =   HWRITE;
assign HWRITE_P5     =   HWRITE;
assign HWRITE_P6     =   HWRITE;
assign HWRITE_P7     =   HWRITE;
assign HWRITE_P8     =   HWRITE;
assign HWRITE_P9     =   HWRITE;
assign HWRITE_P10     =   HWRITE;
assign HWRITE_P11     =   HWRITE;
assign HWRITE_P12     =   HWRITE;
// HREADY
assign HREADY_P0     =   HREADY;
assign HREADY_P1     =   HREADY;
assign HREADY_P2     =   HREADY;
assign HREADY_P3     =   HREADY;
assign HREADY_P4     =   HREADY;
assign HREADY_P5     =   HREADY;
assign HREADY_P6     =   HREADY;
assign HREADY_P7     =   HREADY;
assign HREADY_P8     =   HREADY;
assign HREADY_P9     =   HREADY;
assign HREADY_P10     =   HREADY;
assign HREADY_P11     =   HREADY;
assign HREADY_P12     =   HREADY;
// Decoder---------------------------------------
//-----------------------------------------------

AHBlite_Decoder Decoder(
    .HADDR      (HADDR),
    .P0_HSEL    (HSEL_P0),
    .P1_HSEL    (HSEL_P1),
    .P2_HSEL    (HSEL_P2),
    .P3_HSEL    (HSEL_P3),
    .P4_HSEL    (HSEL_P4), 
    .P5_HSEL    (HSEL_P5),
    .P6_HSEL    (HSEL_P6), 
    .P7_HSEL    (HSEL_P7),
	.P8_HSEL    (HSEL_P8),
	.P9_HSEL    (HSEL_P9),
	.P10_HSEL    (HSEL_P10),
	.P11_HSEL    (HSEL_P11),
	.P12_HSEL    (HSEL_P12)

);

// Slave MUX-------------------------------------
//-----------------------------------------------
AHBlite_SlaveMUX SlaveMUX(

    // CLOCK & RST
    .HCLK           (HCLK),
    .HRESETn        (HRESETn),
    .HREADY         (HREADY),

    //P0
    .P0_HSEL        (HSEL_P0),
    .P0_HREADYOUT   (HREADYOUT_P0),
    .P0_HRESP       (HRESP_P0),
    .P0_HRDATA      (HRDATA_P0),

    //P1
    .P1_HSEL        (HSEL_P1),
    .P1_HREADYOUT   (HREADYOUT_P1),
    .P1_HRESP       (HRESP_P1),
    .P1_HRDATA      (HRDATA_P1),

    //P2
    .P2_HSEL        (HSEL_P2),
    .P2_HREADYOUT   (HREADYOUT_P2),
    .P2_HRESP       (HRESP_P2),
    .P2_HRDATA      (HRDATA_P2),

    //P3
    .P3_HSEL        (HSEL_P3),
    .P3_HREADYOUT   (HREADYOUT_P3),
    .P3_HRESP       (HRESP_P3),
    .P3_HRDATA      (HRDATA_P3),

    //P4
    .P4_HSEL        (HSEL_P4),
    .P4_HREADYOUT   (HREADYOUT_P4),
    .P4_HRESP       (HRESP_P4),
    .P4_HRDATA      (HRDATA_P4),
	
    //P5
    .P5_HSEL        (HSEL_P5),
    .P5_HREADYOUT   (HREADYOUT_P5),
    .P5_HRESP       (HRESP_P5),
    .P5_HRDATA      (HRDATA_P5),

    //P6
    .P6_HSEL        (HSEL_P6),
    .P6_HREADYOUT   (HREADYOUT_P6),
    .P6_HRESP       (HRESP_P6),
    .P6_HRDATA      (HRDATA_P6),

    //P7
    .P7_HSEL        (HSEL_P7),
    .P7_HREADYOUT   (HREADYOUT_P7),
    .P7_HRESP       (HRESP_P7),
    .P7_HRDATA      (HRDATA_P7),

    //P8
    .P8_HSEL        (HSEL_P8),
    .P8_HREADYOUT   (HREADYOUT_P8),
    .P8_HRESP       (HRESP_P8),
    .P8_HRDATA      (HRDATA_P8),
    //P9
    .P9_HSEL        (HSEL_P9),
    .P9_HREADYOUT   (HREADYOUT_P9),
    .P9_HRESP       (HRESP_P9),
    .P9_HRDATA      (HRDATA_P9),

    //P10
    .P10_HSEL        (HSEL_P10),
    .P10_HREADYOUT   (HREADYOUT_P10),
    .P10_HRESP       (HRESP_P10),
    .P10_HRDATA      (HRDATA_P10),
    //P11
    .P11_HSEL        (HSEL_P11),
    .P11_HREADYOUT   (HREADYOUT_P11),
    .P11_HRESP       (HRESP_P11),
    .P11_HRDATA      (HRDATA_P11),

    //P12
    .P12_HSEL        (HSEL_P12),
    .P12_HREADYOUT   (HREADYOUT_P12),
    .P12_HRESP       (HRESP_P12),
    .P12_HRDATA      (HRDATA_P12),

    .HREADYOUT      (HREADY),
    .HRESP          (HRESP),
    .HRDATA         (HRDATA)

);

endmodule