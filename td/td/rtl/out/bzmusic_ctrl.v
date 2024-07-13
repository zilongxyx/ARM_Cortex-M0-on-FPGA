module bzmusic_ctrl (clk,en,rstn,addr_finish,beat_finish,addr_en,addr_rstn,tune_pwm_en,tune_pwm_rstn,beat_cnt_en,beat_cnt_rstn);
input clk,en,rstn;
input addr_finish,beat_finish;
output reg addr_en,addr_rstn;
output reg tune_pwm_en,tune_pwm_rstn;
output reg beat_cnt_en,beat_cnt_rstn;

parameter  S0=2'b00;
parameter  S1=2'b01;
parameter  S2=2'b10;
reg [3:0] state=S1;
reg [3:0] next_state=S1;

always @(en or beat_finish or addr_finish or state) begin 
	case(state)
		S0 :begin
			if (en) begin
				next_state=S1;
			end else begin
				next_state=S0;
			end
		end

		S1 :begin
			if (addr_finish) begin
				next_state=S0;
			end else begin
				next_state=S2;
			end
		end

		S2 :begin
			if (beat_finish) begin
				next_state=S1;
			end else begin
				next_state=S2;
			end
		end
		default:next_state=S0;
		endcase // state
	end


always @(posedge clk or negedge rstn) begin 
	if(~rstn) begin
		state<=S0;
	end else begin
		state<=next_state;		
		end
end

always @(posedge clk ) begin 
	case (next_state)
		S0:begin
			addr_en<=1'b0;
			addr_rstn<=1'b0;
			tune_pwm_en<=1'b0;
			tune_pwm_rstn<=1'b0;
			beat_cnt_en<=1'b0;
			beat_cnt_rstn<=1'b0;
		end

		S1:begin
			addr_en<=1'b1;
			addr_rstn<=1'b1;
			tune_pwm_en<=1'b0;
			tune_pwm_rstn<=1'b0;
			beat_cnt_en<=1'b0;
			beat_cnt_rstn<=1'b0;
		end

		S2:begin
			addr_en<=1'b0;
			addr_rstn<=1'b1;
			tune_pwm_en<=1'b1;
			tune_pwm_rstn<=1'b1;
			beat_cnt_en<=1'b1;
			beat_cnt_rstn<=1'b1;
		end

		default:begin
			addr_en<=1'b0;
			addr_rstn<=1'b0;
			tune_pwm_en<=1'b0;
			tune_pwm_rstn<=1'b0;
			beat_cnt_en<=1'b0;
			beat_cnt_rstn<=1'b0;
		end
	endcase
end


endmodule
