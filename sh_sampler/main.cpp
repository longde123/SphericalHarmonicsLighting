#include <iostream>
#include <array>
#include <string>
#include "convert.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 8){
		cout << "Usage: sh_sampler posx posy posz negx negy negz output.txt" << endl;
		return 0;
	}
	
	try{
		array<string, 6> images;
		for (int i = 0; i < 6; i++){
			images[i] = argv[i + 1];
		}
		auto points = ReadCubemap(images);
		WritePLY(points, argv[7]);
	}
	catch (const exception& e){
		cerr << e.what() << endl;
	}
	return 0;
}