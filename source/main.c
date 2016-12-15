#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>

int main(void){
    consoleDemoInit();
    consoleClear();
    //setBrightness(3, 0);

    iprintf("\n Hello World!");

    u16 Pressed;
    u16 Held;
    u16 Released;

    
    NF_Set2D(0, 0);
        
    NF_SetRootFolder("NITROFS");
        
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);

    NF_LoadTiledBg("Background", "Background", 256, 256);
    NF_CreateTiledBg(0, 3, "Background");
    

    while(1){
        scanKeys();
        Pressed = keysDown();
        Held = keysHeld();
        Released = keysUp();

        if(KEY_TOUCH & Pressed)
        {
            u16 temp = Pressed;
            iprintf("\n \n \n Tap! -- ");
            u8 index = 0x01;
            for (u8 bit = 0; bit < 16; bit++)
            {
                iprintf("%i", temp & 0x01);
                temp >>= 1;
            }
            iprintf("\n");
        }

        swiWaitForVBlank();
    }

    return 0;
}
