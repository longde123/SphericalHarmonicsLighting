#include <iostream>
#include <array>
#include <string>
#include "convert.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 8){
		cout << "Usage: sh_sampler posx negx posy negy posz negz output.txt" << endl;
		return 0;
	}
	
	try{
		array<string, 6> images;
		for (int i = 0; i < 6; i++){
			images[i] = argv[i + 1];
		}
		string plyfile = argv[7];
		Cubemap cubemap(images);
		WritePLY plywriter(plyfile, cubemap.size());
		cubemap.Read(plywriter);
	}
	catch (const exception& e){
		cerr << e.what() << endl;
	}
	return 0;
}