#pragma once
#include <iostream>
#include <string>
#include <fstream>

struct rgba {
	int r = 0, g = 0, b = 0, a = 255;
	rgba() {}
	rgba(int r, int g, int b, int a = 255) : r(r), g(g), b(b), a(a) {}
};


class BMP {
public:
	BMP();
	BMP(const std::string& imageName);
	BMP(const std::string& imagePath, const std::string& imageName);

	~BMP();


	int getHeight() const;
	int getWidth() const;
	int getBitsPerPixel() const;


	rgba operator()(int x, int y) const;
	rgba& operator()(int x, int y);


private:
	int width, height;
	int bitsPerPixel;

	std::string imageName;
	std::string imagePath;

	unsigned char* info = nullptr;
	unsigned char* data = nullptr;
};


BMP* createBMP(int width, int height, int bitsPerPixel);