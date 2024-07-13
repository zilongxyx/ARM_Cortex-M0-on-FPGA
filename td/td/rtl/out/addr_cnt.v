//计数器模块
module addr_cnt (clk,rstn,en,data,addr_finish,addr);
input clk,rstn,en;//
input wire [11:0] data;
output addr_finish,addr;//
wire addr_finish;
reg [8:0] addr;

always@(posedge clk or negedge rstn)
begin
    if(!rstn) 
    addr <=9'd0;
    else if(en)begin
            if(addr_finish)
            addr <=9'd0 ;
        else
            addr <=addr+1'd1;
    end
end   
    assign addr_finish = ~(|data);

endmodule
