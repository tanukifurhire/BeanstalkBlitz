/* Start Header ************************************************************************/
/*!
\file	    Event.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file declares an Event object class responsible for communicating events to subscribers.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include <functional>

using Callback = std::function<void(void*)>;

class Event {
public:
	Event() {};

	void Emit(void* context) {
		for (const Callback& callback : callback_list) {
			if (callback) {
				callback(context);
			}
		}
	}

	void AddCallback(const Callback& callback) {
		if (callback) {
			callback_list.push_back(callback);
		}
	}

	void RemoveCallback(const Callback& callback) {
		callback_list.erase(
			std::remove_if(callback_list.begin(), callback_list.end(),
				[&callback](const Callback& child) { return child.target_type() == callback.target_type(); }),
			callback_list.end());

		callback_list.shrink_to_fit();
	}

	Event& operator+=(const Callback& callback) {
		if (callback) {
			callback_list.push_back(callback);
		}

		return *this;
	}

	Event& operator-=(const Callback& callback) {
		callback_list.erase(
			std::remove_if(callback_list.begin(), callback_list.end(),
				[&callback](const Callback& child) { return child.target_type() == callback.target_type(); }),
			callback_list.end());

		callback_list.shrink_to_fit();

		return *this;
	}

protected:
	std::vector<Callback> callback_list;
};