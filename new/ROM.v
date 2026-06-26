`timescale 1ns / 1ps

module ROM(
    //no need for clk. Should be immediate
    input wire [9:0] ROM_address,
    output reg [11:0] ROM_hexcode //change size?
    );
    reg [11:0] player_sprite [0:1023];
    initial begin
        $readmemh("C:/Users/Emman/riscv_32i/sprite.hex", player_sprite);
    end
    always @(*) begin
        ROM_hexcode = player_sprite[ROM_address];
    end
endmodule
