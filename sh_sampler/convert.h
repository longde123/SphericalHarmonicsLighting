#ifndef CONVERT_H_
#define CONVERT_H_

#include <math.h>
#include <vector>
#include <array>
#include <string>


enum CUBEMAP_FACES{ POSX, POSY, POSZ, NEGX, NEGY, NEGZ };

struct XYZ{ float x, y, z; };
struct RGB{ float r, g, b; };
struct XYZRGB{ XYZ pos; RGB color; };

std::vector<XYZRGB> ReadCubemap(std::array<std::string, 6> images);
void WritePLY(const std::vector<XYZRGB>& points, std::string filename);

#endif