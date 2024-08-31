#pragma once
#include <map>
#include <memory>
#include "Audio.h"

class AudioManager {
public:

	struct AudioData {
		std::string audioName;
		uint32_t handle;
		bool isLoop;
		float volume;
		bool isPlay;
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

	/// <summary>
	/// 鳴らすリストに指定のwavを追加する
	/// </summary>
	/// <param name="soundPath"></param>
	/// <param name="isLoop"></param>
	/// <param name="volume"></param>
	void AddPlayList(const std::string& soundPath, const bool& isLoop, const float& volume);

	/// <summary>
	/// playListにある音声を止める
	/// </summary>
	/// <param name="soundPath"></param>
	void StopAudioPlayerList(const std::string& soundPath);

	/// <summary>
	/// playListを削除する
	/// </summary>
	void ClearPlayerList() { playList_.clear(); }

private:

	Audio* audio_ = nullptr;

	// データを格納した物
	std::map<std::string, uint32_t> audioMap_;
	// 鳴らすリスト
	std::list<AudioData> playList_;
};

