#ifndef GFX_H
#define GFX_H

#define TILES(a) a##Tiles
#define TILES_LEN(a) a##TilesLen
#define PAL(a) a##Pal
#define PAL_LEN(a) a##PalLen
#define MAP(a) a##Map
#define MAP_LEN(a) a##MapLen
#define CREATE_OBJECT_GFX(a) gfx_t (a) = {TILES(a), TILES_LEN(a), PAL(a), PAL_LEN(a)}; 
#define CREATE_BG_GFX(a) gfx_t (a) = {TILES(a), TILES_LEN(a), PAL(a), PAL_LEN(a), MAP(a), MAP_LEN(a)}; 


typedef struct
{
    const uint *tiles;
    u32 tilesLen;
    const u16 *pal;
    u32 palLen;
    const u16 *map;
    u32 mapLen;
} gfx_t;

#endif
