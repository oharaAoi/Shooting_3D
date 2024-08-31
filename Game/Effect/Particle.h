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
#include "Math/MyRandom.h"
#include "Loader/ModelLoader.h"

static float kSpeed_ = 0.1f;

class Particle {
public:

	Particle(Particle&&) = default;  // ムーブコンストラクタをデフォルトで使用
	Particle& operator=(Particle&&) = default;  // ムーブ代入演算子をデフォルトで使用

	// コピー操作を禁止
	Particle(const Particle&) = delete;
	Particle& operator=(const Particle&) = delete;

	Particle(Model* model, const Vector3& position, const Vector3& velocity, const uint32_t& lifeTime);
	~Particle();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(Model* model, const Vector3& position, const Vector3& velocity, const uint32_t& lifeTime);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	const bool GetIsDead() const { return isDead_; }

private:

	Model* model_;
	WorldTransform worldTransform_;
	ObjectColor color_;
	uint32_t lifeTime_;
	Vector3 velocity_;
	bool isDead_;
};

