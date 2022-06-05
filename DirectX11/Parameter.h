#pragma once

class Parameter
{
public:
	Parameter();
	~Parameter();

	void SetIsFromDemo(bool isFromDemo) { mIsFromDemo = isFromDemo; }
	void SetIsGameWin(bool win) { mIsGameWin = win; }
	bool GetIsFromDemo() const { return mIsFromDemo; }
	bool GetIsGameWin() const { return mIsGameWin; }

private:
	bool mIsFromDemo;
	bool mIsGameWin;
};
