#include <iostream>
#include <string>
#include "BMP.h"
#include <fstream>

using namespace std;

int main() {

	try {

		BMP bmp("forest");

		cout << "Width: " << bmp.getWidth() << "\nHeight: " << bmp.getHeight() << "\nBPP: " << bmp.getBitsPerPixel() << std::endl;
	
		int x = 100, y = 100;
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


		BMP *white = createBMP(500, 750, "", "white2", BMP24);
		

		for (int i = 0; i < bmp.getHeight() / 2; ++i) {
			for (int j = 0; j < bmp.getWidth(); ++j) {
				white->setPixel(j, i, rgba::PURPLE);
			}
		}
		white->save();
		white->open();


	}
	catch (std::runtime_error &err) {
		cout << err.what() << endl;
	}


	return 0;
}