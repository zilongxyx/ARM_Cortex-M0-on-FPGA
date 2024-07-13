module beat_cnt (clk,en,rstn,beat_cnt_parameter,beat_finish);
	input clk,en,rstn;
	input [27:0] beat_cnt_parameter;
	output  beat_finish;
	
	reg [27:0] cnt;
    always@(posedge clk or negedge rstn)
    begin
        if(!rstn)
        cnt <=27'd0;
        else if(en) begin
            if(beat_finish)
            cnt <=27'd0;
            else
            cnt <=cnt +1'd1;
            end
    end
    
    assign beat_finish =(cnt == beat_cnt_parameter)?1'd1:1'd0;

endmodule