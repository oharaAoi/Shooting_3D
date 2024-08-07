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

class BaseCharacter {
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
	virtual void Draw(const ViewProjection& viewProjection);

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	/// <summary>
	/// ワールド変換データ
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }


	//virtual Vector3 GetCenterPosition() const override;

protected:
	// モデルデータ配列
	std::vector<Model*> models_;
	// パーツごとのワールド変換データ
	std::vector<WorldTransform> worldTransforms_;
	// 元となるワールド変換データ
	WorldTransform worldTransform_;
	// キーとなる配列
	std::vector<std::string> keys_;
};

