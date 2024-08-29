#pragma once
#include <cassert>
#include <map>
#include <string>
#include "Model.h"
#include "Input.h"
#include "Audio.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vector3.h"
#include "ObjectColor.h"
#include "Math/MyMath.h"
// collision
#include "Collision/Collider.h"
#include "Collision/CollisionTypeIdDef.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

class BaseCharacter 
: public Collider {
public:

	BaseCharacter() = default;
	~BaseCharacter() = default;

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init(std::vector<Model*> models);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection) const;

	void DrawCollision();

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override {};

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	/// <summary>
	/// ワールド変換データ
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// World上の位置を取得する関数
	/// </summary>
	/// <returns>worldPosition</returns>
	virtual Vector3 GetWorldPosition() const override;

protected:
	// モデルデータ配列
	std::vector<Model*> models_;
	// パーツごとのワールド変換データ
	std::vector<WorldTransform> worldTransforms_;
	// 元となるワールド変換データ
	WorldTransform worldTransform_;
	// キーとなる配列
	std::vector<std::string> keys_;
	// 色
	ObjectColor color_;
};

