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
    //counter
    uint32_t player_counter = 0; 
    uint32_t bullet_counter = 0; 
    const uint32_t player_division_rate = 300000;
    const uint32_t bullet_division_rate = 50000;

    while (1) {
        /*             player logic            */
        if (player_counter >= player_division_rate) {
            if (*btn_reg & BTN_LEFT) {
                *px_reg = --player_x;
            } 
            if (*btn_reg & BTN_RIGHT) {
                *px_reg = ++player_x;
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

        //bullet logic 
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



    }
    return (0);
}
