#include <iostream>
#include <string>
#include "BMP.h"

using namespace std;

int main() {

	try {

		BMP bmp("forest");

		cout << "Width: " << bmp.getWidth() << "\nHeight: " << bmp.getHeight() << "\nBPP: " << bmp.getBitsPerPixel() << std::endl;
	
		int x = 10, y = 10;
		//cout << bmp(x, y).r << " " << bmp(x, y).g << " " << bmp(x, y).b << endl;
		
		rgba pixel = bmp.getPixel(x, y);
		cout << int(pixel.r) << " " << int(pixel.g) << " " << int(pixel.b) << " " << int(pixel.a) << endl;

		for (int i = 0; i < 100; ++i) {
			for (int j = 0; j < 100; ++j) {
				bmp.setPixel(x + j, y + i, bmp.getPixel(x + j, y + i));
			}
		}

		bmp.saveAs("newImage");
		bmp.open("newImage");
	}
	catch (std::runtime_error &err) {
		cout << err.what() << endl;
	}


	return 0;
}