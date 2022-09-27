#ifndef WB_RT_UTILITY_HPP
#define WB_RT_UTILITY_HPP

#include <SFML/Graphics.hpp>
#include <math.h>

#include "camera.hpp"
#include "colour.hpp"
#include "light.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

// main.cpp global variables
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int BLOCK_SIZE_X;
extern const int BLOCK_SIZE_Y;
extern Camera CAMERA;
extern Sphere SPHERE_LIST[];
extern const int SPHERE_COUNT;
extern Light LIGHT_LIST[];
extern const int LIGHT_COUNT;
extern const float GAMMA;
extern sf::Uint8 PIXELS[];
extern Colour TOP_BG_COL;
extern Colour BOT_BG_COL;
extern int availableThreads;
extern int pixelsRendered;

void DrawPixel(int x, int y, Colour(col));
bool RaySphereIntersection(Vec3 p0, Vec3 p1, Vec3 &intersection, int &sphereHitIndex, int ignoreIndex);
bool SpherePointInShadow(Vec3 spherePoint, Light light, int ignoreIndex);
void RenderPixelBlock(int x, int y);

#endif