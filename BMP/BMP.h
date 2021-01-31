#pragma once
#include <iostream>
#include <string>
#include <fstream>

class rgba {
public:
	unsigned char r = 0, g = 0, b = 0, a = 255;
	rgba();
	rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);


	void operator+(rgba& p);
	void operator-(rgba& p);
	void operator*(rgba& p);
	void operator/(rgba& p);
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

	rgba getPixel(int x, int y) const;
	void setPixel(int x, int y, rgba& color);

	void loadImage(const std::string& imagePath, const std::string& imageName);

	void save();
	void saveAs(const std::string& newImageName);

	void open();
	void open(const std::string& imageName);

	void clear();

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