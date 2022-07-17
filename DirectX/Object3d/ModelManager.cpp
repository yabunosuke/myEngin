#include "ModelManager.h"
#include "ModelLoader.h"

std::map<const std::string, std::unique_ptr<Model>> ModelManager::model;

ModelManager *ModelManager::GetIns()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::AllLoad()
{
	LoadModel("ball");
}

void ModelManager::LoadModel(std::string fileName)
{
	//model[fileName] = ModelLoader::GetInstance()->LoadModelFromFile(fileName);
	
}
