#pragma once
#include <stack>

class SceneManager
{
public:
	enum class SceneType
	{
		ETitle,
		EDemo,
		EGame,
		EResult
	};

	SceneManager(class GameApp* gameApp);
	~SceneManager();
	
	void RunLoop();
	void ChangeScene(SceneType scene, const class Parameter& parameter, bool stackClear);

	class Renderer* GetRenderer();
	class InputSystem* GetInputSystem();
	class AudioSystem* GetAudioSystem();
	class Window* GetWindow();

private:
	std::stack<class BaseScene*> mSceneStack;
	class GameApp* mApp;
};
