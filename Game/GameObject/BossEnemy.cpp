#include "BossEnemy.h"
#include "GameScene.h"

BossEnemy::BossEnemy(std::vector<Model*> models, const std::vector<Vector3>& controlPoint, const float& division) {
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kBoss));
	appearanceControlPoint_ = controlPoint;
	worldTransform_.translation_ = appearanceControlPoint_[2];
	division_ = division;
	Init(models);
}

BossEnemy::~BossEnemy() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEnemy::Init(std::vector<Model*> models) {
	BaseEnemy::Init(models);
	enemyType_ = EnemyType::Type_Mob;
	worldTransforms_[BossParts::Boss_Body].parent_ = &worldTransform_;

	worldTransform_.scale_ = { 6.0f, 6.0f, 6.0f };
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.rotation_ = { 0.0f,3.1f,0.0f };

	behaviorRequest_ = EnemyBehavior::kRoot;
	ChangeBehavior(std::make_unique<BossRootState>(this));

	hp_ = 500;
	radius_ = 2.0f;
	isDead_ = false;

	attackType_ = BossAttackType::Normal_Attack;

	floating_.parameter = 0;
	floating_.period = 90;
	floating_.amplitude = 0.2f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEnemy::Update() {
	// 状態の変更のリクエストがあるかを確認する
	CheckBehaviorRequest();
	// 現在の状態を更新する
	behaviorState_->Update();

	BaseEnemy::Update();
	// ImGuiの編集
	EditImGui();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEnemy::Draw(const ViewProjection& viewProjection) const {
	BaseEnemy::Draw(viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 攻撃を行う関数 ------------------- //

void BossEnemy::Move() {
	FloatingGimmick();
}

void BossEnemy::Attack() {
	switch (attackType_) {
	case Normal_Attack:
		ShotBullet();
		break;
	case Homing_Attack:
		break;
	case TripleHoming_Attack:
		break;
	}
}

void BossEnemy::ShotBullet() {
	gameScene_->AddBossBullet(
		std::move(std::make_unique<BossBullet>(
			bulletModel_, worldTransform_.translation_, playerPosition_, worldTransform_.rotation_, worldTransform_.parent_, BossAttackType::Normal_Attack
	)));
}

// ------------------- 状態遷移のリクエストがあるかを確認する ------------------- //

void BossEnemy::CheckBehaviorRequest() {
	// リクエストがあったら
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case EnemyBehavior::kRoot:
			ChangeBehavior(std::make_unique<BossRootState>(this));
			break;

		case EnemyBehavior::kAttack:
			ChangeBehavior(std::make_unique<BossAttackState>(this));
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

// ------------------- 衝突時に呼ばれる関数 ------------------- //

void BossEnemy::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {
		hp_--;
	}
}

// ------------------- 浮遊する ------------------- //

void BossEnemy::FloatingGimmick() {
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
	Vector3 translate = worldTransform_.translation_;
	translate.y = std::sin(floating_.parameter) * floating_.amplitude;
	worldTransform_.translation_ = translate;
}

// ------------------- ImGuiを編集する関数 ------------------- //

void BossEnemy::EditImGui() {
#ifdef _DEBUG
	ImGui::Begin("Boss");
	ImGui::DragScalar("hp", ImGuiDataType_U32, &hp_);
	ImGui::Separator();
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 BossEnemy::GetWorldPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}
