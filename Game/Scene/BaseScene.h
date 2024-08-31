#pragma once

enum class Scene {
	kTitle,
	kGame,
	kClear
};

class BaseScene {
public:

	BaseScene() = default;
	virtual ~BaseScene() = default;

	virtual void Initialize() {};

	virtual void Update() {};

	virtual void Draw() {};

	bool GetIsFinish() const { return isFinish_; }

	Scene GetNextScene() const { return nextScene_; }

protected:
	// sceneを終了するか
	bool isFinish_ = false;

	Scene nextScene_;
};

