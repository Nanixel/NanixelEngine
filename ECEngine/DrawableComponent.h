#pragma once
#include "./Component.h"
#include <memory>
#include <vector>

namespace Engine {

	class DrawableComponent : public Component {
	public:
		DrawableComponent() : Component(EC_DrawableComponent, MC_Drawable) {}
		
		std::vector<float> verticies;
		int numberOfVerticies;
		//for now each drawable will only have one texture, but I can change this later
		unsigned int texture;

	};

	using DrawableComponentPointer = std::shared_ptr<DrawableComponent>;

}