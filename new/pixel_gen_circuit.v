`timescale 1ns / 1ps

module pixel_gen_circuit (
    input  wire clk,
    input  wire reset,
    input  wire [9:0] px,
    input  wire [9:0] py,
    input  wire [9:0] px_reg,
    input  wire [9:0] py_reg,
    input  wire [9:0] bullet_px_reg,
    input  wire [9:0] bullet_py_reg,
    input  wire video_on,
    output reg  [3:0] vga_r,
    output reg  [3:0] vga_g,
    output reg  [3:0] vga_b
    );
    
    // sprite offset within bounding box
    wire [4:0] sprite_x = px - px_reg;
    wire [4:0] sprite_y = py - py_reg;
    
    //ROM stuff
    wire [9:0] ROM_address;
    assign ROM_address = sprite_y * 32 + sprite_x;
    //the output
    wire [11:0] ROM_hexcode;
    
    ROM ROM(
        .ROM_address(ROM_address), 
        .ROM_hexcode(ROM_hexcode)
    );
    
    wire player_appears;
    assign player_appears = (px >= px_reg) && (px < px_reg + 32) &&
                       (py >= py_reg) && (py < py_reg + 32);
    wire bullet_wire;
    assign bullet_wire = (px == bullet_px_reg + 16) && (py  == bullet_py_reg);
    
    wire left_border;
    assign left_border = (px >= 0) && (px <= 149) && (py >= 0) && (py <= 479);
    wire right_border;
    assign right_border = (px >= 490) && (px <= 639)  && (py >= 0) && (py <= 479);

    //px, py reg are player position. px and py are just the current pixel's position
    always @(*) begin
        if (!video_on) begin
            vga_r = 4'h0;
            vga_g = 4'h0;
            vga_b = 4'h0;
        end else if (player_appears) begin
            vga_r = ROM_hexcode[11:8];
            vga_g = ROM_hexcode[7:4];
            vga_b = ROM_hexcode[3:0];
        end else if (bullet_wire) begin
            vga_r = 4'hF;
            vga_g = 4'hF;
            vga_b = 4'hF;
        end else if (left_border) begin
            vga_r = 4'h0;
            vga_g = 4'h0;
            vga_b = 4'h4;
        end else if (right_border) begin
            vga_r = 4'h0;
            vga_g = 4'h0;
            vga_b = 4'h4;
        end else begin
            vga_r = 4'h0;
            vga_g = 4'h0;
            vga_b = 4'h0;
        end
    end
endmodule