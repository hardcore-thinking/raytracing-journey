#include "RTWSTBImage.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "external/stb_image.h"

#define ENV_STRING_SIZE 250

RTWImage::RTWImage() {}

RTWImage::RTWImage(char* const imageFilename) {
	// Loads image data from the specified file. If the RTW_IMAGES environment variable is
	// defined, looks only in that directory for the image file. If the image was not found,
	// searches for the specified image file first from the current directory, then in the 
	// images/ subdirectory, then the _parent's_ images/ subdirectory, and then _that_
	// parent, on so on, for six levels up. If the image was not loaded successfully,
	// width() and height() will return 0.

	auto filename = std::string(imageFilename);

	char* imageDir = new char[ENV_STRING_SIZE];
	size_t envStrSize = ENV_STRING_SIZE;
	_dupenv_s(&imageDir, &envStrSize, "RTW_IMAGES");

	// Hunt for the image file in some likely locations
	if (imageDir && Load(std::string(imageDir) + "/" + imageFilename)) {
		return;
	}
	
	if (Load(filename)) {
		return;
	}

	if (Load("images/" + filename)) {
		return;
	}
	
	if (Load("../images" + filename)) {
		return;
	}

	if (Load("../../images" + filename)) {
		return;
	}

	if (Load("../../../images" + filename)) {
		return;
	}

	if (Load("../../../../images" + filename)) {
		return;
	}

	if (Load("../../../../../images" + filename)) {
		return;
	}

	if (Load("../../../../../../images" + filename)) {
		return;
	}

	std::cerr << "ERROR: Could not load image file '" << imageFilename << "'.\n";
}

RTWImage::~RTWImage() {
	delete[] _bData;
	STBI_FREE(_fData);
}

bool RTWImage::Load(std::string const& filename) {
	// Loads the linear (gamma = 1) image data from the given file name. Return true if the
	// load succeeded. The resulting data buffer contains the three [0.0, 1.0]
	// floating-point values for the first pixel (red, then green, then blue). Pixels are
	// contiguous, going left to right for the width of the image, followed by the next row
	// below, for the full height of the image.

	auto n = _bytesPerPixel; // Dummy out parameter: original components per pixel
	_fData = stbi_loadf(filename.c_str(), &_imageWidth, &_imageHeight, &n, _bytesPerPixel);

	if (_fData == nullptr) {	
		return false;
	}

	_bytesPerScanlines = _imageWidth * _bytesPerPixel;
	ConvertToBytes();

	return true;
}

int RTWImage::Width() const {
	return _imageWidth;
}

int RTWImage::Height() const {
	return _imageHeight;
}

unsigned char* const RTWImage::PixelData(int x, int y) const {
	// Returns the address of the three RGB bytes of the pixel at x,y. If the is no image
	// data, returns magenta.

	static unsigned char magenta[] = { 255, 0, 255 };
	
	if (_bData == nullptr) {
		return magenta;
	}

	x = Clamp(x, 0, _imageWidth);
	y = Clamp(y, 0, _imageHeight);

	return _bData + y * _bytesPerScanlines + x * _bytesPerPixel;
}

int RTWImage::Clamp(int x, int low, int high) {
	// Returns the value clamped to the range [low, high)

	if (x < low) {
		return low;
	}

	if (x < high) {
		return x;
	}

	return high - 1;
}

unsigned char RTWImage::FloatToByte(float value) {
	if (value <= 0.0) {
		return 0;
	}

	if (value >= 1.0) {
		return 255;
	}

	return static_cast<unsigned char>(256.0 * value);
}

void RTWImage::ConvertToBytes() {
	// Convert the linear floating point pixel data to bytes, storing the resulting byte
	// data in the '_bData' member.

	int totalBytes = _imageWidth * _imageHeight * _bytesPerPixel;
	_bData = new unsigned char(totalBytes);

	// Iterate through all pixel components, converting from [0.0, 1.0] float values to
	// unsigned [0, 255] byte values.
	
	auto* bPtr = _bData;
	auto* fPtr = _fData;
	for (auto i = 0; i < totalBytes; i++, fPtr++, bPtr++) {
		*bPtr = FloatToByte(*fPtr);
	}
}