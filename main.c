#include <stdint.h>

//bit masks
#define BTN_RIGHT 0x00000001
#define BTN_FIRE  0x00000002
#define BTN_LEFT  0x00000004

int main () {

    volatile uint32_t* btn_reg = (volatile uint32_t*)0x10000000;
    volatile uint32_t* px_reg = (volatile uint32_t*)0x20000000;
    volatile uint32_t* py_reg = (volatile uint32_t*)0x30000000;

    uint32_t player_x = 320;
    const uint32_t player_y = 430;
    //change to byte later using typedef
    uint32_t counter = 0; //
    const uint32_t division_rate = 1000000; // one million
    
    *px_reg = player_x;
    *py_reg = player_y;
    //the vlaues in button_reg are received from the physical button
    while (1) {
        //bit masks used so when we add shooting, we can move and shoot at the same time
          //If we added == instead of &, that would evaluate the entire value
        if (counter >= division_rate) {
            if (*btn_reg & BTN_LEFT) {
                //move left 1 pixel
                //if (player_x >= 0) {
                    *px_reg = --player_x;
                //}
            } 
            if (*btn_reg & BTN_RIGHT) {
                //move right 1 pixel
                //if (player_x <= 639) { temporary removal
                    *px_reg = ++player_x;
                    //After we surpass 639, we can no longer move
                //}
            }
            counter = 0;
        } else {
            counter++;
        }
    }
    return (0);
}
