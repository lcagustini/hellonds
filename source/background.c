#include "background.h"

Background *newBackground(World *w, int layer, gfx_t *data, BgType type, BgSize size, int mapBase, int tileBase){
    Background *b = malloc(sizeof(Background));
    b->id = bgInit(layer, type, size, mapBase, tileBase);
    bgSetPriority(b->id, layer);
    b->data = data;

    w->bgs[layer] = b;

    DC_FlushAll();
    dmaCopyHalfWordsAsynch(0, b->data->tiles, bgGetGfxPtr(b->id), b->data->tilesLen);
    dmaCopyHalfWordsAsynch(1, b->data->map, bgGetMapPtr(b->id), b->data->mapLen);
    dmaCopyHalfWordsAsynch(2, b->data->pal, VRAM_E_EXT_PALETTE[b->id], b->data->palLen);
    return b;
}

void startBgDraw(){
    bgExtPaletteEnable();
    vramSetBankE(VRAM_E_LCD);
}

void endBgDraw(){
    vramSetBankE(VRAM_E_BG_EXT_PALETTE);
}
