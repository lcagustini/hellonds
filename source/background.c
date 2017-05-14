#include "background.h"

Background newBackground(int layer, BgType type, BgSize size, int mapBase, int tileBase){
    Background b;
    b.id = bgInit(layer, type, size, mapBase, tileBase);
    return b;
}

void setBackgroundGfx(Background* b, uint* tiles, u32 tilesLen, uint* map, u32 mapLen, uint* pal, u32 palLen){
    b->tiles = tiles;
    b->map = map;
    b->pal = pal;

    b->palLen = palLen;
    b->mapLen = mapLen;
    b->tilesLen = tilesLen;
}


