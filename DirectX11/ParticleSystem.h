#pragma once
#include <DirectXMath.h>
#include <d3d11.h>


class ParticleSystem
{
public:
	ParticleSystem(class Renderer* renderer);
	~ParticleSystem();

	void Init(class Renderer* renderer);
	void Update(class Renderer* renderer);
	void Draw(class Renderer* renderer);

	bool GetIsInit() { return mIsInit; }

private:
	bool mIsInit;
};
