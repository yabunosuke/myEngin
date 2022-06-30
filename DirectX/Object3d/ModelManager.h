#pragma once
#include <map>
#include <string>
#include "Model.h"

class ModelManager final
{
private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager &r) = default;
	ModelManager &operator= (const ModelManager &r) = default;

public:
	static ModelManager *GetIns();		//�擾�p

	void AllLoad();
	void LoadModel(std::string fileName);
	Model *GetModel(std::string modelName) { return model[modelName].get(); }

private:
	static std::map<const std::string , std::unique_ptr<Model>> model;	//���f���i�[�}�b�v
};

