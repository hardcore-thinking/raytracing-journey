#ifndef RTW_STB_IMAGE_HPP
#define RTW_STB_IMAGE_HPP

#include <cstdlib>
#include <iostream>

class RTWImage {
	public:
		RTWImage();
		RTWImage(char* const imageFilename);
		~RTWImage();

		bool Load(std::string const& filename);
		int Width() const;
		int Height() const;
		unsigned char* const PixelData(int x, int y) const;
	
	private:
		int const      _bytesPerPixel     = 3;
		float*         _fData             = nullptr;
		unsigned char* _bData             = nullptr;
		int            _imageWidth        = 0;
		int            _imageHeight       = 0;
		int            _bytesPerScanlines = 0;

		static int Clamp(int x, int low, int high);
		static unsigned char FloatToByte(float value);
		void ConvertToBytes();
};

#endif // RTW_STB_IMAGE_HPP