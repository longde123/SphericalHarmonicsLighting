#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "convert.h"

using namespace std;


enum CUBEMAP_FACES{POSX, POSY, POSZ, NEGX, NEGY, NEGZ};

struct XYZ{ float x, y, z; };
struct RGB{ float r, g, b; };
struct XYZRGB{ XYZ pos; RGB color; };

vector<XYZRGB> LoadCubemap(array<string, 6> images)
{
	vector<XYZRGB> data;
	for (int index = 0; index < 6; index++){
		cv::Mat img = cv::imread(images[index]);
		img.convertTo(img, CV_32FC3);
		
		float w = float(img.cols - 1), h = float(img.rows - 1);
		for (int j = 0; j < img.rows; j++){
			for (int i = 0; i < img.cols; i++){
				auto pixel = img.at<cv::Vec3f>(j, i);
				RGB color = { pixel[0], pixel[1], pixel[2] };
				float u = float(i) / w;
				float v = float(j) / h;
				XYZ p;
				convert_cube_uv_to_xyz(i, u, v, &p.x, &p.y, &p.z);
				data.push_back({p, color});
			}
		}
	}
	return data;
}

int main(int argc, char *argv[])
{
	if (argc < 2){
		cout << "Usage: sh_sampler posx posy posz negx negy negz output.txt" << endl;
		return 0;
	}


	return 0;
}