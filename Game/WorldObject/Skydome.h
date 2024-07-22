#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include <cassert>

/// <summary>
/// 天球
/// </summary>
class Skydome {
public:

	Skydome(Model* model);
	~Skydome();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewpro);

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

};
