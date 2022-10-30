#pragma once

#include <vector>
#include <functional>
class EngineEventHandler
{
private:
	using StartCallback = std::function<void()>;
	using UpdateCallback = std::function<void(float deltaTime)>;
	using QuitCallback = std::function<void()>;

	static std::vector<StartCallback> startCallbacks;
	static std::vector<UpdateCallback> updateCallbacks;
	static std::vector<QuitCallback> quitCallbacks;
public:
	EngineEventHandler() = delete;

	static std::vector<StartCallback>& GetStartCallbacks()
	{
		return startCallbacks;
	}

	static std::vector<UpdateCallback>& GetUpdateCallbacks()
	{
		return updateCallbacks;
	}

	static std::vector<QuitCallback>& GetQuitCallbacks()
	{
		return quitCallbacks;
	}

	static void AttachStartCallback(StartCallback callback)
	{
		startCallbacks.emplace_back(callback);
	}
	static void AttachUpdateCallback(UpdateCallback callback)
	{
		updateCallbacks.emplace_back(callback);
	}
	static void AttachQuitCallback(QuitCallback callback)
	{
		quitCallbacks.emplace_back(callback);
	}
};

