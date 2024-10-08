#pragma once
// GameObject
#include "GameObject/BaseEnemy.h"
#include "GameObject/BossBullet.h"
// Adjustment
#include "Adjustment/AdjustmentItem.h"
// State
#include "State/BossRootState.h"
#include "State/BossAttackState.h"
// Math
#include "Math/MyRandom.h"

enum BossParts {
	Boss_Body = 0,
	Boss_LEye,
	Boss_REye
};

enum BossAttackType {
	Normal_Attack,
	Homing_Attack,
	TripleHoming_Attack,
	Rush_Attack
};

class GameScene;

class BossEnemy :
	public BaseEnemy {

public:

	struct Floating {
		float parameter;
		float amplitude;
		uint32_t period;
	};

public:

	BossEnemy(std::vector<Model*> models, const Vector3& pos, const Vector3& velocity);
	~BossEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::vector<Model*> models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) const override;

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// Enmeyを動かす
	/// </summary>
	void Move() override;

	/// <summary>
	/// 攻撃をまとめた関数
	/// </summary>
	void Attack() override;

	/// <summary>
	/// 弾をリストに追加する処理
	/// </summary>
	void NormalShot();

	/// <summary>
	/// 追従する弾を発射
	/// </summary>
	void HomingShot();

	/// <summary>
	/// 突進攻撃を行う
	/// </summary>
	void RushAttack();

	/// <summary>
	/// 3つの追従する弾
	/// </summary>
	void TripleHomingShot();

	/// <summary>
	/// 状態を変化させるリクエストがあるかを確認する
	/// </summary>
	void CheckBehaviorRequest();

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// 攻撃を食らった時の演出
	/// </summary>
	void HitedEffect() override;

	/// <summary>
	/// 浮遊の更新
	/// </summary>
	void FloatingGimmick();

	/// <summary>
	/// Playerの方向を向く関数
	/// </summary>
	void TurnAroundPlayer();

	/// <summary>
	/// 攻撃の種類を選ぶ
	/// </summary>
	void ChangeAttackType();

	/// <summary>
	/// ImGuiの編集
	/// </summary>
	void EditImGui() override;

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	// ------------ world空間での座標を取得する ------------ // 
	Vector3 GetWorldPosition() const override;

private:

	// ------------ ポインタ(所有権がない物) ------------ // 
	Model* bulletModel_ = nullptr;

	// ------------  ------------ // 
	uint32_t enemyId_;

	// ------------ 挙動に関する変数 ------------ // 
	Floating floating_;
	float forwardDot_ = 0;

	// ------------ 攻撃に関する変数 ------------ // 
	// なんの攻撃を行うか
	BossAttackType attackType_;

	uint32_t rushChargeTimeCount_;
	uint32_t rushChargeTime_;
	uint32_t rushTime_;
	uint32_t stiffnessTime_;

	bool isRushAttack_;
	bool isRush_;
};

