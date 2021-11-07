#pragma once

class TestAss
{
public:
	TestAss(class Renderer* renderer);
	void Draw();

private:
	class Renderer* mRenderer;
	float mAngle = 0.0f;

	unsigned int mNum;
};
