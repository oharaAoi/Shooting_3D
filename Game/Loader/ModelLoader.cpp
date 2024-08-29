#include "ModelLoader.h"

ModelLoader* ModelLoader::GetInstacne() {
	static ModelLoader instance;
	return &instance;
}

void ModelLoader::Finalize() {
	modelMap_.clear();
}

ModelLoader::~ModelLoader() {
	Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ModelLoader::Init() {
	Load("skydome", true);
	// player
	Load("player_body", true);
	Load("player_face", true);
	Load("player_leftArm", true);
	Load("player_rightArm", true);
	// enemy
	Load("mobEnemy", true);
	Load("midEnemy", true);
	// bullet
	Load("cube", true);
	Load("playerBullet", true);
	Load("bossBullet", true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ModelLoader::Load(const std::string& modelPath, const bool& isSmooth) {
	std::unique_ptr<Model> model;
	model.reset(Model::CreateFromOBJ(modelPath, isSmooth));
	modelMap_.emplace(modelPath, std::move(model));
}
