#ifndef CONVERT_H_
#define CONVERT_H_

#include <math.h>
#include <vector>
#include <array>
#include <functional>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>


enum CUBEMAP_FACES{ POSX, POSY, POSZ, NEGX, NEGY, NEGZ };

struct XYZ{ float x, y, z; };
struct RGB{ float r, g, b; };
struct XYZRGB{ XYZ pos; RGB color; };

class Cubemap{
	std::array<std::string, 6> image_files;
	std::array<cv::Mat, 6> images;
	int rows, cols;
public:
	Cubemap(std::array<std::string, 6> image_filenames);
	void Read(std::function<void(XYZRGB)> proc);
	int size()const{ return rows*cols * 6; }
};

class WritePLY{
	ofstream ofs;
public:
	WritePLY(string filename, int size);
	void operator()(XYZRGB pixel);
};

#endif