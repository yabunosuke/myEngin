#pragma once
#include "ConstantBufferManager/ConstantBufferManager.h"
class SystemMain final
{
public:
	bool Initialize();
	void Finalize() const;
	void main() const;

private:
	// �V���O���g���̐���
	ConstantBufferManager *constant_buffer_manager_ = nullptr;	// �R���X�^���g�o�b�t�@�}�l�[�W���[
};

