#include <iostream>
#include <vector>
#include <cmath>
#include "BMP.h"
#include <random>
#include <algorithm>

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


//BMP seemCarving(const BMP& image) {
//	BMP energy = image;
//
//	int r, g, b, k = 0;
//
//	for (int i = 0; i < image.getHeight(); ++i) {
//		for (int j = 0; j < image.getWidth(); ++j) {
//			r = g = b = k = 0;
//			if (i != 0) {
//				r += abs(image.getPixel(j, i).r - image.getPixel(j, i - 1).r);
//				g += abs(image.getPixel(j, i).g - image.getPixel(j, i - 1).g);
//				b += abs(image.getPixel(j, i).b - image.getPixel(j, i - 1).b);
//				k++;
//			}
//			if (j != 0) {
//				r += abs(image.getPixel(j, i).r - image.getPixel(j - 1, i).r);
//				g += abs(image.getPixel(j, i).g - image.getPixel(j - 1, i).g);
//				b += abs(image.getPixel(j, i).b - image.getPixel(j - 1, i).b);
//				k++;
//			}
//			
//			if (k != 0) {
//				r /= k;
//				g /= k;
//				b /= k;
//			}
//			energy.setPixel(j, i, rgba(r+g+b));
//		}
//	}
//
//
//	//energy.saveAs("energy");
//	//open("energy");
//
//
//	vector<vector<int>> matrix(image.getHeight(), vector<int>(image.getWidth(), 0));
//
//	for (int i = 0; i < image.getWidth(); ++i) {
//		matrix[energy.getHeight() - 1][i] = energy.getPixel(i, energy.getHeight() - 1).r;
//	}
//
//	int minV;
//
//	for (int i = energy.getHeight() - 2; i > 0; --i) {
//		for (int j = 0; j < energy.getWidth(); ++j) {
//			minV = matrix[i + 1][j];
//			if(j != 0) minV = min(minV, matrix[i + 1][j - 1]);
//			if(j != energy.getWidth() - 1) minV = min(minV, matrix[i + 1][j + 1]);
//			matrix[i][j] = energy.getPixel(j, i).r + minV;
//		}
//	}
//
//	int minIndex = 0;
//	minV = matrix[0][0];
//	for (int i = 1; i < energy.getWidth(); ++i) {
//		if (matrix[0][i] < minV) {
//			minV = matrix[0][i];
//			minIndex = i;
//		}
//	}
//
//	int x = minIndex, y = 0;
//	int newx = minIndex, newy = 0;
//	
//	while (y + 1 != energy.getHeight()) {
//		minV = matrix[y + 1][x];
//		newx = x;
//		newy = y + 1;
//
//		if (x != 0) { 
//			if (matrix[y + 1][x - 1] < minV) {
//				minV = matrix[y + 1][x - 1];
//				newx = x - 1;
//			}
//		}
//		if (x != energy.getWidth() - 1) { 
//			if (matrix[y + 1][x + 1] < minV) {
//				minV = matrix[y + 1][x + 1];
//				newx = x + 1;
//			}
//		}
//		matrix[y][x] = -1;
//		//matrix[y].erase(matrix[y].begin() + x);
//
//		x = newx;
//		y = newy;
//	}
//
//	matrix[y][x] = -1;
//
//	auto newBMP = *createBMP(image.getWidth() - 1, image.getHeight(), "", "newBMP", BMP24);
//
//	int tempi = 0, tempj = 0;
//
//
//	for (int i = 0; i < image.getHeight(); ++i) {
//		tempj = 0;
//		tempi = i;
//		for (int j = 0; j < image.getWidth() - 1; ++j) {
//			if (matrix[i][j] != -1) {
//				newBMP.setPixel(j, tempi, image.getPixel(tempj, tempi));
//				tempj++;
//			}
//			else {
//				newBMP.setPixel(j, tempi, image.getPixel(tempj + 1, tempi));
//				tempj+=2;
//			}
//			
//		}
//	}
//
//	//newBMP.save();
//
//	return newBMP;
//}


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


		BMP bmp("castle");
		addNoise(bmp, 0, 10);
		bmp.saveAs("blackTemp");
		open("blackTemp");


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