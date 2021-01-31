#include "BMP.h"


rgba::rgba() {

}

rgba::rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {

}



///////////////////////


BMP::BMP() {

}

BMP::BMP(const std::string& imageName) {
	loadImage("", imageName);
}

BMP::BMP(const std::string& imagePath, const std::string& imageName) {
	loadImage(imagePath, imageName);
}


void BMP::loadImage(const std::string& imagePath, const std::string& imageName) {
	std::string buffer = imagePath + imageName + ".bmp";

	FILE* file;
	file = fopen(buffer.c_str(), "rb");
	if (file == nullptr)
		throw std::runtime_error("ERROR::FILE_NOT_FOUND");


	info = new unsigned char[54];
	fread(info, sizeof(unsigned char), 54, file);

	//width = *(int*)&info[18];
	//height = abs(*(int*)&info[22]);
	//bitsPerPixel = *(int*)&info[28];

	width = info[18] + 256 * info[19];
	height = info[22] + 256 * info[23];
	bitsPerPixel = info[28] + 256 * info[29];

	
	this->imageName = imageName;
	this->imagePath = imagePath;

	if (bitsPerPixel == 32) {
		colorModel = 4;
		offset = 0;
	}
	else if (bitsPerPixel == 24) {
		colorModel = 3; 
		offset = abs(((width * 3 + 3) & (~3)) - width * 3); 
	}
	else 
		throw std::runtime_error("ERROR::THIS_BMP_ISN'T_SUPPORTED");

	std::cout << "Offset: " << offset << std::endl;

	data = new unsigned char[width * height * colorModel];
	unsigned char* offsetBuffer = new unsigned char[offset];
	int readingOffset = 0;
	for (int i = 0; i < height; ++i) {
		fread(data + readingOffset, sizeof(unsigned char), width * colorModel, file);
		fread(offsetBuffer, sizeof(unsigned char), offset, file);
		readingOffset += width * colorModel;
	}
	delete[] offsetBuffer;
	fclose(file);
}

BMP::~BMP() {
	if(info != nullptr)
		delete[] info;
	if(data != nullptr)
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
int BMP::getColorModel() const {
	return colorModel;
}
std::string BMP::getImageName() const {
	return imageName;
}
std::string BMP::getImagePath() const {
	return imagePath;
}

rgba BMP::getPixel(int x, int y) const {
	return rgba(data[(y*width + x) * 3 + 2], data[(y*width + x) * 3 + 1], data[(y*width + x) * 3 + 0], colorModel == 4 ? data[(y*width + x) * 3 + 3] : 255);
}
void BMP::setPixel(int x, int y, rgba& color) {
	data[(y*width + x) * 3 + 2] = color.r;
	data[(y*width + x) * 3 + 1] = color.g;
	data[(y*width + x) * 3 + 0] = color.b;
	if (colorModel == 4) data[(y*width + x) * 3 + 3] = color.a;
}

void BMP::save() {
	if(imagePath == "")
		saveAs(imageName);
	else
		saveAs(imagePath + '/' + imageName);
}

void BMP::saveAs(const std::string& newImageName) {

	if (data == nullptr || info == nullptr)
		throw std::runtime_error("ERROR::FILE_DOESN'T_EXIST");

	std::string name = newImageName + ".bmp";

	FILE* file;
	file = fopen(name.c_str(), "wb"); // WARNING: ONLY 'WB', now 'W'. It will destroy image

	

	fwrite(info, sizeof(unsigned char), 54, file);

	unsigned char* offsetBuffer = new unsigned char[offset];
	memset(offsetBuffer, 0, offset);
	int writingOffset = 0;
	for (int i = 0; i < height; ++i) {
		fwrite(data + writingOffset, sizeof(unsigned char), width * colorModel, file);
		fwrite(offsetBuffer, sizeof(unsigned char), offset, file);
		writingOffset += width * colorModel;
	}
	delete[] offsetBuffer;

	offsetBuffer = new unsigned char[2];
	memset(offsetBuffer, 0, 2);
	fwrite(offsetBuffer, sizeof(unsigned char), 2, file);
	delete[] offsetBuffer;

	fclose(file);
}


void BMP::open() {
	std::string bmp;
	if(imagePath == "")
		bmp = "start " + imageName + ".bmp";
	else
		bmp = "start " + imagePath + imageName + ".bmp";
	system(bmp.c_str());
}

void BMP::open(const std::string& imageName) {
	std::string bmp;
	bmp = "start " + imageName + ".bmp";
	system(bmp.c_str());
}


void BMP::clear() {

}