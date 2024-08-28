#include "BossUI.h"

BossUI::BossUI() {
	Init();
}

BossUI::~BossUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::Init() {
	hp_outsideTextureHandle_ = TextureManager::Load("./Resources/UI/Enemy/boss_hp_outside.png");
	hp_barTextureHandle_ = TextureManager::Load("./Resources/UI/Enemy/boss_hp_bar.png");

	uiPosition_ = { 650, 80 };
	barSize_ = { 516, 64 };

	Sprite* outside = Sprite::Create(hp_outsideTextureHandle_, uiPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f });
	Sprite* bar = Sprite::Create(hp_barTextureHandle_, uiPosition_ - Vector2(barSize_.x / 2.0f, barSize_.y / 2.0f), { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f });

	hp_outside_ = std::unique_ptr<Sprite>(outside);
	hp_bar_ = std::unique_ptr<Sprite>(bar);
	hp_bar_->SetSize(barSize_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::Update(const float& bossHpRaito) {
	EditImGui();

	Vector2 bossHpBarPos = { barSize_.x * bossHpRaito, barSize_.y };

	hp_bar_->SetTextureRect({ 0,0 }, bossHpBarPos);
	hp_bar_->SetSize(bossHpBarPos);

	hp_outside_->SetPosition(uiPosition_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossUI::Draw() const {
	hp_outside_->Draw();
	hp_bar_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::EditImGui() {
#ifdef _DEBUG
	ImGui::Begin("UI");
	ImGui::DragFloat2("BossUI_pos", &uiPosition_.x);
	ImGui::DragFloat2("BossUI_barSize", &barSize_.x);
	ImGui::End();
#endif // _DEBUG
}
