#include "AudioManager.h"

AudioManager::~AudioManager() {
}

AudioManager* AudioManager::GetInstacne() {
	static AudioManager instance;
	return &instance;
}

void AudioManager::Finalize() {
}

void AudioManager::Init() {
	audio_ = Audio::GetInstance();

	Load("Audio/hited.wav");
	Load("Audio/playerShot.wav");
	Load("Audio/playerDash.wav");

	// System
	Load("Audio/pushButton.wav");
	Load("Audio/lockOn.wav");
	Load("Audio/lockOnCancel.wav");
	Load("Audio/gameOver.wav");
	Load("Audio/gameClear.wav");

	// BGM
	Load("Audio/title.wav");
	Load("Audio/game.wav");
}

void AudioManager::Update() {
	for (std::list<AudioData>::iterator it = playList_.begin(); it != playList_.end();) {
		if (!(audio_->IsPlaying(it->handle))) {
			it = playList_.erase(it);
		} else {
			it++;
		}
	}
}

void AudioManager::IsPlay() {
	for (std::list<AudioData>::iterator it = playList_.begin(); it != playList_.end(); it++) {
		
			if (!it->isPlay) {
				it->handle = audio_->PlayWave(it->handle, it->isLoop, it->volume);
				it->isPlay = true;
			}
		
	}
}

void AudioManager::Load(const std::string& soundPath) {
	uint32_t audioData = audio_->LoadWave(soundPath);
	audioMap_.emplace(soundPath, audioData);
}

void AudioManager::AddPlayList(const std::string& soundPath, const bool& isLoop, const float& volume) {
	AudioData data;
	data.handle = audioMap_[soundPath];
	data.isLoop = isLoop;
	data.volume = volume;
	data.audioName = soundPath;
	data.isPlay = false;
	
	playList_.push_back(std::move(data));
}

void AudioManager::StopAudioPlayerList(const std::string& soundPath) {
	for (std::list<AudioData>::iterator it = playList_.begin(); it != playList_.end(); it++) {
		if (it->audioName == soundPath) {
			audio_->StopWave(it->handle);
		}
	}
}
