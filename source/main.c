#include <stdio.h>
#include <nds.h>

void createSquare(int x, int y, OamState* screen, void* gfx, u16 color){
    dmaFillHalfWords(color, gfx, 16*16*2); // this is how to assign the color fill to the oam gfx
    oamSet(screen, // which display
        1, // the oam entry to set
        x, y, // x & y location
        0, // priority
        15, // palette for 16 color sprite or alpha for bmp sprite
        SpriteSize_16x16, // size
        SpriteColorFormat_Bmp, // color type
        gfx, // the oam gfx
        -1, //affine index
        true, //double the size of rotated sprites
        false, //don't hide the sprite
        false, false, //vflip, hflip
        false //apply mosaic
        );
}

int main(void){
    u16 Pressed;
    u16 Held;
    u16 Released;

    consoleDemoInit();
    consoleClear();

    u16* mainGFX = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp); // top screen square
    //u16* subGFX = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_Bmp); // bottom screen square

    //Top Screen Sprite Setup
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    oamInit(&oamMain, SpriteMapping_1D_32, false);

    //Bottom Screen Sprite Setup
    /*videoSetModeSub(MODE_0_2D);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    oamInit(&oamSub, SpriteMapping_1D_32, false);
    */

    createSquare(0, 0, &oamMain, mainGFX, ARGB16(1, 31, 12, 12));

    while(1){
        scanKeys();
        Pressed = keysDown();
        Held = keysHeld();
        Released = keysUp();

        if(KEY_TOUCH & Pressed){
            iprintf("Tap! -- ");
            u16 index = 0b1000000000000000;
            for(u8 bit = 16; bit >= 1; bit--){
                if (Held & index) iprintf("1");
                else iprintf("0");
                index >>= 1;
            }
            iprintf("\n");
        }

        if(KEY_A & Pressed){
            consoleClear();
        }

        swiWaitForVBlank();
        oamUpdate(&oamSub);
        oamUpdate(&oamMain);
    }

    return 0;
}
