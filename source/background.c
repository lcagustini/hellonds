#include "background.h"

Background newBackground(int layer, BgType type, BgSize size, int mapBase, int tileBase){
    Background b;
    b.id = bgInit(layer, type, size, mapBase, tileBase);
    return b;
}

void setBackgroundGfx(Background b, uint* tiles, u32 tilesLen, uint* map, u32 mapLen, uint* pal, u32 palLen){
    DC_FlushAll();
    dmaCopyHalfWordsAsynch(0, tiles, bgGetGfxPtr(b.id), tilesLen);
    dmaCopyHalfWordsAsynch(1, map, bgGetMapPtr(b.id), mapLen);
    dmaCopyHalfWordsAsynch(2, pal, BG_PALETTE, palLen);
}
