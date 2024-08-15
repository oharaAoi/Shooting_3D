#include "CollisionManager.h"

CollisionManager::CollisionManager() { Init(); }
CollisionManager::~CollisionManager() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void CollisionManager::Init() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void CollisionManager::Draw() const {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　当たり判定で使用する関数群
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- すべての当たり判定チェックする関数 ------------------- //

void CollisionManager::CheckAllCollision() {
	// リスト内のペアの総当たり判定
	std::list<Collider*>::iterator iterA = colliders_.begin();
	for (; iterA != colliders_.end(); ++iterA) {
		Collider* colliderA = *iterA;

		// イテレータBはイテレータAの次の要素から回す
		std::list<Collider*>::iterator iterB = iterA;
		iterB++;

		for (; iterB != colliders_.end(); ++iterB) {
			Collider* colliderB = *iterB;
			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

// ------------------- コライダー2つの衝突判定と応答 ------------------- //

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();
	// 差分ベクトル
	Vector3 subtract = posB - posA;
	// 座標AとBの距離を求める
	float distance = Length(subtract);
	// 球と球の交差判定
	if (colliderA->GetRadius() + colliderB->GetRadius() > distance) {
		// それぞれの衝突時コールバック関数を呼び出す
		colliderA->OnCollision(colliderB);
		colliderB->OnCollision(colliderA);
	}
}
