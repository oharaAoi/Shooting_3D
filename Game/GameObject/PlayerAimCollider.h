#pragma once
#include "Collision/Collider.h"
class PlayerAimCollider :
    public Collider {

public:

	PlayerAimCollider(const Vector3& size);
	~PlayerAimCollider();

	void Init(const Vector3& size);

	void Update(const Vector3& center, const Vector3& rotation);

	void Draw();

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override {};

private:



};

