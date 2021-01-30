#pragma once
#include <iostream>
#include <string>
#include <fstream>

class rgba {
public:
	int r = 0, g = 0, b = 0, a = 255;
	rgba();
	rgba(int r, int g, int b, int a = 255);
};


class BMP {
public:
	BMP();
	explicit BMP(const std::string& imageName);
	explicit BMP(const std::string& imagePath, const std::string& imageName);
	~BMP();


	int getHeight() const;
	int getWidth() const;
	int getBitsPerPixel() const;
	int getColorModel() const;
	std::string getImageName() const;
	std::string getImagePath() const;

	rgba operator()(int x, int y) const;
	//rgba& operator()(int x, int y);

	void loadImage(const std::string& imagePath, const std::string& imageName);

	void save();
	void saveAs(const std::string& newImageName);

	void open();

private:
	int width = -1, height = -1;
	int bitsPerPixel = -1, colorModel = -1;

	int offset = 0;

	std::string imageName;
	std::string imagePath;

	unsigned char* info = nullptr;
	unsigned char* data = nullptr;
};


BMP* createBMP(int width, int height, int bitsPerPixel);