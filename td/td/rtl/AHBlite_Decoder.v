module AHBlite_Decoder
#(
    /*RAMCODE enable parameter*/
    parameter Port0_en = 1,
    /************************/

    /*RAMDATA enable parameter*/
    parameter Port1_en = 1,
    /************************/

    /*LED enable parameter*/
    parameter Port2_en = 1,
    /************************/

    /*UART enable parametersdram_ctr*/
    parameter Port3_en = 1,

	/*key enable parametersdram_ctr*/
	parameter Port4_en = 1,

	/*ADC_IIC enable parametersdram_ctr*/
	parameter Port5_en = 1,
	/*IO enable parametersdram_ctr*/
	parameter Port6_en = 1,
	/*SPI enable parametersdram_ctr*/
	parameter Port7_en = 1,
	/*PAJ_IIC enable parametersdram_ctr*/
	parameter Port8_en = 1,
	/*IO_WRITE enable parametersdram_ctr*/
	parameter Port9_en = 1,
	/*LCD enable parametersdram_ctr*/
	parameter Port10_en = 1,
	/*IO_READ enable parametersdram_ctr*/
	parameter Port11_en = 1,
	/*UART2 enable parametersdram_ctr*/
	parameter Port12_en = 1
    /************************/

    /************************/
)(
    input [31:0] HADDR,

    /*RAMCODE OUTPUT SELECTION SIGNAL*/
    output wire P0_HSEL,

    /*RAMDATA OUTPUT SELECTION SIGNAL*/
    output wire P1_HSEL,

    /*LED OUTPUT SELECTION SIGNAL*/
    output wire P2_HSEL,

    /*UART OUTPUT SELECTION SIGNAL*/
    output wire P3_HSEL,

    /*KEY OUTPUT SELECTION SIGNAL*/
    output wire P4_HSEL,

    /*IIC OUTPUT SELECTION SIGNAL*/
    output wire P5_HSEL,
	/*IO OUTPUT SELECTION SIGNAL*/
	output wire P6_HSEL,
	/*spi OUTPUT SELECTION SIGNAL*/
	output wire P7_HSEL,
	/*PAJ_IIC OUTPUT SELECTION SIGNAL*/
	output wire P8_HSEL,
	output wire P9_HSEL,
	output wire P10_HSEL,
	output wire P11_HSEL,
	output wire P12_HSEL
);

//RAMCODE-----------------------------------

//0x00000000-0x00000400
/*Insert RAMCODE decoder code there*/
assign P0_HSEL = (HADDR[31:16] == 16'h0000) ? Port0_en : 1'b0; 
/***********************************/

//RAMDATA-----------------------------

//0X20000000-0X20000400
/*Insert RAMDATA decoder code there*/
assign P1_HSEL = (HADDR[31:16] == 16'h2000) ? Port1_en : 1'b0; 
/***********************************/

//PERIPHRAL-----------------------------

//0X40000000 LED
/*Insert LED decoder code there*/
assign P2_HSEL = (HADDR[31:16] == 16'h4000) ? Port2_en : 1'b0; 


/***********************************/
//0X40010000 UART RX DATA
//0X40010004 UART TX STATE
//0X40010008 UART TX DATA
/*Insert UART decoder code there*/
assign P3_HSEL = (HADDR[31:16] == 16'h4001) ? Port3_en : 1'b0;
/***********************************/

/*Insert KEY decoder code there*/
assign P4_HSEL = (HADDR[31:16] == 16'h4002) ? Port4_en : 1'b0;

//0X40060000 IIC_SCL
//0X40060004 IIC_SDA
//0X40060008 ACK
/*Insert QN8027 decoder code there*/
assign P5_HSEL = (HADDR[31:16] == 16'h4006) ? Port5_en : 1'b0;
/*Insert IO decoder code there*/
assign P6_HSEL = (HADDR[31:16] == 16'h4003) ? Port6_en : 1'b0;
/*Insert spi decoder code there*/
assign P7_HSEL = (HADDR[31:16] == 16'h4004) ? Port7_en : 1'b0;
/*Insert PAJ_IIC decoder code there*/
assign P8_HSEL = (HADDR[31:16] == 16'h4005) ? Port8_en : 1'b0;
assign P9_HSEL = (HADDR[31:16] == 16'h4009) ? Port9_en : 1'b0;
assign P10_HSEL = (HADDR[31:16] == 16'h4008) ? Port10_en : 1'b0;
assign P11_HSEL = (HADDR[31:16] == 16'h400A) ? Port11_en : 1'b0;
assign P12_HSEL = (HADDR[31:16] == 16'h400F) ? Port12_en : 1'b0;
endmodule