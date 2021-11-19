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
	SceneManager(class GameApp* app);
	~SceneManager();
	
	void RunLoop();
	void ChangeScene(SceneType scene, const class Parameter& parameter, bool stackClear);

	class GameApp* GetApp() { return mApp; }

private:
	class GameApp* mApp;
	std::stack<class BaseScene*> mSceneStack;
};
