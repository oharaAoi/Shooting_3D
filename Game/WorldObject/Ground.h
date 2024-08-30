#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include <cassert>

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

class Ground {
public:

	Ground(Model* model);
	~Ground();

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

