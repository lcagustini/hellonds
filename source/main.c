#ifndef NDS
#define NDS
#include <stdio.h>
#include <nds.h>
#endif

#include "object.h"
#include "background.h"

#include <zapdos.h>
#include <grass.h>

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define TRUE 1
#define FALSE 0

int main(void){
    u16 Pressed;
    u16 Held;

    consoleDemoInit();
    consoleClear();

    //Top Screen Sprite Setup
    videoSetMode(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    vramSetBankE(VRAM_E_LCD);
    vramSetBankF(VRAM_F_BG_EXT_PALETTE_SLOT01);
    vramSetBankG(VRAM_G_SPRITE_EXT_PALETTE);
    vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
    vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);

    oamInit(&oamMain, SpriteMapping_1D_128, false);

    //Bottom Screen Sprite Setup
    /*videoSetModeSub(MODE_0_2D);
      vramSetBankD(VRAM_D_SUB_SPRITE);
      oamInit(&oamSub, SpriteMapping_1D_32, false);
      */

    Object s = newObject(0, 0, 1, &oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
    Background b = newBackground(0, BgType_Text4bpp, BgSize_T_512x512, 0, 1);
    setBackgroundGfx(&b, grassTiles, grassTilesLen, grassMap, grassMapLen, grassPal, grassPalLen);

    timerStart(0, ClockDivider_1024, 0, NULL);
    touchPosition* t = (touchPosition*) malloc(sizeof(touchPosition*));

    DC_FlushRange(zapdosPal, zapdosPalLen);
    dmaCopy(zapdosPal, SPRITE_PALETTE, zapdosPalLen);

    DC_FlushRange(b.tiles, b.tilesLen);
    dmaCopyHalfWordsAsynch(0, b.tiles, bgGetGfxPtr(b.id), b.tilesLen);
    DC_FlushRange(b.map, b.mapLen);
    dmaCopyHalfWordsAsynch(1, b.map, bgGetMapPtr(b.id), b.mapLen);
    DC_FlushRange(b.pal, b.palLen);
    dmaCopyHalfWordsAsynch(2, b.pal, BG_PALETTE, b.palLen);

    u16 dt;
    while(1){
        scanKeys();
        Pressed = keysDown();
        Held = keysHeld();

        touchRead(t);

        dt = timerElapsed(0);

        DC_FlushRange(zapdosTiles, 32*32);
        dmaCopy(zapdosTiles + s.direction*16*16, s.gfx, 32*32);
        updateObject(s);

        // handle walking
        // TODO: handle walking by scrolling the background instead of moving the player
        //       (depending on where the player is)
        walk(&s, Held);

        // this is how to scroll the background
        //bgSetScroll(bgID, 16, 16);
        //bgUpdate();

        // debug
        {
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
        }

        swiWaitForVBlank();
        oamUpdate(&oamSub);
        oamUpdate(&oamMain);
    }

    return 0;
}
