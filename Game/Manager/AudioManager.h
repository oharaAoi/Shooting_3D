#pragma once
#include <map>
#include "Audio.h"

class AudioManager {
public:

	struct AudioData {
		uint32_t handle;
		bool isLoop;
		float volume;
	};

public:

	AudioManager() = default;
	~AudioManager();
	AudioManager(const AudioManager&) = delete;
	const AudioManager& operator=(const AudioManager&) = delete;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static AudioManager* GetInstacne();

	void Finalize();

	void Init();

	void Update();

	/// <summary>
	/// 音を鳴らす関数
	/// </summary>
	void IsPlay();

	/// <summary>
	/// ObjectFileを読み込む
	/// </summary>
	/// <param name="modelPath">objのパス</param>
	void Load(const std::string& soundPath);

	void AddPlayList(const std::string& soundPath, const bool& isLoop, const float& volume);

private:

	Audio* audio_ = nullptr;

	// データを格納した物
	std::map<std::string, uint32_t> audioMap_;
	// 鳴らすリスト
	std::list<AudioData> playList_;

};

