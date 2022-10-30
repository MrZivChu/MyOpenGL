#pragma once

#include <vector>
#include <functional>

class InputHelper
{
private:
	using KeyCallback = std::function<void(int key, int scancode, int action, int mode)>;
	using CursorCallback = std::function<void(double xpos, double ypos)>;
	using ScrollCallback = std::function<void(double xoffset, double yoffset)>;
	using MouseCallback = std::function<void(int key, int xpos, int ypos)>;

	static std::vector<KeyCallback> keyCallbacks;
	static std::vector<CursorCallback> cursorCallbacks;
	static std::vector<ScrollCallback> scrollCallbacks;
	static std::vector<MouseCallback> mouseCallbacks;

public:
	void Init();
	void AttachKeyCallback(KeyCallback callback);
	void AttachCursorCallback(CursorCallback callback);
	void AttachScrollCallback(ScrollCallback callback);
	void AttachMouseCallback(MouseCallback callback);
};

class InputHelperService
{
public:
	static InputHelper& Get()
	{
		static InputHelper instance;
		return  instance;
	}
};

