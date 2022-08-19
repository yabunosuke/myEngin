#pragma once
#include <list>
#include <memory>

class LightComponent;

class LightManager
{
public:
	LightManager(){};

	// ���C�g�R���|�[�l���g�����X�g�ɒǉ�
	void AddLight(std::shared_ptr<LightComponent> light);
	// ���C�g�̍폜����
	void RemoveLight(std::shared_ptr<LightComponent> light);


private:
	std::list<std::weak_ptr<LightComponent>> light_list_;
};

