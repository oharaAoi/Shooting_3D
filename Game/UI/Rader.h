#pragma once
#include "Enviroment.h"
// STL
#include <vector>
#include <list>
// KamataEngine
#include <Sprite.h>
#include <TextureManager.h>
// Math
#include "Math/MyMath.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

class Rader {
public:

	struct SpriteData {
		std::unique_ptr<Sprite> sprite;
		uint32_t textureHandle = 0;
		Vector2 pos;
	};

public:

	Rader();
	~Rader();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="playerNearEnemyCount"></param>
	void Update(const uint32_t& playerNearEnemyCount, const uint32_t& playerAimCount);

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGuiの編集
	/// </summary>
	void EditImGui();

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// Playerの位置を求める
	/// </summary>
	void CalculationPlayerPos();

	/// <summary>
	/// 敵の位置を計算する
	/// </summary>
	void CalculationEnemiesPos();

	/// <summary>
	/// 敵の座標を追加する
	/// </summary>
	/// <param name="enemyPos"></param>
	void AddEnemiesPos(const Vector3& enemyPos) {
		enemiesPos_.push_back(enemyPos);
	}

	void ClearList();

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

	void SetPlayerPosition(const Vector3& pos) { (playerPosition_.x = pos.x, playerPosition_.y = pos.z); }

private:

	// playerの近くにいる敵の数(レーダーの範囲内の敵の数)
	uint32_t playerNearEnemyCount_ = 0;

	// 基盤
	SpriteData base_;
	// 基盤の光
	SpriteData baseLight_;
	Vector4 baseLightColor_;
	// player
	SpriteData player_;
	// Enemies
	std::list<SpriteData> enemies_;
	std::list<Vector3> enemiesPos_;
	uint32_t enemyTextureHandle_;

	// ゲーム上の位置
	Vector2 playerPosition_;

	Vector2 baseTextureSize_;

	Vector2 baseCenterPos_;
};

