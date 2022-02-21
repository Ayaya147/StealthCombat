#pragma once
#include "BaseScene.h"
#include <vector>

class DemoScene : public BaseScene
{
public:
	DemoScene(class SceneManager* sm, const class Parameter& parameter);
	~DemoScene();

	void ProcessInput() override;
	void Update() override;
	void GenerateOutput() override;

	class PlaneActor* GetPlane() const { return mPlane; }
	class CloudActor* GetCloud() const { return mCloud; }
	class ExplosionActor* GetExplosion() const { return mExplosion; }

private:
	class PlaneActor* mPlane;
	class CloudActor* mCloud;
	class ExplosionActor* mExplosion;
};