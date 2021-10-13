#pragma once
#include <Windows.h>
#include <string>

class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::string& functionName, const std::string filename, int lineNumber);

	std::string ToString();

	HRESULT mErrorCode = S_OK;
	std::string mFunctionName;
	std::string mFileName;
	int mLineNumber = -1;
};

#define ReleaseCom(x) {if(x) {x->Release(); x=0;}}
#define ThrowIfFailed(x)                                               \
{                                                                      \
	HRESULT hr = (x);                                                  \
	if (FAILED(hr)) { throw DxException{hr, #x, __FILE__, __FILE__}; } \
}
