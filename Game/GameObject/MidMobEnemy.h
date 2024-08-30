#pragma once
#include "GameObject/BaseEnemy.h"
// State
#include "State/MidEnemyRootState.h"
#include "State/MidEnemyAttackState.h"

enum MidEnemyParts {
	MidMob_Body = 0,
	MidMob_LEye,
	MidMob_REye,
};

class GameScene;

class MidMobEnemy :
	public BaseEnemy {
public:

	struct Animation {
		float t;
		float amplitudeX;
		float amplitudeY;
	};

public:

	MidMobEnemy(std::vector<Model*> models, const Vector3& pos, const Vector3& velocity);
	~MidMobEnemy();

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
	/// 普段の動きを行う関数
	/// </summary>
	void AnimationGimmick();

	/// <summary>
	/// Enmeyを動かす
	/// </summary>
	void Move() override;

	/// <summary>
	/// 攻撃をまとめた関数
	/// </summary>
	void Attack() override;

	/// <summary>
	/// 弾をリストに追加する関数
	/// </summary>
	void Shot();

	/// <summary>
	/// 進行方向に回転をする
	/// </summary>
	void TurnAoundVelocity();

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

	// ------------ world座標 ------------ // 
	Vector3 GetWorldPosition() const override;

private:

	// ------------ ポインタ(所有権がない物) ------------ // 
	Model* bulletModel_ = nullptr;

	// ------------  ------------ // 
	uint32_t enemyId_;

	// ------------ 挙動の変数 ------------ // 
	Animation animation_;

};

