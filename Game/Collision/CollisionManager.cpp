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

bool CollisionManager::IsCollision(const OBB& obb1, const OBB& obb2) {
	// 分離軸の組み合わせを取得
	std::vector<Vector3> crossSeparatingAxises;
	for (uint8_t obb1Index = 0; obb1Index < 3; obb1Index++) {
		for (uint8_t obb2Index = 0; obb2Index < 3; obb2Index++) {
			Vector3 crossAxis = Cross(obb1.orientations[obb1Index], obb2.orientations[obb2Index]);
			if (Length(crossAxis) > 1e-6) { // 長さが非常に小さい場合を除外
				Normalize(crossAxis);
				crossSeparatingAxises.push_back(crossAxis);
			}
		}
	}

	// 面法線をまとめる
	std::vector<Vector3> obb1SurfaceNormals;
	for (uint8_t obbIndex = 0; obbIndex < 3; obbIndex++) {
		obb1SurfaceNormals.push_back(Normalize(TransformNormal(obb1.orientations[obbIndex], obb1.matRotate)));
	}

	std::vector<Vector3> obb2SurfaceNormals;
	for (uint8_t obbIndex = 0; obbIndex < 3; obbIndex++) {
		obb2SurfaceNormals.push_back(Normalize(TransformNormal(obb2.orientations[obbIndex], obb2.matRotate)));
	}

	// ------------------------------------------------------------
	// 分離軸を割り出す
	std::vector<Vector3> separatingAxises;
	separatingAxises.insert(separatingAxises.end(), obb1SurfaceNormals.begin(), obb1SurfaceNormals.end());
	separatingAxises.insert(separatingAxises.end(), obb2SurfaceNormals.begin(), obb2SurfaceNormals.end());
	separatingAxises.insert(separatingAxises.end(), crossSeparatingAxises.begin(), crossSeparatingAxises.end());

	// obbから頂点を取り出す
	std::vector<Vector3> obb1Indecies = obb1.MakeIndex();
	std::vector<Vector3> obb2Indecies = obb2.MakeIndex();

	// 頂点を分離軸候補に射影したベクトルを格納する

	// 取り出した頂点を分離軸へ射影する
	for (uint8_t axis = 0; axis < separatingAxises.size(); axis++) {
		std::vector<float> obb1ProjectIndecies;
		std::vector<float> obb2ProjectIndecies;

		for (uint8_t oi = 0; oi < obb1Indecies.size(); oi++) {
			// 各obbの頂点を射影する
			// 正射影ベクトルの長さを求める
			obb1ProjectIndecies.push_back(Dot(obb1Indecies[oi], separatingAxises[axis]));
			obb2ProjectIndecies.push_back(Dot(obb2Indecies[oi], separatingAxises[axis]));
		}

		// 最大値/最小値を取り出す
		float maxObb1 = *std::max_element(obb1ProjectIndecies.begin(), obb1ProjectIndecies.end());
		float maxObb2 = *std::max_element(obb2ProjectIndecies.begin(), obb2ProjectIndecies.end());
		float minObb1 = *std::min_element(obb1ProjectIndecies.begin(), obb1ProjectIndecies.end());
		float minObb2 = *std::min_element(obb2ProjectIndecies.begin(), obb2ProjectIndecies.end());

		// 影の長さを得る
		float projectLenght1 = float(maxObb1 - minObb1);
		float projectLenght2 = float(maxObb2 - minObb2);

		float sumSpan = projectLenght1 + projectLenght2;
		float longSpan = ((std::max)(maxObb1, maxObb2) - ((std::min)(minObb1, minObb2)));

		// 影の長さの合計 < 2つの影の両端の差分だったら分離軸が存在しているためfalse
		if (sumSpan <= longSpan) {
			return false;
		}
	}

	return true;
}
