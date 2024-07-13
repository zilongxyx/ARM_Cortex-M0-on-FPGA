`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/05/06 16:25:51
// Design Name: 
// Module Name: buzzermusic
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


//顶层文件
module buzzermusic(
    input           clk,
    input           rstn,
    input    [3:0]  col,
    input           en1,
    input           en2,
    input           en3,
    output   [3:0]  row,
    output          beep
);

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

bzmusic_ctrl bzmusic_ctrl(
    .clk             (clk)
    ,.en             (en)
    ,.rstn           (rstn)
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
     .clk          (clk)
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
    ,.clk(clk)
    ,.addr_i(addr)
    ,.data_o(data1)
);

wire [11:0] data2;
BlockROM2 #(
     .ADDR_WIDTH(8)
    ,.DATA_WIDTH(12)
) BlockROM2 (
     .en(en2)
    ,.clk(clk)
    ,.addr_i(addr)
    ,.data_o(data2)
);

wire [11:0] data3;
BlockROM3 #(
     .ADDR_WIDTH(8)
    ,.DATA_WIDTH(12)
) BlockROM3 (
     .en(en3)
    ,.clk(clk)
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
     .clk            (clk)
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
     .clk                 (clk)
    ,.en                  (beat_cnt_en)
    ,.rstn                (beat_cnt_rstn)
    ,.beat_cnt_parameter  (beat_cnt_parameter)
    ,.beat_finish         (beat_finish)   
);

endmodule