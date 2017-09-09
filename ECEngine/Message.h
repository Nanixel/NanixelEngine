#pragma once

namespace Engine {

	namespace Message {

		using Message = unsigned;

		enum MessageValues {
			MSG_ESCAPE,
			MSG_MOUSE_SCROLL,
			MSG_A_PRESS,
			MSG_S_PRESS,
			MSG_D_PRESS,
			MSG_W_PRESS,
			MSG_ESC_PRESS,
			MSG_MOUSE_MOVE,
			MSG_UPDATE_RESOURCE
			//types of game messages
		};
	}
}
