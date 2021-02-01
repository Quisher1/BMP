#include "BMP.h"



rgba::rgba() {

}
rgba::rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {

}
rgba rgba::operator+(rgba& p) {
	return rgba(r + p.r > 255 ? 255 : r + p.r,
				g + p.g > 255 ? 255 : g + p.g, 
				b + p.b > 255 ? 255 : b + p.b, 
				a + p.a > 255 ? 255 : a + p.a);
}
rgba rgba::operator-(rgba& p) {
	return rgba(r - p.r < 0 ? 0 : r - p.r,
				g - p.g < 0 ? 0 : g - p.g,
				b - p.b < 0 ? 0 : b - p.b,
				a - p.a < 0 ? 0 : a - p.a);
}
rgba rgba::operator*(unsigned char k) {
	return rgba(r * k > 255 ? 255 : r * k,
				g * k > 255 ? 255 : g * k,
				b * k > 255 ? 255 : b * k,
				a * k > 255 ? 255 : a * k);
}
rgba rgba::operator/(unsigned char k) {
	if (k == 0)
		return rgba(255, 255, 255, 255);

	return rgba(r / k, g / k, b / k, a / k);
}


const rgba rgba::RED(255, 0, 0);
const rgba rgba::GREEN(0, 255, 0);
const rgba rgba::BLUE(0, 0, 255);
const rgba rgba::YELLOW(255, 255, 0);
const rgba rgba::CYAN(0, 255, 255);
const rgba rgba::PURPLE(255, 0, 255);
const rgba rgba::WHITE(255, 255, 255);
const rgba rgba::BLACK(0, 0, 0);

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
	bitsPerPixel = (BMP_FORMAT)(info[28] + 256 * info[29]);

	
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
	if (x < 0 || y < 0 || x >= width || y >= height)
		throw std::runtime_error("ERROR::INDEXES_OUT_OF_RANGE");

	return rgba(data[(y*width + x) * 3 + 2], data[(y*width + x) * 3 + 1], data[(y*width + x) * 3 + 0], colorModel == 4 ? data[(y*width + x) * 3 + 3] : 255);
}
void BMP::setPixel(int x, int y, rgba color) {
	if (x < 0 || y < 0 || x >= width || y >= height)
		throw std::runtime_error("ERROR::INDEXES_OUT_OF_RANGE");

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
	width = -1;
	height = -1;
	bitsPerPixel = NONE;
	colorModel = -1;
	offset = 0;

	imageName.clear();
	imagePath.clear();

	if (info != nullptr)
		delete[] info;
	if (data != nullptr)
		delete[] data;
}


BMP* createBMP(const int width, const int height, const std::string& imagePath, const std::string& imageName, const BMP_FORMAT format) {
	if (format == NONE)
		return nullptr;

	FILE* file;


	std::string image;

	if (imagePath == "")
		image = imageName + ".bmp";
	else
		image = imagePath + '/' + imageName + ".bmp";

	file = fopen(image.c_str(), "wb");
	
	int offset = 0;
	long long imageSize = (format == 24 ? 3 : 4) * width * height + 54 + 2;
	if (format == 24)
		offset = abs(((width * 3 + 3) & (~3)) - width * 3);


	unsigned char* info = new unsigned char[54];
	memset(info, 0, 54);

	unsigned char* data = new unsigned char[width * height * (format == BMP24 ? 3 : 4) + offset*height + 2];
	memset(data, 0, width * height * (format == BMP24 ? 3 : 4) + offset*height + 2);


	info[0] = unsigned char('B');
	info[1] = unsigned char('M');

	info[2] = unsigned char(imageSize);
	info[3] = unsigned char(imageSize >> 8);
	info[4] = unsigned char(imageSize >> 16);
	info[5] = unsigned char(imageSize >> 24);

	info[10] = unsigned char(54);
	info[14] = unsigned char(40);

	info[18] = unsigned char(width);
	info[19] = unsigned char(width >> 8);
	info[20] = unsigned char(width >> 16);
	info[21] = unsigned char(width >> 24);
	info[22] = unsigned char(height);
	info[23] = unsigned char(height >> 8);
	info[24] = unsigned char(height >> 16);
	info[25] = unsigned char(height >> 24);

	info[26] = unsigned char(1);

	info[28] = unsigned char(BMP_FORMAT(format));

	info[30] = unsigned char(0);

	info[34] = unsigned char(width*height * 3 + offset*height + 2);
	info[35] = unsigned char((width*height * 3 + offset*height + 2) >> 8);
	info[36] = unsigned char((width*height * 3 + offset*height + 2) >> 16);
	info[37] = unsigned char((width*height * 3 + offset*height + 2) >> 24);

	info[38] = unsigned char(2834);
	info[39] = unsigned char(2834 >> 8);

	info[42] = unsigned char(2834);
	info[43] = unsigned char(2834 >> 8);

	info[46] = unsigned char(0);
	info[50] = unsigned char(0);

	
	fwrite(info, sizeof(unsigned char), 54, file);
	fwrite(data, sizeof(unsigned char), width * height * (format == BMP24 ? 3 : 4) + offset*height + 2, file);

	fclose(file);

	BMP* bmp = new BMP(imagePath, imageName);


	return bmp;
}