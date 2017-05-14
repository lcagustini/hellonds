#include "background.h"

Background newBackground(int layer, BgType type, BgSize size, int mapBase, int tileBase){
    Background b;
    b.id = bgInit(layer, type, size, mapBase, tileBase);
    return b;
}

void setBackgroundGfx(Background b, gfx_t *data){
    DC_FlushAll();
    dmaCopyHalfWordsAsynch(0, data->tiles, bgGetGfxPtr(b.id), data->tilesLen);
    dmaCopyHalfWordsAsynch(1, data->map, bgGetMapPtr(b.id), data->mapLen);
    dmaCopyHalfWordsAsynch(2, data->pal, BG_PALETTE, data->palLen);
}
