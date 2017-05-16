#ifndef NDS
#define NDS
#include <nds.h>
#endif

#include <stdio.h>

#include "object.h"
#include "background.h"

#include <moltres.h>
#include <zapdos.h>
#include <player_gfx.h>
#include <grass.h>

#define TRUE 1
#define FALSE 0

void setUpScreens(){
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

    oamInit(&oamMain, SpriteMapping_1D_128, true);
}

int main(void){
    u16 Pressed;
    u16 Held;

    powerOn(POWER_ALL);
    consoleDemoInit();
    consoleClear();

    setUpScreens();

    World w = {0};

    CREATE_OBJECT_GFX(player_gfx);
    Object *player = newObject(&w, 0, 0, 1, &oamMain, SpriteSize_16x32, SpriteColorFormat_16Color, &player_gfx);

    Background b = newBackground(0, BgType_Text4bpp, BgSize_T_512x512, 0, 1);
    CREATE_BG_GFX(grass);
    setBackgroundGfx(b, &grass);

    timerStart(0, ClockDivider_1024, 0, NULL);

    touchPosition* t = (touchPosition*) malloc(sizeof(touchPosition*));
    u16 dt;
    while(1){
        scanKeys();
        touchRead(t);

        Pressed = keysDown();
        Held = keysHeld();
        dt = timerElapsed(0);

        // handle walking
        // TODO: handle walking by scrolling the background instead of moving the player
        //       (depending on where the player is)
        walk(&w, player, Held);

        // this is how to scroll the background
        bgSetScroll(b.id, 0, 0);
        bgUpdate();

        updateScreens(&w);

        swiWaitForVBlank();
        oamUpdate(&oamSub);
        oamUpdate(&oamMain);
    }

    return 0;
}
