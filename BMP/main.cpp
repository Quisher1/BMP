#include <iostream>
#include <string>
#include "BMP.h"

using namespace std;

int main() {

	try {

		BMP bmp("space");

		cout << "Width: " << bmp.getWidth() << "\nHeight: " << bmp.getHeight() << "\nBPP: " << bmp.getBitsPerPixel() << std::endl;
	
		int x = 1, y = 1;
		//cout << bmp(x, y).r << " " << bmp(x, y).g << " " << bmp(x, y).b << endl;
		
		//bmp(x, y) = rgba(255, 255, 255);
		cout << bmp(x, y).r << " " << bmp(x, y).g << " " << bmp(x, y).b << endl;


		bmp.save();
		bmp.open();
	}
	catch (std::runtime_error &err) {
		cout << err.what() << endl;
	}


	return 0;
}