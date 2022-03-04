#pragma once
#include <vector>
#include <DirectXMath.h>

template<class C>
class VertexConstantBuffer;

class UIScreen
{
public:
	enum class UIState
	{
		EActive,
		EClosing
	};

	UIScreen(class BaseScene* scene, class Texture* texture);
	virtual ~UIScreen();

	virtual void Update(float deltaTime);
	virtual void ProcessInput();
	virtual void Draw(class Renderer* renderer, class VertexBuffer* vertexBuffer);
	void ComputeWorldTransform();

	void SetUIState(UIState state) { mState = state;}
	void SetPosition(const DirectX::XMFLOAT3& postion) { mPosition = postion;}
	void SetScale(float scale) { mScale = DirectX::XMFLOAT3{ mScale.x*scale,mScale.y*scale ,mScale.z*scale };}
	void SetRotation(const DirectX::XMFLOAT3& rotation) { mRotation = rotation;}

private:
	struct Transforms
	{
		DirectX::XMMATRIX mWorldTransform;
		DirectX::XMMATRIX mViewProj;
	};
	Transforms mTransforms;

	UIState mState;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;
	DirectX::XMFLOAT3 mScale;
	DirectX::XMMATRIX mWorldTransform;

	class BaseScene* mScene;
	class Texture* mTexture;
	static VertexConstantBuffer<Transforms>* mCBuffer;
	static int mCount;
};
