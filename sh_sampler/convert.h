#ifndef CONVERT_H_
#define CONVERT_H_

#include <math.h>
#include <vector>
#include <array>
#include <functional>
#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>


enum CUBEMAP_FACES{ POSX, NEGX, POSY, NEGY, POSZ, NEGZ};

struct XYZ{ float x, y, z; };
struct RGB{ float r, g, b; };
struct XYZRGB{ XYZ pos; RGB color; };

std::ostream& operator<<(std::ostream& os, RGB color);

class Cubemap{
	std::array<std::string, 6> image_files;
	std::array<cv::Mat, 6> images;
	int rows, cols;
public:
	Cubemap(std::array<std::string, 6> image_filenames);
	template<typename T>
	void Read(T& proc);
	int size()const{ return rows*cols * 6; }
};

class WritePLY{
	std::shared_ptr<std::ofstream> plyfs;
public:
	WritePLY(std::string filename, int size);
	void operator()(XYZRGB pixel);
};

void convert_cube_uv_to_xyz(int index, float u, float v, float *x, float *y, float *z);
void convert_xyz_to_cube_uv(float x, float y, float z, int *index, float *u, float *v);

template<typename T>
void Cubemap::Read(T& proc)
{
	for (int index = 0; index < 6; index++){
		cv::Mat& img = images[index];
		img /= 255;
		float w = float(img.cols - 1), h = float(img.rows - 1);
		for (int j = 0; j < img.rows; j++){
			for (int i = 0; i < img.cols; i++){
				auto pixel = img.at<cv::Vec3f>(j, i);
				RGB color = { pixel[0], pixel[1], pixel[2] };
				float u = float(i) / w;
				float v = float(img.rows - j - 1) / h;
				XYZ p;
				convert_cube_uv_to_xyz(index, u, v, &p.x, &p.y, &p.z);
				proc({ p, color });
			}
		}
	}
}


#endif