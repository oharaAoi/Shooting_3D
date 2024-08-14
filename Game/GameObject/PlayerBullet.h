#pragma once
#include "GameObject/BaseBullet.h"

class PlayerBullet :
	public BaseBullet {
public:

	PlayerBullet(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation);
	~PlayerBullet();

	void Init(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) const override;

private:

	

};

