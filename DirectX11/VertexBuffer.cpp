#include "VertexBuffer.h"

void VertexBuffer::Bind(Renderer* renderer)
{
	const UINT offset = 0;
	renderer->GetContext()->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &mStride, &offset);
}
