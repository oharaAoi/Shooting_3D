#include "ModelLoader.h"

ModelLoader::ModelLoader() { Init(); }
ModelLoader::~ModelLoader() {}

//=============================================
// 初期化処理
//=============================================

void ModelLoader::Init() {
	Load("skydome", true);
}

//=============================================
// モデルの読み込みを行う
//=============================================

void ModelLoader::Load(const std::string& modelPath, const bool& isSmooth) {
	std::unique_ptr<Model> model;
	model.reset(Model::CreateFromOBJ(modelPath, isSmooth));
	modelMap_.emplace(modelPath, std::move(model));
}
