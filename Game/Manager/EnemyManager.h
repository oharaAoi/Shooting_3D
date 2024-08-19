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
// Loader
#include "Loader/ModelLoader.h"

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

private:

	const WorldTransform* parentWorldTransform_;

	// ------------ ファイル読み込みに関する変数 ------------ // 
	// グローバル変数の保存先ファイルパス
	const std::string kDirectorPath_ = "Resources/EnemyPos/";
	// 今どの配置を行っているか
	uint32_t nowWaveEnemyPos_ = 0;
	// 敵の配置が入った配列
	std::vector<std::string> filePathArray_;
	uint32_t currentIndex_ = 0;

	// ------------ Enemyに関する変数 ------------ // 
	// Model
	std::vector<Model*> mobEnemyPartsModels_;
	// enemyList
	std::list<std::unique_ptr<BaseEnemy>> enemysList_;
};

