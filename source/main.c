#ifndef NDS
#define NDS
#include <nds.h>
#include "types.h"
#endif

#include <stdio.h>

#include "object.h"
#include "background.h"

#include <player_gfx.h>
#include <grass.h>
#include <center.h>
#include <centerTop.h>

void setUpScreens(){
    videoSetMode(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    vramSetBankE(VRAM_E_BG_EXT_PALETTE);
    vramSetBankF(VRAM_F_LCD);
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

    // pokemon center hitbox
    for (int x = 2; x < 7; x++)
    {
        for (int y = 2; y < 5; y++)
        {
            w.grid[y][x] = 1;
        }
    }

    // rock hitbox
    for (int x = 16; x < 19; x++)
    {
        for (int y = 16; y < 18; y++)
        {
            w.grid[y][x] = 1;
        }
    }

    // world border
    for(int x = 0; x < WORLD_SIZE_TILES; x++){
        for(int y = 0; y < WORLD_SIZE_TILES; y++){
            if(y == 0 || y == WORLD_SIZE_TILES -1 || x == 0 || x == WORLD_SIZE_TILES -1){
                w.grid[y][x] = 1;
            }
        }
    }

    CREATE_OBJECT_GFX(player_gfx);
    Object *player = newObject(&w, 128, 128, 2, &oamMain, SpriteSize_16x32, SpriteColorFormat_16Color, &player_gfx);

    startBgDraw();
    CREATE_BG_GFX(grass);
    newBackground(&w, 2, &grass, BgType_Text8bpp, BgSize_T_512x512, 9, 3);

    CREATE_BG_GFX(center);
    newBackground(&w, 1, &center, BgType_Text8bpp, BgSize_T_512x512, 4, 2);

    CREATE_BG_GFX(centerTop);
    newBackground(&w, 0, &centerTop, BgType_Text8bpp, BgSize_T_512x512, 0, 1);
    endBgDraw();

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
        w.camera_x = player->x-120;
        w.camera_y = player->y-80;
        bgSetScroll(w.bgs[0]->id, w.camera_x, w.camera_y);
        bgSetScroll(w.bgs[1]->id, w.camera_x, w.camera_y);
        bgSetScroll(w.bgs[2]->id, w.camera_x, w.camera_y);
        bgUpdate();

        updateScreens(&w);

        swiWaitForVBlank();
        oamUpdate(&oamSub);
        oamUpdate(&oamMain);
    }

    return 0;
}
