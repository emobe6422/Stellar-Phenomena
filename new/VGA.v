`timescale 1ns / 1ps
module VGA(
    input wire clk,
    input wire reset,
    input wire [9:0] px_reg,
    input wire [9:0] py_reg,
    output wire hsync,
    output wire vsync,
    output wire [3:0] vga_r,
    output wire [3:0] vga_g,
    output wire [3:0] vga_b
    );
    wire clk_out1;
    wire locked;
    wire video_on;
    wire [9:0] px, py;
    
    VGA_25MHz VGA_25MHz( 
        .clk_in1(clk),
        .reset(reset),
        .clk_out1(clk_out1),
        .locked(locked)
    );
    
    wire rst;
    assign rst = reset | ~locked;
    
    VGA_sync VGA_sync(
        .clk(clk_out1),
        .reset(rst),
        .hsync(hsync),
        .vsync(vsync),
        .video_on(video_on),
        .px(px),
        .py(py)
    );
    
    pixel_gen_circuit pixel_gen_circuit (
        .clk(clk_out1),
        .reset(rst),
        .px(px),
        .py(py),
        .px_reg(px_reg),
        .py_reg(py_reg),
        .video_on(video_on),
        .vga_r(vga_r),
        .vga_g(vga_g),
        .vga_b(vga_b)
    );
endmodule