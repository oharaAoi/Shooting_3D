#include "PlayerUI.h"

PlayerUI::PlayerUI() { Init();}
PlayerUI::~PlayerUI() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Init() {
	hp_outsideTextureHandle_ = TextureManager::Load("./Resources/UI/Player/HP_outside.png");
	hp_barTextureHandle_ = TextureManager::Load("./Resources/UI/Player/HP_bar.png");

	uiPosition_ = { 185, 630 };
	barSize_ = { 256, 64 };

	Sprite* outside = Sprite::Create(hp_outsideTextureHandle_, uiPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f });
	Sprite* bar = Sprite::Create(hp_barTextureHandle_, uiPosition_ - Vector2(barSize_.x / 2.0f, barSize_.y / 2.0f), {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	hp_outside_ = std::unique_ptr<Sprite>(outside);
	hp_bar_ = std::unique_ptr<Sprite>(bar);
	hp_bar_->SetSize(barSize_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Update(const float& playerHpRaito) {
	EditImGui();

	Vector2 playerHp = { barSize_.x * playerHpRaito, barSize_.y };

	hp_bar_->SetTextureRect({ 0,0 }, playerHp);
	hp_bar_->SetSize(playerHp);
	
	hp_outside_->SetPosition(uiPosition_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Draw() const {
	hp_outside_->Draw();
	hp_bar_->Draw();
}

void PlayerUI::EditImGui() {
#ifdef _DEBUG
	ImGui::Begin("UI");
	ImGui::DragFloat2("PlayerUI_pos", &uiPosition_.x);
	ImGui::DragFloat2("barSize", &barSize_.x);
	ImGui::DragFloat("raito", &raito_, 0.1f);
	ImGui::End();
#endif // _DEBUG
}
