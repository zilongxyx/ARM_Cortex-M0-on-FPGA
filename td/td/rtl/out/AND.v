`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/05/08 22:50:14
// Design Name: 
// Module Name: AND
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


module AND(
    input en1,en2,en3,
    input [11:0]data1,data2,data3,
    output reg[11:0] data
    );

    reg [2:0]sel=3'b000;
    always@(en1,en2,en3)
        sel <= {en3,en2,en1};

    always@(data1,data2,data3,sel)
        case(sel) 
            3'b001 : data <= data1;
            3'b010 : data <= data2;
            3'b100 : data <= data3;
            default : data <= 0;
        endcase

endmodule