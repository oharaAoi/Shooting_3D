#include "MobRootState.h"
#include "GameObject/MobEnemy.h"

MobRootState::MobRootState(MobEnemy* baseEnemy)
	: BaseEnemyState("State_Root", baseEnemy) {
		Init();
}

MobRootState::~MobRootState() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MobRootState::Init() {
	work_.moveTime = 0;
	work_.attackTime = 120;

	floating_.parameter = 0;
	floating_.period = 90;
	floating_.amplitude = 0.2f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MobRootState::Update() {
	FloatingGimmick();
	// フレームをカウントし、一定時間過ぎたら攻撃フェーズに移動する
	work_.moveTime++;
	if (work_.moveTime > work_.attackTime) {
		enemy_->SetBehaviorRequest(EnemyBehavior::kAttack);
		work_.moveTime = 0;
	}
}

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

void MobRootState::FloatingGimmick() {
	//----------------------------------------------------------
	// bodyのアニメーション
	//----------------------------------------------------------
	// 1フレームでのパラメータ加算値
	const float step = (2.0f * std::numbers::pi_v<float>) / static_cast<float>(floating_.period);
	// パラメータを1ステップ分加算
	floating_.parameter += step;
	// 2πを超えたら0に戻す
	floating_.parameter = std::fmod(floating_.parameter, 2.0f * std::numbers::pi_v<float>);
	// 座標に反映
	Vector3 translate = enemy_->GetWorldTransform().translation_;
	translate.y = std::sin(floating_.parameter) * floating_.amplitude;
	enemy_->SetTranslation(translate);
}

