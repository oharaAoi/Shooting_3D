#include "PlayerUI.h"

PlayerUI::PlayerUI() { Init();}
PlayerUI::~PlayerUI() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Init() {
	hp_outsideTextureHandle_ = TextureManager::Load("./Resources/UI/Player/HP_outside.png");
	hp_barTextureHandle_ = TextureManager::Load("./Resources/UI/Player/HP_bar.png");

	stamina_outsideTextureHandle_ = TextureManager::Load("./Resources/UI/Player/stamina_outside.png");
	stamina_barTextureHandle_ = TextureManager::Load("./Resources/UI/Player/stamina_bar.png");

	uiPosition_ = { 185, 630 };
	barSize_ = { 256, 64 };

	staminaBarSize_ = { 32, 96 };

	Sprite* outside = Sprite::Create(hp_outsideTextureHandle_, uiPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f });
	Sprite* bar = Sprite::Create(hp_barTextureHandle_, uiPosition_ - Vector2(barSize_.x / 2.0f, barSize_.y / 2.0f), {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	Sprite* outsideStamina = Sprite::Create(stamina_outsideTextureHandle_, uiPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f });
	Sprite* barStamina = Sprite::Create(stamina_barTextureHandle_, uiPosition_ , { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f });

	hp_outside_ = std::unique_ptr<Sprite>(outside);
	hp_bar_ = std::unique_ptr<Sprite>(bar);

	stamina_outside_ = std::unique_ptr<Sprite>(outsideStamina);
	stamina_bar_ = std::unique_ptr<Sprite>(barStamina);

	hp_bar_->SetSize(barSize_);
	stamina_outside_->SetSize(staminaBarSize_);
	stamina_bar_->SetSize(staminaBarSize_);
	stamina_bar_->SetRotation(std::numbers::pi_v<float>);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Update(const float& playerHpRaito, const float& playerStamina, const Vector2& playerScreenPos) {
	EditImGui();

	float staminaRaitio = playerStamina / 90.0f;

	Vector2 playerHp = { barSize_.x * playerHpRaito, barSize_.y };
	Vector2 stamina = { staminaBarSize_.x , staminaBarSize_.y * (1.0f - staminaRaitio) };

	hp_bar_->SetTextureRect({ 0,0 }, playerHp);
	hp_bar_->SetSize(playerHp);

	stamina_bar_->SetTextureRect({ 0,stamina.y }, staminaBarSize_);
	//stamina_bar_->SetSize(stamina);
	
	Vector2 staminaUIPos = { playerScreenPos.x + 80.0f, playerScreenPos.y - 20.0f };
	hp_outside_->SetPosition(uiPosition_);
	stamina_outside_->SetPosition(staminaUIPos);
	stamina_bar_->SetPosition(staminaUIPos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Draw() const {
	hp_outside_->Draw();
	hp_bar_->Draw();
	stamina_bar_->Draw();
	stamina_outside_->Draw();
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
