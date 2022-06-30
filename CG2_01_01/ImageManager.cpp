#include "ImageManager.h"

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
}

ImageManager *ImageManager::GetIns()
{
	static ImageManager ins;
	return &ins;
}

void ImageManager::LoadImagALL(ID3D12Device *dev) {
	//Sprite::LoadTexture(DebugText, L"Resources/debugfont.png", dev);
	
}
