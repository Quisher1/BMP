#include <iostream>
#include <vector>
#include "BMP.h"


using namespace std;

#define PI 3.14159265
#define E 2.71828284

double gauss(int x, int y, float sigma = 0.84089642) { // 0.84089642 | 0.65041
	return (pow(E, -((x*x + y*y) / (2 * sigma*sigma))) / (2 * PI*sigma*sigma));
}



void negative(BMP& image) {
	for (int i = 0; i < image.getHeight(); ++i) {
		for (int j = 0; j < image.getWidth(); ++j) {
			image.setPixel(j, i, rgba::WHITE - image.getPixel(j, i));
		}
	}
}

void logTransformation(BMP& image, const float c = 1) {
	float maxLog = c * 255 / log10(1 + 255);
	for (int i = 0; i < image.getHeight(); ++i) {
		for (int j = 0; j < image.getWidth(); ++j) {
			image.setPixel(j, i, rgba(maxLog * log10(1 + image.getPixel(j, i).r),
									  maxLog * log10(1 + image.getPixel(j, i).g),
									  maxLog * log10(1 + image.getPixel(j, i).b),
									  image.getPixel(j, i).a));
		}
	}
}


int main() {
	try {

		BMP bmp("Ocean32");

		bmp.saveAs("temp");

		bmp.loadImage("", "temp");

		logTransformation(bmp, 1);

		bmp.save();
		bmp.open();
		
	}
	catch (std::runtime_error &err) {
		cout << err.what() << endl;
	}


	return 0;
}