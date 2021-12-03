#pragma once
#include <stack>

enum class SceneType
{
	ETitle,
	EGame,
	EResult,
};

class SceneManager
{
public:
	SceneManager(class Renderer* renderer);
	~SceneManager();
	
	void RunLoop();
	void ChangeScene(SceneType scene, const class Parameter& parameter, bool stackClear);

	class Renderer* GetRenderer() { return mRenderer; }
	//const std::stack<class BaseScene*>& GetScene() const { return mSceneStack; }

private:
	class Renderer* mRenderer;
	std::stack<class BaseScene*> mSceneStack;
};
