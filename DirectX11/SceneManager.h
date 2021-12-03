#pragma once
#include <stack>

class Renderer;
class Parameter;

enum class SceneType
{
	ETitle,
	EGame,
	EResult,
};

class SceneManager
{
public:
	SceneManager(Renderer* renderer);
	~SceneManager();
	
	void RunLoop();
	void ChangeScene(SceneType scene, const Parameter& parameter, bool stackClear);

	Renderer* GetRenderer() { return mRenderer; }
	//const std::stack<class BaseScene*>& GetScene() const { return mSceneStack; }

private:
	Renderer* mRenderer;
	std::stack<class BaseScene*> mSceneStack;
};
