`timescale 1ns / 1ps
module VGA_sync(
    input wire clk, reset,
    output reg hsync, vsync, video_on,
    output reg [9:0] px, py
    );
    
    localparam total_width  = 799;
    localparam hs_start     = 656;
    localparam hs_end       = 751;
    localparam width        = 639;
    
    localparam total_height = 524;
    localparam vs_start     = 490;
    localparam vs_end       = 491;
    localparam height       = 479;
    
    always @(*) begin
        hsync = ~(px >= hs_start && px <= hs_end);
        vsync = ~(py >= vs_start && py <= vs_end);
        video_on = (px <= width && py <= height);
    end
    
    always @(posedge clk, posedge reset) begin
        if (reset) begin
            px <= 0;
            py <= 0;
        end else if (px == total_width) begin
            px <= 0;
            py <= (py == total_height) ? 0 : py + 1;
        end else begin
            px <= px + 1;
        end
    end
endmodule
