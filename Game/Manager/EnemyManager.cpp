#include "EnemyManager.h"
#include "GameScene.h"

EnemyManager::EnemyManager() { Init(); }
EnemyManager::~EnemyManager() {}

///////////////////////////////////////////////////////////
// 初期化関数
///////////////////////////////////////////////////////////

void EnemyManager::Init() {
	ModelLoader* modelLoader = ModelLoader::GetInstacne();
	mobEnemyPartsModels_.emplace_back(modelLoader->GetModel("mobEnemy"));
	midEnemyPartsModels_.emplace_back(modelLoader->GetModel("midEnemy"));
	bossEnemyPartsModels_.emplace_back(modelLoader->GetModel("bossEnemy"));
	bossEnemyPartsModels_.emplace_back(modelLoader->GetModel("bossEnemyLEye"));
	bossEnemyPartsModels_.emplace_back(modelLoader->GetModel("bossEnemyREye"));

	currentIndex_ = 0;
	fileNum_ = 0;

	// ファイルを読み込む
	AllLoadFilesName();

	nowWaveEnemyPos_ = 0;
	LoadFile();

	bossEnemy_ = enemysList_.front().get();
}

///////////////////////////////////////////////////////////
// 更新関数
///////////////////////////////////////////////////////////

void EnemyManager::Update() {

	//EnemyPop();
	
	ImGuiEdit();

	EditEnemyPos();

	enemysList_.remove_if([](const std::unique_ptr<BaseEnemy>& enemy) {
		if (enemy->GetIsDead()) {
			return true;
		}
		return false;
	});

	for (const std::unique_ptr<BaseEnemy>& enemy : enemysList_) {
		enemy->SetPlayerPosition(playerPosition_);
		enemy->SetGameScene(gameScene_);
		enemy->Update();
	}

	for (const std::unique_ptr<BaseEnemy>& enemy : createEnemysList_) {
		enemy->SetGameScene(gameScene_);
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

	for (const std::unique_ptr<BaseEnemy>& enemy : createEnemysList_) {
		enemy->Draw(viewProjection);
	}
}

void EnemyManager::DrawCollisions() {
	for (const std::unique_ptr<BaseEnemy>& enemy : enemysList_) {
		enemy->DrawCollision();
	}

	for (const std::unique_ptr<BaseEnemy>& enemy : createEnemysList_) {
		enemy->DrawCollision();
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
		json enemyData = element.value();;

		// 位置データを取得
		Vector3 pos = { enemyData["pos"].at(0), enemyData["pos"].at(1),enemyData["pos"].at(2), };
		// タイプデータを取得
		int type = enemyData["type"][0];
		// 速さ
		Vector3 velocity = { enemyData["velocity"].at(0), enemyData["velocity"].at(1),enemyData["velocity"].at(2), };
		
		switch (type) {
		case EnemyType::Type_Mob:
			enemysList_.push_back(std::make_unique<MobEnemy>(mobEnemyPartsModels_, pos, velocity));
			break;

		case EnemyType::Type_MidEnemy:
			enemysList_.push_back(std::make_unique<MidMobEnemy>(midEnemyPartsModels_, pos, velocity));
			break;

		case EnemyType::Type_Boss:
			enemysList_.push_back(std::make_unique<BossEnemy>(bossEnemyPartsModels_, pos, velocity));
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

// ------------------- Enemyの配置のファイル名をすべて配列に格納する ------------------- //

void EnemyManager::AllLoadFilesName() {
	filePathArray_.clear();

	for (const auto& entry : std::filesystem::directory_iterator(kDirectorPath_)) {
		// 拡張子が .json のファイルを探す
		if (entry.is_regular_file() && entry.path().extension() == ".json") {
			// ファイル名を vector に追加
			filePathArray_.push_back(entry.path().filename().string());
		}

		fileNum_++;
	}
}

// ------------------- Enemyの配置をファイルに保存する ------------------- //

void EnemyManager::SaveEnemyPos() {
	json data = json::object();

	uint32_t createListIndex = 0;
	for (const std::unique_ptr<BaseEnemy>& enemy : createEnemysList_) {
		std::string enemyNum = "Enemy" + std::to_string(createListIndex);
		Vector3 enemyPos = enemy->GetWorldTransform().translation_;
		uint32_t enemyType = enemy->GetEnemyType();
		Vector3 enemyVelocity = enemy->GetVelocity();

		data[enemyNum]["pos"] = { enemyPos.x, enemyPos.y, enemyPos.z };
		data[enemyNum]["type"] = { enemyType };
		data[enemyNum]["velocity"] = { enemyVelocity.x, enemyVelocity.y, enemyVelocity.z };

		createListIndex++;
	}

	// パス
	std::string filePath = kDirectorPath_ + "EnemyPos" + std::to_string(fileNum_) + ".json";
	// 書き込み用のファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープンが出来ているか
	if (ofs.fail()) {
		std::string message = "Faild open data file for write";
		MessageBoxA(nullptr, message.c_str(), "Adjustment", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << data.dump(4) << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();
}

// ------------------- Enemyの配置を作成する ------------------- //

void EnemyManager::EditEnemyPos() {
#ifdef _DEBUG
	ImGui::Begin("EnemyManager");

	// 敵の種類を選択する
	ImGui::RadioButton("mob", &createEnemyType_, 0);
	ImGui::SameLine();
	ImGui::RadioButton("midMob", &createEnemyType_, 1);
	ImGui::SameLine();
	ImGui::RadioButton("boss", &createEnemyType_, 2);

	// 敵の座標を指定する
	ImGui::DragFloat3("position", &createEnemyPos_.x, 0.01f);

	// 敵の速さを指定する
	ImGui::DragFloat3("velocity", &createEnemyVelocity_.x, 0.01f);

	// 敵をリストに追加する
	if (ImGui::Button("Create")) {
		switch (createEnemyType_) {
		case EnemyType::Type_Mob:
			createEnemysList_.push_back(std::make_unique<MobEnemy>(mobEnemyPartsModels_, createEnemyPos_, createEnemyVelocity_));
			break;

		case EnemyType::Type_MidEnemy:
			createEnemysList_.push_back(std::make_unique<MidMobEnemy>(midEnemyPartsModels_, createEnemyPos_, createEnemyVelocity_));
			break;

		case EnemyType::Type_Boss:
			createEnemysList_.push_back(std::make_unique<BossEnemy>(bossEnemyPartsModels_, createEnemyPos_, createEnemyVelocity_));
			break;
		}
	}

	ImGui::Spacing();

	// リストの中を表示
	uint32_t createListIndex = 0;
	for (auto it = createEnemysList_.begin(); it != createEnemysList_.end(); /* no increment here */) {
		std::string label = "Enemy " + std::to_string(createListIndex);
		if (ImGui::BeginMenu(label.c_str())) {
			(*it)->ImGuiSetTranslation();
			if (ImGui::Button("Delete")) {
				it = createEnemysList_.erase(it);  // 要素を削除し、イテレーターを更新
				ImGui::EndMenu(); // メニューを閉じる
				continue; // 次の要素に進む
			}
			ImGui::EndMenu();
		}
		(*it)->SetParent(parentWorldTransform_);
		++it; // イテレーターを次に進める
		createListIndex++;
	}

	ImGui::Spacing();

	// リストの中身をファイルに保存する
	if (ImGui::Button("Save")) {
		SaveEnemyPos();
		AllLoadFilesName();
		createEnemysList_.clear();
	}

	ImGui::End();
#endif
}

// ------------------- 敵を出現させる ------------------- //

void EnemyManager::EnemyPop() {
	popTime_++;
	if (popTime_ > popTimeLimit_) {
		createEnemyType_ = 0;
		switch (createEnemyType_) {
		case EnemyType::Type_Mob:
			enemysList_.push_back(std::make_unique<MobEnemy>(mobEnemyPartsModels_, createEnemyPos_, createEnemyVelocity_));
			break;

		case EnemyType::Type_MidEnemy:
			enemysList_.push_back(std::make_unique<MidMobEnemy>(midEnemyPartsModels_, createEnemyPos_, createEnemyVelocity_));
			break;

		case EnemyType::Type_Boss:
			enemysList_.push_back(std::make_unique<BossEnemy>(bossEnemyPartsModels_, createEnemyPos_, createEnemyVelocity_));
			break;
		}
		popTime_ = 0;
	}
}

// ------------------- ImGuiを配置する ------------------- //

void EnemyManager::ImGuiEdit() {
#ifdef _DEBUG
	ImGui::Begin("EnemyManager");
	if (ImGui::Button("ListClear")) {
		enemysList_.clear();
	}

	ImGui::Separator();

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
		//enemysList_.clear();
		LoadFile();
	}

	ImGui::Separator();

	ImGui::End();
#endif
}


///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

void EnemyManager::SetParent(const WorldTransform* parent) {
	parentWorldTransform_ = parent;
}
