#pragma once
#include "GameObject/BaseBullet.h"
class PlayerBullet :
	public BaseBullet {
public:

	PlayerBullet(Model* model);
	~PlayerBullet();

	void Init(Model* model) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

private:

	// ------------ 移動に関する変数 ------------ // 
	// 方向
	Vector3 direction_;
	// 速度
	Vector3 velocity_;

};

