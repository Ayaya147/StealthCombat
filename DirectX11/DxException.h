#pragma once
#include <string>
#include <comdef.h>

class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::string& functionName, const std::string& filename, int lineNumber)
		:
		mErrorCode(hr),
		mFunctionName(functionName),
		mFilename(filename),
		mLineNumber(lineNumber)
	{}

	std::string ToString() const
	{
		_com_error err(mErrorCode);
		std::string msg = err.ErrorMessage();
		return mFunctionName + " failed in " + mFilename + "; line " + std::to_string(mLineNumber) + "; error: " + msg;
	}

private:
	HRESULT mErrorCode = S_OK;
	std::string mFunctionName;
	std::string mFilename;
	int mLineNumber = -1;
};

#define ThrowIfFailed(x)                                                  \
{                                                                         \
    HRESULT hr__ = (x);                                                   \
    if(FAILED(hr__)) { throw DxException(hr__, #x, __FILE__, __LINE__); } \
}
