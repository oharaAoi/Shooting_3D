#include "ModelLoader.h"

ModelLoader::ModelLoder() {
	Init();
}

ModelLoader::~ModelLoder() {
}

void ModelLoader::Init() {
	Load("skydome", true);
}

// ===========================================================
// モデルの読み込み
// ===========================================================

void ModelLoader::Load(const std::string& objPath, const bool& isSmooth) {
	std::unique_ptr<Model> model;
	model.reset(Model::CreateFromOBJ("objPath", isSmooth));
	modelMap_.emplace(std::move(model));
}
