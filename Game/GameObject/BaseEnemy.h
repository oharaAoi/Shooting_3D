#pragma once
#include "GameObject/BaseCharacter.h"

enum EnemyType {
	Type_Mob = 0,
	Type_MidBoss,
	Type_Boss
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
	/// 攻撃に関する関数
	/// </summary>
	virtual void Attack() {};

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
	Vector3 GetScreenPosition(const ViewProjection& viewProjection);

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

	// ------------ gameSceneをセットする ------------ // 
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

protected:

	GameScene* gameScene_ = nullptr;

	// ------------ キャラクターの情報に関する変数 ------------ // 
	uint32_t hp_ = 0;

	int enemyType_;

	// ------------ Bulletに必要な変数 ------------ // 

	Vector3 playerPosition_;
};

