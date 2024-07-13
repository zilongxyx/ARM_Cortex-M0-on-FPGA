`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/05/06 16:34:51
// Design Name: 
// Module Name: keyboard
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


module key_16(clk,rstn,col,row,key_pulse);
    input clk,rstn;
    input [3:0] col;  
    output reg [3:0] row = 4'b1110;
    output [15:0] key_pulse;

    reg [15:0] key;
    reg [31:0] cnt0 = 0;
    reg scan_clk = 0;
    
    always@(posedge clk) begin
        if(cnt0 == 2499) begin
            cnt0 <= 0;
            scan_clk <= ~scan_clk;
        end
        else
            cnt0 <= cnt0 + 1;
    end
    always@(posedge scan_clk)
        row <= {row[2:0],row[3]}; 
    always@(negedge scan_clk) 
        case(row)
            4'b1110 : key[3:0] <= col;
            4'b1101 : key[7:4] <= col;
            4'b1011 : key[11:8] <= col;
            4'b0111 : key[15:12] <= col;
            default : key <= 0;
        endcase

    //分频计数
    reg [19:0] cnt = 0;
    parameter CNTMAX = 999_999;
    always@(posedge clk or negedge rstn) begin
        if(~rstn)
            cnt <= 0;
        else if(cnt == CNTMAX)
            cnt <= 0;
        else
            cnt <= cnt + 1'b1;
     end
     //每20ms采样一次按键电平

     reg [15:0] key_reg0;
     reg [15:0] key_reg1;
     reg [15:0] key_reg2; 
     always@(posedge clk or negedge rstn) begin
        if(~rstn) begin
            key_reg0 <= 16'hffff;
            key_reg1 <= 16'hffff;
            key_reg2 <= 16'hffff;
        end
        else if(cnt == CNTMAX) begin
            key_reg0 <= key;
            key_reg1 <= key_reg0;
            key_reg2 <= key_reg1;                                
        end
    end

    assign key_pulse = (~key_reg0&~key_reg1& ~key_reg2)|(~key_reg0&~key_reg1&key_reg2);


endmodule