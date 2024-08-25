#pragma once
// GameObject
#include "GameObject/BaseEnemy.h"
// Adjustment
#include "Adjustment/AdjustmentItem.h"
// State
#include "State/BossRootState.h"
#include "State/BossAttackState.h"

enum BossParts {
	Boss_Body = 0,
};

class BossEnemy :
	public BaseEnemy {

public:

	BossEnemy(std::vector<Model*> models, const Vector3& pos);
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
	/// 攻撃をまとめた関数
	/// </summary>
	void Attack() override;

	/// <summary>
	/// 状態を変化させるリクエストがあるかを確認する
	/// </summary>
	void CheckBehaviorRequest();

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// ImGuiの編集
	/// </summary>
	void EditImGui() override;

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	/// <summary>
	/// world空間での座標を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const override;

private:

	// ------------ ポインタ(所有権がない物) ------------ // 
	Model* bulletModel_ = nullptr;

	// ------------  ------------ // 
	uint32_t enemyId_;
};

