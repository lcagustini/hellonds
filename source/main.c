#include <stdio.h>
#include <nds.h>

int main(void){
    consoleDemoInit();
    consoleClear();
    //setBrightness(3, 0);

    u16 Pressed;
    u16 Held;
    u16 Released;

    while(1){
        scanKeys();
        Pressed = keysDown();
        Held = keysHeld();
        Released = keysUp();

        if(KEY_TOUCH & Pressed){
            iprintf("\n \n \n Tap! -- ");
            u16 index = 0b1000000000000000;
            for(u8 bit = 16; bit >= 1; bit--){
                if (Pressed & index) iprintf("1");
                else iprintf("0");
                index >>= 1;
            }
            iprintf("\n");
        }

        if(KEY_A & Pressed){
            consoleClear();
        }

        swiWaitForVBlank();
    }

    return 0;
}
