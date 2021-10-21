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
		return "Function: " + mFunctionName + ";\nFile: " + mFilename +
			";\nLine: " + std::to_string(mLineNumber) + ";\nError: " + msg;
	}

private:
	HRESULT mErrorCode = S_OK;
	std::string mFunctionName;
	std::string mFilename;
	int mLineNumber = -1;
};

#ifdef DEBUG
#define ThrowIfFailed(x)                                                  \
{                                                                         \
    HRESULT hr__ = (x);                                                   \
    if(FAILED(hr__)) { throw DxException(hr__, #x, __FILE__, __LINE__); } \
}
#else
#define ThrowIfFailed(x) (x)
#endif
