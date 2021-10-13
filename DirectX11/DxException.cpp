#include "DxException.h"
#include <comdef.h>

DxException::DxException(HRESULT hr, const std::string& functionName, const std::string filename, int lineNumber)
	:
	mErrorCode(hr),
	mFunctionName(functionName),
	mFileName(filename),
	mLineNumber(lineNumber)
{
}

std::string DxException::ToString()
{
	_com_error err(mErrorCode);
	std::string msg = err.ErrorMessage();

	return mFunctionName + " failed in " + "; line " + std::to_string(mLineNumber) + "; error: " + msg;
}
