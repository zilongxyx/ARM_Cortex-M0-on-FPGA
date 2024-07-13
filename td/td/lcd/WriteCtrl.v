module WriteCtrl(input wire clk,rstn,en,
                 input wire data_stop,
                 output reg addr_en,LCD_CS,LCD_WR);

    parameter S0    = 3'b000;
    parameter Wait0 = 3'b001;
    parameter Pre   = 3'b010;
    parameter Wri   = 3'b011;
    parameter Wait1 = 3'b100;
    reg[2:0] curr_state,next_state;

    always @(posedge clk or negedge rstn) begin
        if(!rstn) curr_state <= S0;
        else curr_state <= next_state;
    end

    always @(*) begin
        case(curr_state)
            S0:begin
                if(en) next_state = Pre;
                else next_state = S0;
            end
            Wait0:begin
                next_state = Pre;
            end
            Pre:begin
                next_state = Wri;
            end
            Wri:begin
                next_state = Wait1;
            end
            Wait1:begin
                if(data_stop) next_state = S0;
                else next_state = Wait0;
            end
            default:begin
                next_state = S0;
            end
        endcase
    end

    always @(*) begin
        case(curr_state)
            S0:begin
                addr_en = 1'b0;
                LCD_CS  = 1'b1;
                LCD_WR  = 1'b1;
            end
            Wait0:begin
                addr_en = 1'b0;
                LCD_CS  = 1'b0;
                LCD_WR  = 1'b1;
            end
            Pre:begin
                addr_en = 1'b0;
                LCD_CS  = 1'b0;
                LCD_WR  = 1'b0;
            end
            Wri:begin
                addr_en = 1'b0;
                LCD_CS  = 1'b0;
                LCD_WR  = 1'b1;
            end
            Wait1:begin
                addr_en = 1'b1;
                LCD_CS  = 1'b0;
                LCD_WR  = 1'b1;
            end
            default:begin
                addr_en = 1'b0;
                LCD_CS  = 1'b1;
                LCD_WR  = 1'b1;
            end
        endcase
    end

endmodule