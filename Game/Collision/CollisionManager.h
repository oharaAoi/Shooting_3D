#pragma once
// STL
#include <list>
// KamataEngien
#include "Model.h"
#include "ViewProjection.h"
// collision
#include "Collision/Collider.h"
// AdjustmentItem
#include "Adjustment/AdjustmentItem.h"

/// <summary>
/// 当たり判定を取るクラス
/// </summary>
class CollisionManager {
public:

	CollisionManager();
	~CollisionManager();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// Colliderの描画
	/// </summary>
	void Draw() const;

	/// <summary>
	/// すべての当たり判定チェック
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーAQ</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 敵がプレイヤーの前にいるか
	/// </summary>
	/// <param name="colliderA">: player</param>
	/// <param name="colliderB">: enemies</param>
	/// <returns></returns>
	uint32_t CountEnemiesPlayerRange(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// Enemyが敵を発見したか
	/// </summary>
	/// <returns></returns>
	bool IsDiscoveryEnemy(Collider* colliderA, Collider* colliderB);

	void IsOBBCollision(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider"></param>
	void AddCollider(Collider* collider) { colliders_.push_back(collider); };

	// リストを空にする
	void Rest() { colliders_.clear(); }

	/// <summary>
	/// OBBとOBBで当たり判定を取る
	/// </summary>
	/// <param name="obb1"></param>
	/// <param name="obb2"></param>
	/// <returns></returns>
	bool IsCollision(const OBB& obb1, const OBB& obb2);

private:

	// コライダーのリスト
	std::list<Collider*> colliders_;
	// 可視コライダーのモデル
	std::unique_ptr<Model> model_;

	bool isDraw_ = false;

};

