#include <iostream>
#include <vector>
#include <cmath>
#include "BMP.h"
#include <random>

using namespace std;

#define PI 3.14159265
#define E 2.71828284

double gauss(int x, int y, float sigma = 0.84089642) { // 0.84089642 | 0.65041
	return (pow(E, -((x*x + y*y) / (2 * sigma*sigma))) / (2 * PI*sigma*sigma));
}



void blur(BMP& image, float** core, int width, int height) {
	if (width % 2 == 0 || height % 2 == 0)
		throw runtime_error("indices must be odd");

	float d = 0;
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			d += core[i][j];

	std::cout << width << " " << height << std::endl;

	float r, g, b, a;
	for (int i = 0; i < image.getHeight(); ++i) {
		for (int j = 0; j < image.getWidth(); ++j) {
			r = g = b = a = 0;
			for (int y = -height/2; y <= height/2; ++y) {
				for (int x = -width/2; x <= width/2; ++x) {
					if (j + x < 0 || j + x >= image.getWidth() || i + y < 0 || i + y >= image.getHeight()) {
						b += image.getPixel(j, i).b * core[y + height / 2][x + width / 2]; //b += data[(i*width + j) * colorModel + 0] * Core[(y + n / 2)*n + x + n / 2];
						g += image.getPixel(j, i).g * core[y + height / 2][x + width / 2]; //g += data[(i*width + j) * colorModel + 1] * Core[(y + n / 2)*n + x + n / 2];
						r += image.getPixel(j, i).r * core[y + height / 2][x + width / 2]; //r += data[(i*width + j) * colorModel + 2] * Core[(y + n / 2)*n + x + n / 2];
						if (image.getColorModel() == 4)
							a += image.getPixel(j, i).a * core[y + height / 2][x + width / 2];
					}
					else {
						b += image.getPixel(j + x, i + y).b * core[y + height / 2][x + width / 2]; //b += data[((i + y)*width + j + x) * colorModel + 0] * Core[(y + n / 2)*n + x + n / 2];
						g += image.getPixel(j + x, i + y).g * core[y + height / 2][x + width / 2]; //g += data[((i + y)*width + j + x) * colorModel + 1] * Core[(y + n / 2)*n + x + n / 2];
						r += image.getPixel(j + x, i + y).r * core[y + height / 2][x + width / 2]; //r += data[((i + y)*width + j + x) * colorModel + 2] * Core[(y + n / 2)*n + x + n / 2];
						if (image.getColorModel() == 4)
							a += image.getPixel(j + x, i + y).a * core[y + height / 2][x + width / 2];
					}
				}
			}

			r /= d; g /= d; b /= d; a /= d;
			image.setPixel(j, i, rgba(r > 255 ? 255 : r , g > 255 ? 255 : g, b > 255 ? 255 : b, a > 255 ? 255 : a));
		}
	}
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

double convert(double value, double From1, double From2, double To1, double To2){
	return (value - From1) / (From2 - From1)*(To2 - To1) + To1;
}

void addNoise(BMP &image, float mean = 0.f, float stddiv = 3.16f) { // Gaussian noise

	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<float> d(mean, stddiv);

	rgba color;
	int rand;
	for (int i = 0; i < image.getHeight(); ++i) {
		for (int j = 0; j < image.getWidth(); ++j) {
			rand = d(gen);
			rgba c = image.getPixel(j, i);
			color = rgba(c.r + rand, c.g + rand, c.b + rand, c.a);			
			image.setPixel(j, i, color);
		}
	}
}



int main() {
	try {

		//BMP bmp("Ocean32");
		//bmp.open();
		//bmp.saveAs("temp");

		//bmp.loadImage("", "temp");

		/*int n = 3, m = 3;

		float ** core = new float*[n];
		for (int i = 0; i < n; ++i) {
			core[i] = new float[m];
			for (int j = 0; j < m; ++j)
				core[i][j] = gauss(j - m / 2, i - n / 2);
		}*/


		//blur(bmp, core, m, n);

		////logTransformation(bmp, 1);

		//bmp.save();
		//bmp.open();



		BMP bmp("forest");

		addNoise(bmp, 0, 10);
		bmp.saveAs("blackTemp");
		bmp.open("blackTemp");
;


		/*float d = 0;
		int n = 100, m = 100;
		float ** core = new float*[n];
		for (int i = 0; i < n; ++i) {
			core[i] = new float[m];
			for (int j = 0; j < m; ++j) {
				core[i][j] = gauss(j - m / 2, i - n / 2, 5);
				d += core[i][j];
			}
				
		}

		auto image = createBMP(m, n, "", "Distribution", BMP24, rgba::BLACK);
		
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				float f = core[i][j] * 255 > 255 ? 255 : core[i][j] * 255;
				cout << f << endl;
				image->setPixel(j, i, rgba(f, f, f));
			}
		}
		image->save();
		image->open();*/
	}
	catch (std::runtime_error &err) {
		cout << err.what() << endl;
	}


	return 0;
}