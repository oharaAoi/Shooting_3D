#include "Rader.h"

Rader::Rader() {Init();}
Rader::~Rader() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rader::Init() {
	baseTextureSize_ = { 256, 256 };
	base_.pos = { 1100, 570 };
	baseLight_.pos = base_.pos;
	baseLightColor_ = { 1,1,0,1 };
	baseCenterPos_ = { base_.pos.x + (baseTextureSize_.x / 2.0f), base_.pos.y + (baseTextureSize_.y / 2.0f) };

	// 基盤
	base_.textureHandle = TextureManager::Load("./Resources/UI/Rader/base.png");
	Sprite* base = Sprite::Create(base_.textureHandle, base_.pos, { 1,1,1,1 }, { 0.5f, 0.5f });
	base_.sprite = std::unique_ptr<Sprite>(base);

	// 基盤の光
	baseLight_.textureHandle = TextureManager::Load("./Resources/UI/Rader/baseLight.png");
	Sprite* baseLight = Sprite::Create(baseLight_.textureHandle, baseLight_.pos, { 1,1,1,1 }, { 0.5f, 0.5f });
	baseLight_.sprite = std::unique_ptr<Sprite>(baseLight);

	// プレイヤーの位置
	player_.textureHandle = TextureManager::Load("./Resources/UI/Rader/player.png");
	Sprite* player = Sprite::Create(player_.textureHandle, player_.pos, { 1,1,1,1 }, { 0.5f, 0.5f });
	player_.sprite = std::unique_ptr<Sprite>(player);

	// プレイヤーの光
	playerDire_.textureHandle = TextureManager::Load("./Resources/UI/Rader/playerDire.png");
	Sprite* playerDire = Sprite::Create(playerDire_.textureHandle, player_.pos, { 1,1,1,1 }, { 0.5f, 0.5f });
	playerDire_.sprite = std::unique_ptr<Sprite>(playerDire);

	enemyTextureHandle_ = TextureManager::Load("./Resources/UI/Rader/enemy.png");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rader::Update(const uint32_t& playerNearEnemyCount, const uint32_t& playerAimCount,
				   std::list<std::unique_ptr<BaseEnemy>>& enemyList) {
	playerNearEnemyCount_ = playerNearEnemyCount;

	baseLightColor_.w = static_cast<float>(playerAimCount) / 10.0f;
	baseLightColor_.x = static_cast<float>(playerAimCount) / 10.0f;
	baseLightColor_.y = static_cast<float>(playerAimCount) / 10.0f;

	// -------------------------------------------------
	// ↓ playerの基盤上の位置を計算する
	// -------------------------------------------------
	CalculationPlayerPos();

	// -------------------------------------------------
	// ↓ 敵の位置を計算する
	// -------------------------------------------------
	CalculationEnemiesPos(enemyList);

	base_.sprite->SetPosition(base_.pos);
	baseLight_.sprite->SetPosition(base_.pos);
	baseLight_.sprite->SetColor(baseLightColor_);
	player_.sprite->SetPosition(player_.pos);
	playerDire_.sprite->SetPosition(player_.pos);
	playerDire_.sprite->SetRotation(std::atan2f(playerRotation_.x, playerRotation_.z));

	EditImGui();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rader::Draw() {
	base_.sprite->Draw();
	baseLight_.sprite->Draw();
	player_.sprite->Draw();
	playerDire_.sprite->Draw();

	for (std::map<BaseEnemy*, SpriteData>::iterator it = enemiesMap_.begin(); it != enemiesMap_.end(); ++it) {
		SpriteData& spriteData = it->second;
		spriteData.sprite->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ImGui
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rader::EditImGui() {
#ifdef _DEBUG

	ImGui::Begin("Rader");
	ImGui::DragFloat2("base", &base_.pos.x, 2.0f);
	ImGui::DragFloat4("baseLight", &baseLightColor_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- Playerの位置を求める ------------------- //

void Rader::CalculationPlayerPos() {
	// playerの基盤上の位置を計算する
	Vector2 worldRaito = { playerPosition_.x / (kWorldSize.x / 2.0f), playerPosition_.y / (kWorldSize.z / 2.0f) };
	worldRaito.x = std::clamp(worldRaito.x, -1.0f, 1.0f);
	worldRaito.y = std::clamp(worldRaito.y, -1.0f, 1.0f);
	// 基盤のsizeから左上座標から足す分を求める
	Vector2 baseRaito = { (baseTextureSize_.x / 2.0f) * worldRaito.x, (baseTextureSize_.y / 2.0f) * worldRaito.y };
	// 基盤上から出ないように
	float distance = Length(baseRaito);
	if (distance > (baseTextureSize_.x / 2.0f)) {
		baseRaito = Normalize(baseRaito) * (baseTextureSize_.x / 2.0f);
	}
	// 基盤の中心から足す分を追加してScreen座標を求める
	player_.pos = { base_.pos.x + baseRaito.x, base_.pos.y - baseRaito.y };
}

// ------------------- 敵の位置を計算する ------------------- //

void Rader::CalculationEnemiesPos(std::list<std::unique_ptr<BaseEnemy>>& enemyList) {
	for (const std::unique_ptr<BaseEnemy>& enemy : enemyList) {
		// キーが存在する場合
		if (enemiesMap_.find(enemy.get()) != enemiesMap_.end()) {
			Vector3 pos = Vector3(enemy->GetWorldTransform().translation_);
			Vector2 basePos = Vector2(pos.x, pos.z);
			// -------------------------------------------------
			// ↓ 座標の計算を行う
			// -------------------------------------------------
			// 基盤上の位置を計算する
			Vector2 worldRaito = { basePos.x / (kWorldSize.x / 2.0f), basePos.y / (kWorldSize.z / 2.0f) };
			worldRaito.x = std::clamp(worldRaito.x, -1.0f, 1.0f);
			worldRaito.y = std::clamp(worldRaito.y, -1.0f, 1.0f);
			// 基盤のsizeから左上座標から足す分を求める
			Vector2 baseRaito = { (baseTextureSize_.x / 2.0f) * worldRaito.x, (baseTextureSize_.y / 2.0f) * worldRaito.y };
			// 基盤上から出ないように
			float distance = Length(baseRaito);
			if (distance > (baseTextureSize_.x / 2.0f)) {
				baseRaito = Normalize(baseRaito) * (baseTextureSize_.x / 2.0f);
			}
			// 座標の更新を行う
			enemiesMap_[enemy.get()].pos = { base_.pos.x + baseRaito.x, base_.pos.y - baseRaito.y };
			enemiesMap_[enemy.get()].sprite->SetPosition(enemiesMap_[enemy.get()].pos);
		
			// キーが存在しない場合
		}else {
			Vector3 pos = Vector3(enemy->GetWorldTransform().translation_);
			Vector2 basePos = Vector2(pos.x, pos.y);
			// -------------------------------------------------
			// ↓ 座標の計算を行う
			// -------------------------------------------------
			// 基盤上の位置を計算する
			Vector2 worldRaito = { basePos.x / (kWorldSize.x / 2.0f), basePos.y / (kWorldSize.z / 2.0f) };
			worldRaito.x = std::clamp(worldRaito.x, -1.0f, 1.0f);
			worldRaito.y = std::clamp(worldRaito.y, -1.0f, 1.0f);
			// 基盤のsizeから左上座標から足す分を求める
			Vector2 baseRaito = { (baseTextureSize_.x / 2.0f) * worldRaito.x, (baseTextureSize_.y / 2.0f) * worldRaito.y };
			// 基盤上から出ないように
			float distance = Length(baseRaito);
			if (distance > (baseTextureSize_.x / 2.0f)) {
				baseRaito = Normalize(baseRaito) * (baseTextureSize_.x / 2.0f);
			}
			// 新しくデータを作成
			SpriteData data;
			data.pos = { base_.pos.x + baseRaito.x, base_.pos.y - baseRaito.y };
			Sprite* sprite = Sprite::Create(enemyTextureHandle_, data.pos, { 1,1,1,1 }, { 0.5f, 0.5f });
			data.sprite = std::unique_ptr<Sprite>(sprite);
			enemiesMap_.emplace(enemy.get(), std::move(data));
		}
	}
}
