#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <array>

using namespace std;


enum CUBEMAP_FACES{POSX, POSY, POSZ, NEGX, NEGY, NEGZ};


void LoadCubemap(array<string, 6> images)
{
	cv::Mat imgs[6];
	for (int i = 0; i < 6; i++){
		imgs[i] = cv::imread(images[i]);
		
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2){
		cout << "Usage: sh_sampler posx posy posz negx negy negz output.txt" << endl;
		return 0;
	}


	return 0;
}