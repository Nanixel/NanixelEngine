#pragma once
#include <string>
#include <memory>
#include <vector>

#include "Entity.h"
#include "Component.h"
#include "Message.h"


namespace Engine {

	enum SystemType {
		EC_CustomWindow,
		EC_GLGraphics,
		EC_CameraSystem,
		MovementSys,
		GameStateSys,
		MAX
	};

	struct System {

		//remember to fill the mask here
		System(std::string &name, SystemType type) : _name(name), _type(type), _mask(MC_ALIVE) {}

		virtual ~System() {};		

		virtual void Init() = 0;		
		virtual void Update(float) = 0;
		virtual void ShutDown() = 0;

		virtual void SendMsg(EntityPointer entityOne, EntityPointer entity2, Message::Message message) {}

		void RegisterComponent(BitFieldComponent component);

		inline mask Mask() {
			return _mask;
		}

		//only the Space and Engine class can access the private methods of a system
		friend class Space;
		friend class Engine;

	protected:
		mask _mask;
		EntityPointerList _entities;
		const SystemType _type;
		std::string _name;
	private:
		System() = delete;
	};
	using SystemPointer = std::shared_ptr<System>;
	using SystemPointerList = std::vector<SystemPointer>;


}