#pragma once
#include "Model.h"
// STL
#include <string>
#include <map>
#include <memory>

class ModelLoader {
public:

	ModelLoader() = default;
	~ModelLoader();
	ModelLoader(const ModelLoader&) = delete;
	const ModelLoader& operator=(const ModelLoader&) = delete;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ModelLoader* GetInstacne();

	void Finalize();

	void Init();

	/// <summary>
	/// ObjectFileを読み込む
	/// </summary>
	/// <param name="modelPath">objのパス</param>
	/// <param name="isSmooth">平滑化するか</param>
	void Load(const std::string& modelPath, const bool& isSmooth);

	/// <summary>
	/// モデルのポインタを返す
	/// </summary>
	/// <param name="modelName">モデルの名前</param>
	/// <returns>モデルのポインタ</returns>
	Model* GetModel(const std::string& modelName) const {
		auto it = modelMap_.find(modelName);
		if (it != modelMap_.end()) {
			return it->second.get(); // unique_ptr から生のポインタを取得
		}
		return nullptr;
	}

private:

	// モデルのポインタをまとめた変数
	std::map<std::string, std::unique_ptr<Model>> modelMap_;

};