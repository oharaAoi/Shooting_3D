#pragma once
#include "GameObject/BaseBullet.h"
#include "GameObject/Player.h"

class BossEnemy;

enum class BulletType {
	Normal_Bullet,
	Homing_Bullet,
	TripleHoming_Bullet
};

class BossBullet :
    public BaseBullet {
public:

    BossBullet(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent, const BulletType& attackType);
    ~BossBullet();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="model">: modelのポインタ</param>
	/// <param name="pos">: 座標</param>
	/// <param name="velocity">: 速度</param>
	/// <param name="rotation">: 回転</param>
	/// <param name="parent">: 親のworldTransformのポインタ</param>
	void Init(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection) const override;

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// 拡大を行う関数
	/// </summary>
	void ScaleUp();

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	// ------------ ポインタ ------------ // 
	void SetPlayer(Player* player) { player_ = player; }

	// ------------ world座標 ------------ // 
	Vector3 GetWorldPosition() const override;

	// ------------ isFire ------------ // 
	void SetIsFire(const bool& isFire) { isFire_ = isFire; }

	// ------------ waitTime ------------ // 
	void SetWaitTime(const uint32_t& waitTime) { waitTime_ = waitTime; }

private:

	// ------------ ポインタ(所有権がない物) ------------ // 
	Player* player_ = nullptr;

	// ------------ 弾の状態に関する変数 ------------ // 
	const float kBulletSpeed_ = 0.6f;
	BulletType  attackType_;

	// ------------ Homigに関わる変数 ------------ // 
	uint32_t homigAttackTime_;
	bool isFire_;
	uint32_t waitTime_;

	// ------------ 拡大に関する変数 ------------ // 
	uint32_t scaleUpFrameCount_;
	uint32_t scaleUpFrame_ = 60;
	float scaleUpT_;

	// ------------ 速度に関わる変数 ------------ // 
	Vector3 firstPlayerPos_;

};

