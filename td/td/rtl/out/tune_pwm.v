module tune_pwm (
    input                clk,
    input                en,
    input                rst_n,
    input        [19:0]  pwm_parameter,
    output  reg          clk_pwm
);

reg [19:0] cnt;
always @(posedge clk, negedge rst_n) begin
    if(!rst_n)begin
        cnt <= 20'd0;
        clk_pwm <= 1'b0;
    end
    else begin
        if (en) begin
            if (cnt == pwm_parameter) begin
                cnt <= 20'd0;
            end else begin
                if (cnt == (pwm_parameter>>1)) begin
                    clk_pwm <= ~clk_pwm;
                end else begin
                    clk_pwm <= clk_pwm;
                end
                cnt <= cnt + 1'b1;
            end           
        end else begin
            cnt <= cnt;
            clk_pwm <= clk_pwm;
        end
    end
end

endmodule