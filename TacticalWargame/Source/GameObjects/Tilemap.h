#pragma once
#include <raylib/raylib.h>
#include "GameObject.h"
#include <memory>

namespace aie
{
    class Tilemap : public GameObject
    {
    private:
        Texture maptexture;
        const int xtiles;
        const int ytiles;
        const float xdim;
        const float ydim;

        //'data' is a linear array of size xtiles*ytiles
        std::unique_ptr<float[]> data;

        float maxweight;
        //Tilemap overlays weights on top of an image
        //The value of a tile can be queried
    public:
        static const GUID guid = 0x4262149C;
        
        //Tilemap takes ownership of 'weights'
        //x/y_tiles is the number of squares in x and y
        //x/ysize is the size of the map in pixels
        //weights is an array of x_tiles*y_tiles floats
        Tilemap(int x_tiles, int y_tiles, const float xsize, const float ysize, float* weights, Texture map);
        
        //Gets the weight at position x,y
        float GetWeight(float x, float y) const;
        virtual void Render() override;
    };
}