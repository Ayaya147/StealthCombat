#pragma once
#include <DirectXMath.h>

template<class C>
class VertexConstantBuffer;

class Fade
{
public:
	enum class FadeState
	{
		EFadeNone,
		EFadeIn,
		EFadeOut,
	};

	Fade(class BaseScene* scene);
	~Fade();

	void Update(float deltaTime);
	void Draw(class Renderer* renderer, class VertexBuffer* vertexBuffer);

	FadeState GetFadeState() const { return mFadeState; }
	float GetAlpha() const { return mAlpha; }
	void SetFadeState(FadeState state) { mFadeState = state; }

private:
	struct Transforms
	{
		DirectX::XMMATRIX mWorldTransform;
		DirectX::XMMATRIX mViewProj;
	};

	class BaseScene* mScene;
	float mRate;
	float mAlpha;
	FadeState mFadeState;

	class Texture* mTexture;
	VertexConstantBuffer<Transforms>* mCBuffer;
};
