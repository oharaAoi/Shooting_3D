#pragma once
#include "Vector3.h"
#include "stdint.h"
#include "Collision/CollisionTypeIdDef.h"

const uint32_t kCollisionAttributePlayer = 0b1;
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:

	Collider() {};
	~Collider() {};

public:

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision([[maybe_unused]] Collider* other) {};

	/// <summary>
	/// World上の位置を取得する関数
	/// </summary>
	/// <returns>worldPosition</returns>
	virtual Vector3 GetWorldPosition() const { return Vector3(); };

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	// ------------ 半径 ------------ // 
	float GetRadius() const { return radius_; }
	void SetRadius(const float& radius) { radius_ = radius; }

	// ------------ 衝突属性(自分) ------------ // 
	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }
	void SetCollisionAttribute(const uint32_t& attribute) { collisionAttribute_ = attribute; }

	// ------------ 衝突マスク(相手) ------------ // 
	uint32_t GetCollisionMask() const { return collisionMask_; }
	void SetCollisionMask(const uint32_t& mask) { collisionMask_ = mask; }

	// ------------ 種別IDの取得/設定 ------------ // 
	uint32_t GetTypeID() const { return typeID_; }
	void SetTypeID(const uint32_t& typeID) { typeID_ = typeID; }

protected:

	// 半径
	float radius_ = 1.5f;
	// 衝突属性
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク
	uint32_t collisionMask_ = 0xffffffff;
	// 種別ID
	uint32_t typeID_ = 0u;

};

