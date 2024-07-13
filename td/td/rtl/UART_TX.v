module UART_TX(
    input clk,
    input clk_uart,      //方波 与波特率设置相关
    input RSTn,
    input [7:0] data,    //待发送数据
    input tx_en,         //数据写入FIFO使能 -> 数据有效标志位
    output reg TXD,		 //串口输出
    output wire state,	 //FIFO缓存满标志位 
    output wire bps_en	 //写请求(触发波特率时钟产生)
);

//FIFO 8bit-30depth
wire FIFOrd_en;
wire FIFOwr_en;
wire [7:0] FIFOdata;
wire FIFOempty;
wire FIFOfull;
FIFO FIFO(
    .clock(clk),
    .sclr(RSTn),
    .rdreq(FIFOrd_en),
    .wrreq(FIFOwr_en),
    .full(FIFOfull),
    .empty(FIFOempty),
    .data(data),
    .q(FIFOdata)
);

//FIFO write control
assign FIFOwr_en = (~FIFOfull) & tx_en;

assign state = FIFOfull;

//UART TX 
reg counter_en;     //发送开始使能
reg [3:0] counter;

wire trans_finish;
assign trans_finish = (counter == 4'hb);      //11  开始位+8+停止位

wire trans_start;
assign trans_start = (~FIFOempty) & (~counter_en);

always@(posedge clk or negedge RSTn) begin
    if(~RSTn) counter_en <= 1'b0;
    else if(trans_start) counter_en <= 1'b1;
    else if(trans_finish) counter_en <= 1'b0;
end

always@(posedge clk or negedge RSTn) begin
    if(~RSTn) counter <= 4'h0;
    else if(counter_en) begin 
        if(clk_uart) counter <= counter + 1'b1;
        else if(trans_finish) counter <= 4'h0;
    end
end

assign bps_en = counter_en;   //写请求

wire [9:0] data_formed;

assign data_formed = {1'b1,FIFOdata,1'b0};

always@(posedge clk or negedge RSTn) begin
    if(~RSTn) TXD <= 1'b1;
    else if(counter_en) begin
        if(clk_uart && (counter <= 4'h9)) TXD <= data_formed[counter];
    end else TXD <= 1'b1;
end

//FIFO read control
assign FIFOrd_en = (~FIFOempty) & trans_finish;

endmodule