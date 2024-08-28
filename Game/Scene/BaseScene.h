#pragma once

class BaseScene {
public:

	BaseScene() = default;
	~BaseScene() = default;

	virtual void Initialize() {};

	virtual void Update() {};

	virtual void Draw() {};

	bool GetIsFinish() const { return isFinish_; }

protected:
	// sceneを終了するか
	bool isFinish_ = false;
};

