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
	};

	class BaseScene* mScene;
	float mRate;
	float mAlpha;
	FadeState mFadeState;

	class VertexBuffer* mVertexBuffer;
	class InputLayout* mInputLayout;
	class PixelShader* mPixelShader;
	class VertexShader* mVertexShader;
	VertexConstantBuffer<Transforms>* mBuffer;
};
