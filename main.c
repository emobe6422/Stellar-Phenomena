#include <stdint.h>

//bit masks
#define BTN_RIGHT 0x00000001
#define BTN_FIRE  0x00000002
#define BTN_LEFT  0x00000004

int main () {
    //buttons: left, right, fire
    volatile uint32_t* btn_reg = (volatile uint32_t*)0x10000000;
    //player
    volatile uint32_t* px_reg = (volatile uint32_t*)0x20000000;
    volatile uint32_t* py_reg = (volatile uint32_t*)0x30000000;
    //bullet
    volatile uint32_t* bullet_px_reg = (volatile uint32_t*)0x40000000;
    volatile uint32_t* bullet_py_reg = (volatile uint32_t*)0x50000000;
    //aliens array
    /* 32 bit register will hold a 32 bit number (2 msb's dont matter) 
     * for the 30 aliens where each bit will be 1 or 0 for alive or dead
     *
     * Tracks the upper leftmost bit of the upper leftmost alien. Everything
     * is relative to that pixel
     */
    volatile uint32_t* alien_grid_px_reg = (volatile uint32_t*)0x60000000;
    volatile uint32_t* alien_grid_py_reg = (volatile uint32_t*)0x70000000; 
    volatile uint32_t* alien_status = (volatile uint32_t*)0x80000000; 
    volatile uint32_t* alien_bullet_px_reg_one = (volatile uint32_t*)0x90000000;
    volatile uint32_t* alien_bullet_py_reg_one = (volatile uint32_t*)0x11000000;
    volatile uint32_t* alien_bullet_px_reg_two = (volatile uint32_t*)0x12000000;
    volatile uint32_t* alien_bullet_py_reg_two = (volatile uint32_t*)0x13000000;
    
    /*
     * Store the position of 1 alien and then everything else is jsut relative to that
     */
 
    //initial player coordinates
    uint32_t player_x = 320;
    const uint32_t player_y = 430;
    *px_reg = player_x;
    *py_reg = player_y;
    //initial bullet coordinates  
    uint32_t bullet_x;
    uint32_t bullet_y = 500;  // start off screen
    uint8_t bullet_active = 0;
    *bullet_py_reg = bullet_y;     // initialize off screen
    //initial alien bullet one position
    uint32_t alien_bullet_one_x;
    uint32_t alien_bullet_one_y = 600;  // start off screen
    //initial alien bullet two position
    uint32_t alien_bullet_two_x;
    uint32_t alien_bullet_two_y = 700;  // start off screen

    //"frames"
    const uint32_t player_division_rate = 200000;
    const uint32_t bullet_division_rate = 50000;
    const uint32_t alien_bullet_division_rate = 50000;
    const uint32_t alien_division_rate = 600000;
    const uint32_t alien_bullet_division_rate = 100000;

    //counters
    uint32_t player_counter = 0; 
    uint32_t bullet_counter = 0; 
    uint32_t alien_counter = 0; 
    uint32_t alien_bullet_counter = 0; 


    //I STOPPED HERE then started working on designs for the other 2 aliens




    /* alien/angel logic */
    uint32_t alien_x = 170;
    uint32_t alien_y = 40;
    *alien_grid_px_reg = alien_x;
    *alien_grid_py_reg = alien_y;
    //random stuff
    uint32_t aliens_alive = 0xFFFFFFFF; //bit mask. Each 1 represents a live alien
    *alien_status = aliens_alive;
    uint8_t direction = 0; //0 right 1 left
    while (1) {
        /*             player logic            */
        if (player_counter >= player_division_rate) {
            if (*btn_reg & BTN_LEFT) {
                if (*px_reg >= 170) {
                    *px_reg = --player_x;
                }
            } 
            if (*btn_reg & BTN_RIGHT) {
                if (*px_reg <= 439) {
                    *px_reg = ++player_x;
                }
            }
            if ((*btn_reg & BTN_FIRE) && !bullet_active) {
                bullet_x = player_x;
                bullet_y = player_y;
                *bullet_px_reg = bullet_x;
                *bullet_py_reg = bullet_y;
                bullet_active = 1;
            }
            player_counter = 0;
        } else {
            player_counter++;
        }

        /*                bullet logic             */
        if (bullet_counter >= bullet_division_rate) {
            if (bullet_active) {
                if (bullet_y == 0) {
                    bullet_active = 0;
                    *bullet_py_reg = 500;  // move off screen
                } else {
                    *bullet_py_reg = --bullet_y;
                }
            }
            bullet_counter = 0;
        } else {
            bullet_counter++;
        }
        /*              alien logic              */
        if (alien_counter >= alien_division_rate) {
        // Move in the current direction (0 = Right, 1 = Left)
            if (direction == 0) {
                alien_x++;
            } else {
                alien_x--;
            }
            if (alien_x >= 250) {
                direction = 1; // Reached right wall -> start moving left
            } else if (alien_x <= 180) {
                direction = 0; // Reached left wall -> start moving right
            }

            *alien_grid_px_reg = alien_x;
            uint16_t increment_1 = 0;
            uint16_t increment_2 = 0;
            for (uint8_t i = 0; i < 3; i++) {
                increment_1 = 0;  // reset for each new row
                for (uint8_t j = 0; j < 4; j++) {
                    uint8_t bit_index = i * 4 + j;
                    if (bullet_active && 
                        (*bullet_py_reg >= *alien_grid_py_reg + increment_2) &&
                        (*bullet_py_reg <= *alien_grid_py_reg + 31 + increment_2) &&
                        ((aliens_alive >> bit_index) & 1) &&
                        (*bullet_px_reg >= *alien_grid_px_reg - 32 + increment_1 + 16) &&
                        (*bullet_px_reg < *alien_grid_px_reg + increment_1 + 16)) {
                        aliens_alive &= ~(1 << bit_index);
                        *alien_status = aliens_alive;
                        bullet_active = 0;
                        *bullet_py_reg = 500;
                    }
                    increment_1 += 64;
                }
                increment_2 += 64;
            }

            alien_counter = 0;
        } else {
            alien_counter++;
            //if an alien is dead it cannot shoot
        }
    }
    return (0);
}
