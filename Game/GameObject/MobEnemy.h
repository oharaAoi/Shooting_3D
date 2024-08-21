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

enum class MobEnemyBehavior {
	kRoot,		// 通常攻撃
	kAttack,	// 攻撃中,
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
	void Move();

	/// <summary>
	/// 攻撃をまとめた関数
	/// </summary>
	void Attack() override;

	/// <summary>
	/// 弾を撃つ関数
	/// </summary>
	void Shot();

	/// <summary>
	///	状態を変更する
	/// </summary>
	/// <param name="behavior"></param>
	void ChangeBehavior(std::unique_ptr<BaseCharacterState> behavior);

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

	// ------------ Enemyの状態に関する変数 ------------ // 
	MobEnemyBehavior behavior_ = MobEnemyBehavior::kRoot;
	std::unique_ptr<BaseCharacterState> behaviorState_ = nullptr;
	std::optional<MobEnemyBehavior> behaviorRequest_ = std::nullopt;

};

