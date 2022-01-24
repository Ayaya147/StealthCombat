#pragma once
#include <vector>
#include "BaseScene.h"

class GameScene : public BaseScene
{
public:
	GameScene(class SceneManager* sm, const class Parameter& parameter);
	~GameScene();

	void ProcessInput() override;
	void Update() override;
	void GenerateOutput() override;

	class CloudActor* GetCloud() const { return mCloud; }

private:
	class CloudActor* mCloud;
};
