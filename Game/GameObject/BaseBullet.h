#pragma once
#include <cassert>
#include <map>
#include <string>
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vector3.h"
#include "Math/MyMath.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

class BaseBullet {
public:

	BaseBullet() = default;
	~BaseBullet() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init(Model* model);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection);

protected:

	// 3Dモデルのポインタ
	Model* model_;

	// worldTrasform
	WorldTransform worldTransform_;

};

