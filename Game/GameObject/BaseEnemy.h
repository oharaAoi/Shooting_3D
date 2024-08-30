#pragma once
#include "GameObject/BaseCharacter.h"
#include "State/BaseCharacterState.h"

enum EnemyType {
	Type_Mob = 0,
	Type_MidEnemy,
	Type_Boss
};

enum class EnemyBehavior {
	kRoot,		// 通常攻撃
	kAttack,	// 攻撃中,
};

class GameScene;

/// <summary>
/// Enemyの基本となるクラス
/// </summary>
class BaseEnemy :
    public BaseCharacter {

public:

    BaseEnemy() = default;
    ~BaseEnemy() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(std::vector<Model*> models) override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection) const override;

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision([[maybe_unused]] Collider* other) override {};

	/// <summary>
	/// ImGui
	/// </summary>
	virtual void EditImGui() {};

	/// <summary>
	/// 普段の挙動の関数
	/// </summary>
	virtual void Move() {};

	/// <summary>
	/// 攻撃に関する関数
	/// </summary>
	virtual void Attack() {};

	/// <summary>
	///	状態を変更する
	/// </summary>
	/// <param name="behavior"></param>
	void ChangeBehavior(std::unique_ptr<BaseCharacterState> behavior);

	/// <summary>
	/// 攻撃を食らった時の演出
	/// </summary>
	virtual void HitedEffect();

	void ImGuiSetTranslation();

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	/// <summary>
	/// world空間での座標を取得する
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition() const override;

	/// <summary>
	/// ScreenPositionを返す
	/// </summary>
	/// <returns></returns>
	Vector3 GetScreenPosition(const ViewProjection& viewProjection) const ;
	Vector3 GetScreenPosition(const Vector3& posWorld, const ViewProjection& viewProjection) const;

	// ------------ hp ------------ //
	float GetHpRaito() const { return static_cast<float>(hp_) / static_cast<float>(firstHp_); }

	// ------------ worldTransform ------------ // 
	void SetParent(const WorldTransform* parent);
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }
	void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }
	void SetTranslation(const Vector3& translation) { worldTransform_.translation_ = translation; }

	// ------------ enemyType ------------ // 
	const int GetEnemyType() const { return enemyType_; }

	// ------------ position ------------ // 
	void SetPlayerPosition(const Vector3& position) { playerPosition_ = position; }

	// ------------ gameScene ------------ // 
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	// ------------ isDead ------------ // 
	const bool GetIsDead() const { return isDead_; }
	void SetIsDead(const bool& isDead) { isDead_ = isDead; }

	// ------------ isDiscovery ------------ // 
	const bool GetIsDiscovery() const { return isDiscovery_; }
	void SetIsDiscovery(const bool& isDiscovery) { isDiscovery_ = isDiscovery; }

	// ------------ velocity ------------ // 
	const Vector3& GetVelocity() const { return velocity_; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	// ------------ 状態遷移の要求 ------------ // 
	void SetBehaviorRequest(const EnemyBehavior& request) { behaviorRequest_ = request; }

protected:

	GameScene* gameScene_ = nullptr;

	// ------------ キャラクターの情報に関する変数 ------------ // 
	uint32_t hp_ = 0;
	uint32_t firstHp_ = 0;
	int enemyType_;

	bool isDead_;
	bool isDiscovery_;
	bool isPreDiscovery_;

	Vector3 velocity_;

	// ------------ Bulletに必要な変数 ------------ // 
	Vector3 playerPosition_;

	// ------------ 状態 ------------ // 
	EnemyBehavior behavior_ = EnemyBehavior::kRoot;
	std::unique_ptr<BaseCharacterState> behaviorState_ = nullptr;
	std::optional<EnemyBehavior> behaviorRequest_ = std::nullopt;
};

