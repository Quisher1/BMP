#pragma once
#include <iostream>
#include <string>
#include <fstream>


enum BMP_FORMAT {
	NONE = -1,
	BMP24 = 24,
	BMP32 = 32
};



class rgba {
public:
	unsigned char r = 0, g = 0, b = 0, a = 0;
	rgba();
	rgba(int color);
	rgba(int r, int g, int b, int a = 0);


	rgba operator*(float k);
	rgba operator/(float k);

	friend rgba operator*(const float k, const rgba& p);
	friend rgba operator+(const rgba& p1, const rgba& p2);
	friend rgba operator-(const rgba& p1, const rgba& p2);

	static const rgba RED;
	static const rgba GREEN;
	static const rgba BLUE;
	static const rgba YELLOW;
	static const rgba CYAN;
	static const rgba PURPLE;
	static const rgba WHITE;
	static const rgba BLACK;
};





class BMP { // BMP saves pixel in BGR format
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
	void setPixel(int x, int y, rgba color);

	void loadImage(const std::string& imagePath, const std::string& imageName);

	void save();
	void saveAs(const std::string& newImageName);

	void open();
	void open(const std::string& imageName);

	void clear();
	unsigned char* info = nullptr;
	unsigned char* data = nullptr;
private:
	int width = -1, height = -1;
	int colorModel = -1;
	BMP_FORMAT bitsPerPixel = NONE;

	int offset = 0;

	std::string imageName;
	std::string imagePath;

	
};


BMP* createBMP(const int width, const int height, const std::string& imagePath, const std::string& imageName, const BMP_FORMAT format, const rgba fillColor = rgba::WHITE);