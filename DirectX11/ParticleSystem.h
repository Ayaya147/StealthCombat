#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

class ParticleSystem
{
public:
	ParticleSystem(class Renderer* renderer);
	~ParticleSystem();

	void Update(class Renderer* renderer);
	void Draw(class Renderer* renderer);

private:
	class ComputeShader* mParticleInitShader;
	class ComputeShader* mParticleEmitShader;
	class ComputeShader* mParticleUpdateShader;
	
	class Renderer* mRenderer;
};
