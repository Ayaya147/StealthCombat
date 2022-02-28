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
	void Draw(class Renderer* renderer);

	FadeState GetFadeState() const { return mFadeState; }
	float GetAlpha() const { return mAlpha; }
	void SetFadeState(FadeState state) { mFadeState = state; }

private:
	struct Transforms
	{
		DirectX::XMMATRIX mWorldTransform;
		DirectX::XMMATRIX mViewProj;
	};

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 col;
		DirectX::XMFLOAT2 tc;
	};

	class BaseScene* mScene;
	float mRate;
	float mAlpha;
	FadeState mFadeState;

	class Texture* mTexture;
	class VertexBuffer* mVertexBuffer;
	VertexConstantBuffer<Transforms>* mBuffer;
};
