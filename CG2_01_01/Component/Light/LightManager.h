#pragma once
#include <list>
#include <memory>

class LightComponent;

class LightManager
{
public:
	LightManager(){};

	// ライトコンポーネントをリストに追加
	void AddLight(std::shared_ptr<LightComponent> light);
	// ライトの削除処理
	void RemoveLight(std::shared_ptr<LightComponent> light);


private:
	std::list<std::weak_ptr<LightComponent>> light_list_;
};

