#include "EnemyManager.h"

EnemyManager::EnemyManager() { Init(); }
EnemyManager::~EnemyManager() {}

///////////////////////////////////////////////////////////
// 初期化関数
///////////////////////////////////////////////////////////

void EnemyManager::Init() {
	ModelLoader* modelLoader = ModelLoader::GetInstacne();
	mobEnemyPartsModels_.emplace_back(modelLoader->GetModel("mobEnemy"));

	AllLoadFilesName();
	currentIndex_ = 0;
}

///////////////////////////////////////////////////////////
// 更新関数
///////////////////////////////////////////////////////////

void EnemyManager::Update() {
	
	ImGuiEdit();

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

	// --------------------------------------
	// 親を登録する
	// --------------------------------------
	for (const std::unique_ptr<BaseEnemy>& enemy : enemysList_) {
		enemy->SetParent(parentWorldTransform_);
	}
}

void EnemyManager::AllLoadFilesName() {
	const std::string directoryPath = "./Resources/EnemyPos/";

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
		// 拡張子が .json のファイルを探す
		if (entry.is_regular_file() && entry.path().extension() == ".json") {
			// ファイル名を vector に追加
			filePathArray_.push_back(entry.path().filename().string());
		}
	}
}

void EnemyManager::ImGuiEdit() {
#ifdef _DEBUG
	ImGui::Begin("EnemyManager");
	// --------------------------------------
	// 読み込みたいファイルを選択する
	// --------------------------------------
	if (ImGui::BeginCombo("Select File", filePathArray_[currentIndex_].c_str())) {
		// json_files の各要素をプルダウンのアイテムとして追加
		for (uint32_t i = 0; i < filePathArray_.size(); ++i) {
			bool is_selected = (currentIndex_ == i); // 現在のアイテムが選択されているかどうか
			if (ImGui::Selectable(filePathArray_[i].c_str(), is_selected)) {
				currentIndex_ = i; // 新しいアイテムが選択されたら、current_item を更新
			}

			// 現在の選択がまだ続いている場合はアイテムを強調表示
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	// --------------------------------------
	// 実際に読み込む
	// --------------------------------------
	if (ImGui::Button("Load")) {
		nowWaveEnemyPos_ = currentIndex_;
		enemysList_.clear();
		LoadFile();
	}

	ImGui::End();
#endif
}


///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

void EnemyManager::SetParent(const WorldTransform* parent) {
	parentWorldTransform_ = parent;
}
