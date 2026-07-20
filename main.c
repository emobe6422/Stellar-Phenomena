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
    *bullet_py_reg = 500;     // initialize off screen



    //"frames"
    const uint32_t player_division_rate = 300000;
    const uint32_t bullet_division_rate = 50000;
    const uint32_t alien_division_rate = 500000;

    //counters
    uint32_t player_counter = 0; 
    uint32_t bullet_counter = 0; 
    uint32_t alien_counter = 0; 

    /* alien/angel logic */
    uint32_t alien_x = 170;
    uint32_t alien_y = 40;
    *alien_grid_px_reg = alien_x;
    *alien_grid_py_reg = alien_y;
    //random stuff
    uint32_t aliens_alive = 0xFFFFFFFF; //bit mask. Each 1 represents a live alien
    //only concerned with the 5 bits in the lsb F's
    *alien_status = aliens_alive;

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
            //initial alien coordinates
            if (alien_x > 0) {
                alien_x += 1;
                *alien_grid_px_reg = alien_x;
            } 
            uint32_t increment = 0;
            for (uint8_t i = 0; i < 4; i++) {
                if (bullet_active && (*bullet_py_reg <= *alien_grid_py_reg + 31) && (*bullet_px_reg >= *alien_grid_px_reg - 32 + increment + 16) && (*bullet_px_reg < *alien_grid_px_reg + increment + 16)) {
                    aliens_alive &= ~(1 << i);
                    *alien_status = aliens_alive;
                    bullet_active = 0;
                     *bullet_py_reg = 500;  // move off screen
                }
                increment+=64;
            }
            alien_counter = 0;
        } else {
            alien_counter++;
            //if an alien is dead it cannot shoot
            //if a bullet hits an alien it must disappear along with the bullet
        }
    }
    return (0);
}
