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
	SceneManager(Renderer* renderer, InputSystem* input);
	~SceneManager();
	
	void RunLoop();
	void ChangeScene(SceneType scene, const Parameter& parameter, bool stackClear);

	Renderer* GetRenderer() { return mRenderer; }
	InputSystem* GetInputSystem() { return mInputSystem; }
	//const std::stack<class BaseScene*>& GetScene() const { return mSceneStack; }

private:
	Renderer* mRenderer;
	InputSystem* mInputSystem;
	std::stack<class BaseScene*> mSceneStack;
};
