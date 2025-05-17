#include "Tilemap.h"
#include "../Application.h"
#include <string>

aie::Tilemap::Tilemap(int x_tiles, int y_tiles, const float xsize, const float ysize, float* weights, Texture map) 
    : GameObject{Tilemap::guid}
    , maptexture{ map }
    , xtiles{ x_tiles }
    , ytiles{ y_tiles }
    , xdim{ xsize }
    , ydim{ ysize }
    , data{weights}
    , maxweight{*std::max_element(weights, weights + x_tiles * y_tiles)}
{
}

float aie::Tilemap::GetWeight(float x, float y) const{
    x = x / xdim;
    y = y / ydim;
    int nx = (int)(xtiles * x);
    int ny = (int)(ytiles * y);
    if (x >= 1.0f)
        nx = xtiles -1;
    if (y >= 1.0f)
        ny = ytiles -1;
    if (x < 0.0f)
        nx = 0;
    if (y < 0.0f)
        ny = 0;
    return data[nx + ny * xtiles];
}

void aie::Tilemap::Render() {
    //Draw the map
    DrawTexture(maptexture, 0, 0, RAYWHITE);

    //Don't draw if debug mode is too low
    if (Application::GetApplication()->DebugMode() < Application::DebugModes::FULL)
        return;

    //Draw the grid horizontals
    for(int x=0;x<xtiles;++x){
        DrawLine((int)(xdim / xtiles * x), 0, (int)(xdim / xtiles * x), (int)ydim, Color{ 0,0,0,255 });
    }

    //Draw the grid verticals
    for (int y = 0; y < ytiles; ++y) {
        DrawLine(0, (int)(ydim / ytiles * y), (int)xdim, (int)(ydim / ytiles * y), Color{ 0,0,0,255 });
    }

    //Draw the tiles in colour and the weights as text
    const float tilex = 1.0f/xtiles;
    const float tiley = 1.0f/ytiles;

    for (int y = 0; y < ytiles;++y){
        for (int x = 0; x < xtiles; ++x) {
            const float xp = tilex * (0.5f + x) * xdim;
            const float yp = tiley * (0.5f + y) * ydim;
            const float rawweight = GetWeight(xp, yp);
            const float weight = rawweight / maxweight;

            //Colour the tile
            Color c{ (unsigned char)(255 * weight) , (unsigned char)(128 * weight), (unsigned char)(64 * weight), (unsigned char)(64) };
            DrawRectangle((int)(x * xdim / xtiles), (int)(y * ydim / ytiles), (int)(xdim/xtiles), (int)(ydim / ytiles), c);

            //Make the text 
            char numeral[2] = { '0' + (unsigned char)rawweight,'\0' };
            DrawText(numeral, (int)((x+0.5f) * xdim / xtiles) , (int)((y+0.5f) * ydim / ytiles), 10, RAYWHITE);
        }
    }
}