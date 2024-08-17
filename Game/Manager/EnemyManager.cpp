#include "EnemyManager.h"

EnemyManager::EnemyManager() { Init(); }
EnemyManager::~EnemyManager() {}

///////////////////////////////////////////////////////////
// 初期化関数
///////////////////////////////////////////////////////////

void EnemyManager::Init() {
	ModelLoader* modelLoader = ModelLoader::GetInstacne();
	mobEnemyPartsModels_.emplace_back(modelLoader->GetModel("mobEnemy"));

	// インスタンス生成と初期化
	LoadFile();
}

///////////////////////////////////////////////////////////
// 更新関数
///////////////////////////////////////////////////////////

void EnemyManager::Update() {
	for (const std::unique_ptr<BaseEnemy>& enemy : enemysList_) {
		enemy->Update();
	}
}

///////////////////////////////////////////////////////////
// 描画関数
///////////////////////////////////////////////////////////

void EnemyManager::Draw(const ViewProjection& viewProjection) const {
	for (const std::unique_ptr<BaseEnemy>& enemy : enemysList_) {
		enemy->Draw(viewProjection);
	}
}

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

// ------------------- 敵を出現させるファイルを読み込む関数 ------------------- //

void EnemyManager::LoadFile() {
	// --------------------------------------
	// ファイルの読み込みを行う
	// --------------------------------------
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectorPath_ + "enemyPos" + std::to_string(nowWaveEnemyPos_) + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);
	// json文字列からjsonのデータ構造に展開
	json root;
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// --------------------------------------
	// 読み込んだデータからEnemyを生成する
	// --------------------------------------

	for (auto& element : root.items()) {
		std::string enemyName = element.key();
		json enemyData = element.value();

		// 位置データを取得
		Vector3 pos = { enemyData["pos"].at(0), enemyData["pos"].at(1),enemyData["pos"].at(2), };
		
		// タイプデータを取得
		int type = enemyData["type"][0];

		// 
		switch (type) {
		case EnemyType::Type_Mob:
			enemysList_.push_back(std::make_unique<MobEnemy>(mobEnemyPartsModels_, pos));
			break;

		case EnemyType::Type_MidBoss:

			break;

		case EnemyType::Type_Boss:

			break;
		}
	}
}


///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

void EnemyManager::SetParent(const WorldTransform* parent) {
	for (const std::unique_ptr<BaseEnemy>& enemy : enemysList_) {
		enemy->SetParent(parent);
	}
}
