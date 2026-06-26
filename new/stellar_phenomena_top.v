`timescale 1ns / 1ps
module stellar_phenomena_top(
    input  wire clk,
    input  wire btn_left,
    input  wire btn_right,
    input  wire btn_fire,
    input  wire reset,
    output wire [3:0] vga_r,
    output wire [3:0] vga_g,
    output wire [3:0] vga_b,
    output wire vga_hs,
    output wire vga_vs
);

    /*        clock buffer        */
    wire clk_buffered;
    BUFG clk_buf(
        .I(clk),
        .O(clk_buffered)
    );

    /*          rv32           */
    wire [31:0] px_reg;
    wire [31:0] py_reg;
    wire [31:0] bullet_px_reg;
    wire [31:0] bullet_py_reg;

    rv32 rv32(
        .clk(clk_buffered),
        .btn_left(btn_left),
        .btn_right(btn_right),
        .btn_fire(btn_fire),
        .px_reg(px_reg),
        .py_reg(py_reg),
        .bullet_px_reg(bullet_px_reg),
        .bullet_py_reg(bullet_py_reg)
    );

    /*          VGA            */
    VGA VGA(
        .clk(clk_buffered),
        .reset(reset),
        .px_reg(px_reg[9:0]),
        .py_reg(py_reg[9:0]),
        .bullet_px_reg(bullet_px_reg[9:0]),
        .bullet_py_reg(bullet_py_reg[9:0]),
        .hsync(vga_hs),
        .vsync(vga_vs),
        .vga_r(vga_r),
        .vga_g(vga_g),
        .vga_b(vga_b)
    );
endmodule