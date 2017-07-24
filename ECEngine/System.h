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

		//inheriting class MUST define these
		virtual void Init() = 0;		
		virtual void Update(float) = 0;
		virtual void ShutDown() = 0;
		//used to send messages to a system
		virtual void SendMsg(EntityPointer entityOne, EntityPointer entity2, Message::Message message) {}

		//specifies a component type required for the system to function
		void RegisterComponent(BitFieldComponent component);

		//NOTE engine automatically pre-fills the system's _entities member with ALL entities in the current space that meet the mask requirements
		inline mask Mask() {
			return _mask;
		}

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