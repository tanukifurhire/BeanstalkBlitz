
/* Start Header ************************************************************************/
/*!
\file		PieTimer.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file declares a simple pie timer ui object.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "RectPlayer.hpp"

class PieTimer : public RectPlayer {
public:

	void Enter() override;

	void PostUpdate() override;

	f32& TimerProgress() {
		return timer_progress;
	}

protected:
	f32 timer_progress{};
};