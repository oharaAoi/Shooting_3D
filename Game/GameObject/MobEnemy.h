#pragma once
#include "GameObject/BaseEnemy.h"
// State
#include "State/MobRootState.h"
#include "State/MobAttackState.h"
// GameObject
#include "GameObject/EnemyBullet.h"
// Loader
#include "Loader/ModelLoader.h"

static uint32_t mobEnemyNum = 0;

enum EnmeyParts {
	Enmey_Body = 0,
};

class GameScene;

class MobEnemy :
    public BaseEnemy {
public:

    MobEnemy(std::vector<Model*> models, const Vector3& pos);
    ~MobEnemy();

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
	/// 弾を撃つ関数
	/// </summary>
	void Shot();

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

