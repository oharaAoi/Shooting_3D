#include "BossBullet.h"
#include "GameObject/BossEnemy.h"

BossBullet::BossBullet(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent, const BulletType& attackType) {
	attackType_ = attackType;
	Init(model, pos, velocity, rotation, parent);
}

BossBullet::~BossBullet() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossBullet::Init(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent) {
	BaseBullet::Init(model, pos, velocity, rotation, parent);

	// velocityにplayerの座標が入っているため,playerの方向へのVelocityに変更する
	Vector3 direction = velocity_ - worldTransform_.translation_;
	velocity_ = Normalize(direction) * kBulletSpeed_;

	homigAttackTime_ = 30;
	isFire_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossBullet::Update() {
	// ---------------------------------------------
	// ↓ 発射前処理
	// ---------------------------------------------
	if (!isFire_) {
		ScaleUp();
		velocity_ = { 0,0,0 };
		BaseBullet::Update();
		return;
	}

	// ---------------------------------------------
	// ↓ 攻撃の種類によってVelocityを更新する
	// ---------------------------------------------
	switch (attackType_) {
	case BulletType::Normal_Bullet:
		break;
	case BulletType::Homing_Bullet:
		if (--homigAttackTime_ > 0) {
			Vector3 direction = player_->GetTranslation() - worldTransform_.translation_;
			velocity_ = Normalize(direction) * kBulletSpeed_;
		} else {
			attackType_ = BulletType::Normal_Bullet;
		}
		break;
	case BulletType::TripleHoming_Bullet:
		if (--waitTime_ <= 0) {
			attackType_ = BulletType::Homing_Bullet;
		}
		break;
	}

	// ---------------------------------------------
	// ↓ worldTransformの更新
	// ---------------------------------------------
	BaseBullet::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossBullet::Draw(const ViewProjection& viewProjection) const {
	BaseBullet::Draw(viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 衝突判定を行う ------------------- //

void BossBullet::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
		isDead_ = true;
	}
}

// ------------------- 拡大を行う関数 ------------------- //

void BossBullet::ScaleUp() {
	scaleUpFrameCount_++;
	scaleUpT_ = static_cast<float>(scaleUpFrameCount_) / static_cast<float>(scaleUpFrame_);
	worldTransform_.scale_ = Lerp({ 0,0,0 }, { 1,1,1 }, scaleUpT_);

	if (scaleUpFrameCount_ >= scaleUpFrame_) {
		isFire_ = true;
	}
}

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

// ------------------- world空間上での位置を取得する ------------------- //

Vector3 BossBullet::GetWorldPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}
