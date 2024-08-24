#include "PlayerUI.h"

PlayerUI::PlayerUI() { Init();}
PlayerUI::~PlayerUI() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Init() {
	hp_outsideTextureHandle_ = TextureManager::Load("./Resources/UI/Player/HP_outside.png");
	hp_barTextureHandle_ = TextureManager::Load("./Resources/UI/Player/HP_bar.png");

	uiPosition_ = { 100, 630 };

	Sprite* outside = Sprite::Create(hp_outsideTextureHandle_, uiPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f });
	Sprite* bar = Sprite::Create(hp_barTextureHandle_, uiPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f });

	hp_outside_ = std::unique_ptr<Sprite>(outside);
	hp_bar_ = std::unique_ptr<Sprite>(bar);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Update() {
	EditImGui();

	hp_outside_->SetPosition(uiPosition_);
	hp_bar_->SetPosition(uiPosition_);
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
	ImGui::End();
#endif // _DEBUG
}
