#pragma once
#include <stack>

class Renderer;
class Parameter;
class InputSystem;

enum class SceneType
{
	ETitle,
	EGame,
	EResult,
};

class SceneManager
{
public:
	SceneManager(class GameApp* gameApp);
	~SceneManager();
	
	void RunLoop();
	void ChangeScene(SceneType scene, const Parameter& parameter, bool stackClear);

	Renderer* GetRenderer();
	InputSystem* GetInputSystem();
	class Window* GetWindow();
	//const std::stack<class BaseScene*>& GetScene() const { return mSceneStack; }

private:
	std::stack<class BaseScene*> mSceneStack;
	class GameApp* mApp;
};
