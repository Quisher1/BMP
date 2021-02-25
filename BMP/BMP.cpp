#include "BMP.h"


rgba::rgba() {

}
rgba::rgba(int color) : a() {
	this->r = color > 255 ? 255 : color < 0 ? 0 : color;
	this->g = color > 255 ? 255 : color < 0 ? 0 : color;
	this->b = color > 255 ? 255 : color < 0 ? 0 : color;
}
rgba::rgba(int r, int g, int b, int a) {
	this->r = r > 255 ? 255 : r < 0 ? 0 : r;
	this->g = g > 255 ? 255 : g < 0 ? 0 : g;
	this->b = b > 255 ? 255 : b < 0 ? 0 : b;
	this->a = a > 255 ? 255 : a < 0 ? 0 : a;
}

rgba rgba::operator*(float k) {
	return rgba(r * k > 255 ? 255 : r * k,
		g * k > 255 ? 255 : g * k,
		b * k > 255 ? 255 : b * k,
		a * k > 255 ? 255 : a * k);
}
rgba rgba::operator/(float k) {
	if (k == 0)
		return rgba(255, 255, 255, 0);

	return rgba(r / k, g / k, b / k, a / k);
}
rgba operator*(const float k, const rgba& p) {
	return rgba(p.r * k > 255 ? 255 : p.r * k,
		p.g * k > 255 ? 255 : p.g * k,
		p.b * k > 255 ? 255 : p.b * k,
		p.a * k > 255 ? 255 : p.a * k);
}
rgba operator+(const rgba& p1, const rgba& p2) {
	return rgba(p1.r + p2.r, p1.g + p2.g, p1.b + p2.b, p1.a + p2.a);
}
rgba operator-(const rgba& p1, const rgba& p2) {
	return rgba(p1.r - p2.r, p1.g - p2.g, p1.b - p2.b, p1.a - p2.a);
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
BMP::~BMP() {
	if (header != nullptr)
		delete[] header;
	if (info != nullptr)
		delete[] info;
	if (data != nullptr)
		delete[] data;
}

BMP::BMP(const BMP& image) {
	width = image.width;
	height = image.height;
	colorModel = image.colorModel;
	bitsPerPixel = image.bitsPerPixel;

	offset = image.offset;
	infoLogSize = image.infoLogSize;

	imageName = image.imageName;
	imagePath = image.imagePath;

	header = new unsigned char[14];
	info = new unsigned char[infoLogSize];
	data = new unsigned char[width * height * colorModel];

	memcpy(header, image.header, 14);
	memcpy(info, image.info, infoLogSize);
	memcpy(data, image.data, width * height * colorModel);
}

void BMP::operator=(const BMP& image) {
	if (this == &image)
		throw std::runtime_error("ERROR::ASSIGNING_AN_OBJECT_TO_ITSELF");

	if (header != nullptr)
		delete[] header;
	if (info != nullptr)
		delete[] info;
	if (data != nullptr)
		delete[] data;

	width = image.width;
	height = image.height;
	colorModel = image.colorModel;
	bitsPerPixel = image.bitsPerPixel;

	offset = image.offset;
	infoLogSize = image.infoLogSize;

	imageName = image.imageName;
	imagePath = image.imagePath;

	header = new unsigned char[14];
	info = new unsigned char[infoLogSize];
	data = new unsigned char[width * height * colorModel];

	memcpy(header, image.header, 14);
	memcpy(info, image.info, infoLogSize);
	memcpy(data, image.data, width * height * colorModel);
}


void BMP::loadImage(const std::string& imagePath, const std::string& imageName) {
	if (header != nullptr)
		delete[] header;
	if (info != nullptr)
		delete[] info;
	if (data != nullptr)
		delete[] data;


	std::string buffer = imagePath + imageName + ".bmp";

	FILE* file;
	file = fopen(buffer.c_str(), "rb");
	if (file == nullptr)
		throw std::runtime_error("ERROR::FILE_NOT_FOUND");


	header = new unsigned char[14];
	fread(header, sizeof(unsigned char), 14, file);

	//width = *(int*)&info[18];
	//height = abs(*(int*)&info[22]);
	//bitsPerPixel = (BMP_FORMAT)(*(int*)&info[28]);

	infoLogSize = header[10] + 256 * header[11] - 14; // minus header size

	info = new unsigned char[infoLogSize];
	fread(info, sizeof(unsigned char), infoLogSize, file);

	width = info[4] + 256 * info[5];
	height = info[8] + 256 * info[9];
	bitsPerPixel = (BMP_FORMAT)(info[14] + 256 * info[15]);

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
		throw std::runtime_error("ERROR::LOAD_IMAGE::THIS_BMP_ISN'T_SUPPORTED");

	data = new unsigned char[width * height * colorModel];
	unsigned char* offsetBuffer = new unsigned char[offset];
	int readingOffset = 0;
	for (int i = 0; i < height; ++i) {
		fread(data + readingOffset, sizeof(unsigned char), width * colorModel, file);
		fread(offsetBuffer, sizeof(unsigned char), offset, file);
		readingOffset += width * colorModel;
	}
	if(offsetBuffer != nullptr)
		delete[] offsetBuffer;

	fclose(file);
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
	if (x < 0 || y < 0 || x >= width || y >= height) {
		throw std::runtime_error("ERROR::GET_PIXEL::INDEX_OUT_OF_RANGE");
	}

	return rgba(data[(y*width + x) * colorModel + 2], data[(y*width + x) * colorModel + 1], data[(y*width + x) * colorModel + 0], colorModel == 4 ? data[(y*width + x) * colorModel + 3] : 0);
}
void BMP::setPixel(int x, int y, rgba color) {
	if (x < 0 || y < 0 || x >= width || y >= height)
		throw std::runtime_error("ERROR::SET_PIXEL::INDEX_OUT_OF_RANGE");

	data[(y*width + x) * colorModel + 2] = color.r;
	data[(y*width + x) * colorModel + 1] = color.g;
	data[(y*width + x) * colorModel + 0] = color.b;
	if (colorModel == 4) data[(y*width + x) * colorModel + 3] = color.a;
}

void BMP::save() {
	if (imagePath == "")
		saveAs(imageName);
	else
		saveAs(imagePath + imageName);
}

void BMP::saveAs(const std::string& newImageName) {

	if (data == nullptr || info == nullptr || header == nullptr)
		throw std::runtime_error("ERROR::SAVEAS::FILE_DOESN'T_EXIST");

	std::string name = newImageName + ".bmp";

	FILE* file;
	file = fopen(name.c_str(), "wb"); // WARNING: ONLY 'WB', not 'W'. It will create broken image

	fwrite(header, sizeof(unsigned char), 14, file);
	fwrite(info, sizeof(unsigned char), infoLogSize, file);

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
	if (imagePath == "")
		bmp = "start " + imageName + ".bmp";
	else
		bmp = "start " + imagePath + imageName + ".bmp";
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

	if (header != nullptr)
		delete[] header;
	if (info != nullptr)
		delete[] info;
	if (data != nullptr)
		delete[] data;
}

unsigned char* BMP::getData() const {
	return data;
}


void BMP::resize(int newHeight, int newWidth) {
	unsigned char* newData = new unsigned char[newHeight * newWidth * colorModel];

	memset(newData, 200, newHeight * newWidth * colorModel);

	int minW = std::min(newWidth, width), minH = std::min(newHeight, height);



	for (int i = 0; i < minH; ++i) {
		for (int j = 0; j < minW; ++j) {
			newData[(i * newWidth + j) * colorModel + 0] = data[(i * width + j) * colorModel + 0];
			newData[(i * newWidth + j) * colorModel + 1] = data[(i * width + j) * colorModel + 1];
			newData[(i * newWidth + j) * colorModel + 2] = data[(i * width + j) * colorModel + 2];
			if(colorModel == 4)
				newData[(i * newWidth + j) * colorModel + 3] = data[(i * width + j) * colorModel + 3];
		}
	}

	delete[] data;
	data = newData;

	width = newWidth;
	height = newHeight;


	info[4] = unsigned char(width);
	info[5] = unsigned char(width >> 8);
	info[6] = unsigned char(width >> 16);
	info[7] = unsigned char(width >> 24);

	info[8] = unsigned char(height);
	info[9] = unsigned char(height >> 8);
	info[10] = unsigned char(height >> 16);
	info[11] = unsigned char(height >> 24);


	int imageSize = colorModel * width * height + 54 + 2;

	header[2] = unsigned char(imageSize);
	header[3] = unsigned char(imageSize >> 8);
	header[4] = unsigned char(imageSize >> 16);
	header[5] = unsigned char(imageSize >> 24); 


	if (bitsPerPixel == BMP24)
		offset = abs(((width * 3 + 3) & (~3)) - width * 3);
	else offset = 0;


	info[20] = unsigned char(width*height * colorModel + offset * height + 2);
	info[21] = unsigned char((width*height * colorModel + offset * height + 2) >> 8);
	info[22] = unsigned char((width*height * colorModel + offset * height + 2) >> 16);
	info[23] = unsigned char((width*height * colorModel + offset * height + 2) >> 24);
}

void open(const std::string& imageName) {
	std::string bmp;
	bmp = "start " + imageName + ".bmp";
	system(bmp.c_str());
}

BMP* createBMPDynamic(const int width, const int height, const std::string& imagePath, const std::string& imageName, const BMP_FORMAT format, const rgba fillColor) {
	if (format == NONE)
		return nullptr;

	FILE* file;

	std::string image;

	if (imagePath == "")
		image = imageName + ".bmp";
	else
		image = imagePath + imageName + ".bmp";

	file = fopen(image.c_str(), "wb");

	int offset = 0;
	long long imageSize = (format == 24 ? 3 : 4) * width * height + 54 + 2;
	if (format == 24)
		offset = abs(((width * 3 + 3) & (~3)) - width * 3);

	int colorModel = (format == BMP24 ? 3 : 4);

	unsigned char* header = new unsigned char[14];
	unsigned char* info = new unsigned char[40];
	memset(header, 0, 14);
	memset(info, 0, 40);

	header[0] = unsigned char('B');
	header[1] = unsigned char('M');

	header[2] = unsigned char(imageSize);
	header[3] = unsigned char(imageSize >> 8);
	header[4] = unsigned char(imageSize >> 16);
	header[5] = unsigned char(imageSize >> 24);

	header[10] = unsigned char(54);
	info[0] = unsigned char(40);

	info[4] = unsigned char(width);
	info[5] = unsigned char(width >> 8);
	info[6] = unsigned char(width >> 16);
	info[7] = unsigned char(width >> 24);
	info[8] = unsigned char(height);
	info[9] = unsigned char(height >> 8);
	info[10] = unsigned char(height >> 16);
	info[11] = unsigned char(height >> 24);

	info[12] = unsigned char(1);

	info[14] = unsigned char(BMP_FORMAT(format));

	info[16] = unsigned char(0);

	info[20] = unsigned char(width*height * colorModel + offset*height + 2);
	info[21] = unsigned char((width*height * colorModel + offset*height + 2) >> 8);
	info[22] = unsigned char((width*height * colorModel + offset*height + 2) >> 16);
	info[23] = unsigned char((width*height * colorModel + offset*height + 2) >> 24);

	info[24] = unsigned char(2834);
	info[25] = unsigned char(2834 >> 8);

	info[28] = unsigned char(2834);
	info[29] = unsigned char(2834 >> 8);

	info[32] = unsigned char(0);
	info[36] = unsigned char(0);

	fwrite(header, sizeof(unsigned char), 14, file);
	fwrite(info, sizeof(unsigned char), 40, file);

	unsigned char* data = new unsigned char[width * height * colorModel];
	unsigned char* offsetBuffer = new unsigned char[offset];
	memset(offsetBuffer, 0, offset);

	int readingOffset = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; j++) {
			data[(i*width + j)*colorModel + 0] = fillColor.b;
			data[(i*width + j)*colorModel + 1] = fillColor.g;
			data[(i*width + j)*colorModel + 2] = fillColor.r;
			if (colorModel == 4)
				data[(i*width + j)*colorModel + 3] = fillColor.a;
		}
		fwrite(data + readingOffset, sizeof(unsigned char), width * colorModel, file);
		fwrite(offsetBuffer, sizeof(unsigned char), offset, file);
		readingOffset += width * colorModel;
	}
	delete[] offsetBuffer;
	offsetBuffer = new unsigned char[2];
	memset(offsetBuffer, 0, 2);
	fwrite(offsetBuffer, sizeof(unsigned char), 2, file);
	delete[] offsetBuffer;

	fclose(file);

	BMP* bmp = new BMP(imagePath, imageName);
	return bmp;
}


BMP createBMP(const int width, const int height, const std::string& imagePath, const std::string& imageName, const BMP_FORMAT format, const rgba fillColor) {
	if (format == NONE)
		return BMP();

	FILE* file;

	std::string image;

	if (imagePath == "")
		image = imageName + ".bmp";
	else
		image = imagePath + imageName + ".bmp";

	file = fopen(image.c_str(), "wb");

	int offset = 0;
	long long imageSize = (format == 24 ? 3 : 4) * width * height + 54 + 2;
	if (format == 24)
		offset = abs(((width * 3 + 3) & (~3)) - width * 3);

	int colorModel = (format == BMP24 ? 3 : 4);

	unsigned char* header = new unsigned char[14];
	unsigned char* info = new unsigned char[40];
	memset(header, 0, 14);
	memset(info, 0, 40);

	header[0] = unsigned char('B');
	header[1] = unsigned char('M');

	header[2] = unsigned char(imageSize);
	header[3] = unsigned char(imageSize >> 8);
	header[4] = unsigned char(imageSize >> 16);
	header[5] = unsigned char(imageSize >> 24);

	header[10] = unsigned char(54);
	info[0] = unsigned char(40);

	info[4] = unsigned char(width);
	info[5] = unsigned char(width >> 8);
	info[6] = unsigned char(width >> 16);
	info[7] = unsigned char(width >> 24);
	info[8] = unsigned char(height);
	info[9] = unsigned char(height >> 8);
	info[10] = unsigned char(height >> 16);
	info[11] = unsigned char(height >> 24);

	info[12] = unsigned char(1);

	info[14] = unsigned char(BMP_FORMAT(format));

	info[16] = unsigned char(0);

	info[20] = unsigned char(width*height * colorModel + offset*height + 2);
	info[21] = unsigned char((width*height * colorModel + offset*height + 2) >> 8);
	info[22] = unsigned char((width*height * colorModel + offset*height + 2) >> 16);
	info[23] = unsigned char((width*height * colorModel + offset*height + 2) >> 24);

	info[24] = unsigned char(2834);
	info[25] = unsigned char(2834 >> 8);

	info[28] = unsigned char(2834);
	info[29] = unsigned char(2834 >> 8);

	info[32] = unsigned char(0);
	info[36] = unsigned char(0);

	fwrite(header, sizeof(unsigned char), 14, file);
	fwrite(info, sizeof(unsigned char), 40, file);

	unsigned char* data = new unsigned char[width * height * colorModel];
	unsigned char* offsetBuffer = new unsigned char[offset];
	memset(offsetBuffer, 0, offset);

	int readingOffset = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; j++) {
			data[(i*width + j)*colorModel + 0] = fillColor.b;
			data[(i*width + j)*colorModel + 1] = fillColor.g;
			data[(i*width + j)*colorModel + 2] = fillColor.r;
			if (colorModel == 4)
				data[(i*width + j)*colorModel + 3] = fillColor.a;
		}
		fwrite(data + readingOffset, sizeof(unsigned char), width * colorModel, file);
		fwrite(offsetBuffer, sizeof(unsigned char), offset, file);
		readingOffset += width * colorModel;
	}
	delete[] offsetBuffer;
	offsetBuffer = new unsigned char[2];
	memset(offsetBuffer, 0, 2);
	fwrite(offsetBuffer, sizeof(unsigned char), 2, file);
	delete[] offsetBuffer;

	fclose(file);

	return BMP(imagePath, imageName);
}