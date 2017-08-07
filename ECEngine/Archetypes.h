#pragma once
#include "./Entity.h"
#include "./ComponentsInclude.h"

namespace Engine {

	//CreateArchtypeFn is a functionat that takes void and returns an EntityPointer
	typedef EntityPointer(*CreateArchtypeFn)();

	namespace Archtypes {

		EntityPointer DefaultCamera();

		EntityPointer BoxGameObject();

		EntityPointer Player();

	}
}