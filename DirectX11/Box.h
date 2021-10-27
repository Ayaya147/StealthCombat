#pragma once

class Box
{
public:
	Box(class Renderer* renderer);
	void Draw();

private:
	class Renderer* mRenderer;
	float mAngle = 0.f;
};
