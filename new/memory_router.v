`timescale 1ns / 1ps
module memory_router(
    input wire clk,
    input wire [31:0] read_data_2,
    input wire mem_read, mem_write,
    input wire btn_left, 
    input wire btn_fire, 
    input wire btn_right, 
    input wire [31:0] ram_read_data,
    input wire [31:0] address,
    output reg [31:0] px_reg,
    output reg [31:0] py_reg,
    output reg [31:0] bullet_px_reg,
    output reg [31:0] bullet_py_reg,
    output reg [31:0] read_data,
    output reg ram_we,
    output reg [31:0] ram_address
    );
    
    wire [31:0] btn_wire;
    assign btn_wire = {29'b0, btn_left, btn_fire, btn_right};
    //sw rs2, offset(rs1)
    //lw rd, offset(rs1)

    initial begin
        px_reg = 32'd320;
        py_reg = 32'd430; //was 300 before
        bullet_px_reg = 32'd320;
        bullet_py_reg = 32'd430;
    end
    //sw
    always @(posedge clk) begin
        if (mem_write) begin
            case (address[31:28])
                //no 4'h1 here
                //px_reg
                4'h2 : px_reg <= read_data_2;
                //py_reg
                4'h3 : py_reg <= read_data_2;   
                //bullet_px_reg
                4'h4 : bullet_px_reg <= read_data_2;
                //bullet_py_reg
                4'h5 : bullet_py_reg <= read_data_2;
                default : begin
                //0x0000000
                end
            endcase
        end
    end
    //lw for px/py and buttons
    always @(*) begin
        read_data  = 32'b0;
        ram_we = 1'b0;
        ram_address = address;
        if (mem_read) begin
            case (address[31:28])
                //buttons
                4'h1 : read_data = btn_wire;
                //px_reg
                4'h2 : read_data = px_reg;
                //py_reg
                4'h3 : read_data = py_reg; 
                //bullet_px_reg
                4'h4 : read_data = bullet_px_reg; 
                //bullet_py_reg
                4'h5 : read_data = bullet_py_reg; 
                default : begin //0x0
                    read_data = ram_read_data;
                end
            endcase
        end
        if (mem_write && address[31:28] == 4'h0) begin
            ram_we = 1'b1;
        end
    end
endmodule
