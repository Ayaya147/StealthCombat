#pragma once

class Parameter
{
public:
	Parameter();
	~Parameter();

	void SetIsFromDemo(bool isFromDemo) { mIsFromDemo = isFromDemo; }
	bool GetIsFromDemo() const { return mIsFromDemo; }

private:
	bool mIsFromDemo;
};
