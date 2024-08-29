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
}

void AudioManager::Update() {
	for (std::list<AudioData>::iterator it = playList_.begin(); it != playList_.end();) {
		if (!audio_->IsPlaying(it->handle)) {
			it = playList_.erase(it);
		} else {
			it++;
		}
	}
}

void AudioManager::IsPlay() {
	for (std::list<AudioData>::iterator it = playList_.begin(); it != playList_.end(); it++) {
		if (!audio_->IsPlaying(it->handle)) {
			audio_->PlayWave(it->handle, it->isLoop, it->volume);
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
	
	playList_.push_back(data);
}
