#pragma once
#include <list>
#include "Effect/Particle.h"
#include "Math/MyRandom.h"

class EmissionEffect {
public:

	EmissionEffect();
	~EmissionEffect();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	void AddParticleList(const Vector3& origine, const uint32_t& lifeTime, const uint32_t& count);

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

private:

	Model* particleModel_;

	std::list<Particle> particleList_;

};

