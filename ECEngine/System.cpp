#include "stdafx.h"

#include "./System.h"

namespace Engine {

	void System::RegisterComponent(BitFieldComponent component) {
		//bitwise add
		//adds this component to the mask for this system (aka all the components that encompass this system)
		_mask = _mask | component;
	}
}