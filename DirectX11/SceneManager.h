#pragma once
#include <memory>
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
	SceneManager();
	~SceneManager();
	
	void ChangeScene(SceneType scene, const class Parameter& parameter, bool stackClear);
};

