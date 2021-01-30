#include "BMP.h"

BMP::BMP() {

}

BMP::BMP(const std::string& imageName) {
	std::string buffer = imageName + ".bmp";

	FILE* file;
	file = fopen(buffer.c_str(), "rb");

	std::cout << file << std::endl;

	info = new unsigned char[54];
	fread(info, sizeof(unsigned char), 54, file);

	//width = *(int*)&info[18];
	//height = abs(*(int*)&info[22]);
	//bitsPerPixel = *(int*)&info[24];

	width = info[18] + 256 * info[19];
	height = info[22] + 256 * info[23];
	bitsPerPixel = info[24] + 256 * info[25];


	std::cout << width << " " << height << " " << bitsPerPixel << std::endl;
}

BMP::BMP(const std::string& imagePath, const std::string& imageName) {


}


BMP::~BMP() {
	delete[] info;
	delete[] data;
}


int BMP::getHeight() const {
	return height;
}
int BMP::getWidth() const {
	return width;
}
int BMP::getBitsPerPixel() const {
	return bitsPerPixel;
}


//rgba BMP::operator()(int x, int y) const {
//
//}
//rgba& BMP::operator()(int x, int y) {
//
//}