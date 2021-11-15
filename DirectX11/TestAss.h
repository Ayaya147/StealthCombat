#pragma once

class TestAss
{
public:
	TestAss(class Renderer* renderer);
	void Draw();
	void TestDxtex();

private:
	class Renderer* mRenderer;
	float mAngle = 0.0f;

	unsigned int mNum;
};
