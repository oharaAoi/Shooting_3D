#pragma once
// STL
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
// KamataEngien
#include "WorldTransform.h"
// Enemy
#include "GameObject/MobEnemy.h"
#include "GameObject/BossEnemy.h"
#include "GameObject/MidMobEnemy.h"
// Loader
#include "Loader/ModelLoader.h"

class GameScene;

/// <summary>
/// Enemyを管理するクラス
/// </summary>
class EnemyManager {
public:

	EnemyManager();
	~EnemyManager();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw(const ViewProjection& viewProjection) const;

	/// <summary>
	/// 当たり判定の可視か
	/// </summary>
	void DrawCollisions();

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// 敵を出現させるファイルを読み込む関数
	/// </summary>
	void LoadFile();

	/// <summary>
	/// すべてのファイル名を取得する
	/// </summary>
	void AllLoadFilesName();

	/// <summary>
	/// Enemyの配置を保存する
	/// </summary>
	void SaveEnemyPos();

	/// <summary>
	/// 敵をImGui上で配置しファイルに保存する関数
	/// </summary>
	void EditEnemyPos();

	/// <summary>
	/// 敵を出現させる
	/// </summary>
	void EnemyPop(const uint32_t& loadFineNum);

	/// <summary>
	/// ImGuiを編集する
	/// </summary>
	void ImGuiEdit();

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	// ------------ Parent ------------ // 
	void SetParent(const WorldTransform* parent);

	// ------------ List ------------ // 
	std::list<std::unique_ptr<BaseEnemy>>& GetEnemysList() { return enemysList_; }

	// ------------ Position ------------ // 
	void SetPlayerPosition(const Vector3& playerPosition) { playerPosition_ = playerPosition; }

	// ------------ gameScene ------------ // 
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	// ------------ bossHp ------------ // 
	const float GetBossHp() const { return bossEnemy_->GetHpRaito(); }

	// ------------ boss ------------ // 
	BaseEnemy* GetBossEnemy() { return bossEnemy_; }

private:

	const WorldTransform* parentWorldTransform_;

	GameScene* gameScene_ = nullptr;

	uint32_t popTime_ = 0;
	const uint32_t popTimeLimit_ = 240;

	// ------------ ファイル読み込みに関する変数 ------------ // 
	// グローバル変数の保存先ファイルパス
	const std::string kDirectorPath_ = "./Resources/EnemyPos/";
	// 今どの配置を行っているか
	uint32_t nowWaveEnemyPos_ = 0;
	// 敵の配置が入った配列
	std::vector<std::string> filePathArray_;
	uint32_t currentIndex_ = 0;
	uint32_t fileNum_ = 0;

	// ------------ Enemyに関する変数 ------------ // 
	// Model
	std::vector<Model*> mobEnemyPartsModels_;
	std::vector<Model*> midEnemyPartsModels_;
	std::vector<Model*> bossEnemyPartsModels_;
	// enemyList
	std::list<std::unique_ptr<BaseEnemy>> enemysList_;

	// ------------ 敵の配置に関する変数 ------------ // 
	// 生成する敵の種類
	int createEnemyType_ = 0;
	// 生成する敵の配置
	Vector3 createEnemyPos_;
	// 生成する敵の速さ
	Vector3 createEnemyVelocity_;
	// 生成する敵の配列
	std::list<std::unique_ptr<BaseEnemy>> createEnemysList_;

	// ------------ playerに関する変数 ------------ // 
	Vector3 playerPosition_;

	// ------------ bossに関する変数 ------------ // 
	BaseEnemy* bossEnemy_ = nullptr;
};

