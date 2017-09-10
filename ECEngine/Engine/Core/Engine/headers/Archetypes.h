#pragma once
#include "./Entity.h"
#include "../ECEngine/Engine/Core/Components/ComponentsInclude.h"
#include <memory>

namespace Engine {

	//CreateArchtypeFn is a functionat that takes void and returns an EntityPointer
	typedef EntityPointer(*CreateArchtypeFn)();

	namespace Archtypes {

		EntityPointer DefaultCamera();

		EntityPointer Default2DCamera();

		EntityPointer BoxGameObject();

		EntityPointer Player();

	}
}