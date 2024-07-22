#pragma once
#include "Model.h"

// STL
#include <map>
#include <string>
#include <memory>

class ModelLoader {
public:

	ModelLoder();
	~ModelLoder();

	void Init();

	/// <summary>
	/// モデルをLoadする
	/// </summary>
	/// <param name="objPath">: モデル名</param>
	/// <param name="isSmooth">: 平滑化するか</param>
	void Load(const std::string& objPath, const bool& isSmooth);

	/// <summary>
	/// モデルのポインタを返す
	/// </summary>
	/// <param name="modelName">: モデルの名前</param>
	/// <returns>モデルのポインタ</returns>
	Model* GetModel(const std::string& modelName) {
		return modelMap_[modelName].get();
	}

private:

	std::map<std::string, std::unique_ptr<Model>> modelMap_;

};
