#pragma once
#include "ConstantBufferLapper/ConstantBufferManager.h"
class SystemMain final
{
public:
	bool Initialize();
	void Finalize() const;
	void main() const;

private:
	// シングルトンの生成
	ConstantBufferManager *constant_buffer_manager_ = nullptr;	// コンスタントバッファマネージャー
};

