#pragma once
#include "./Component.h"
#include <memory>
#include <vector>

namespace Engine {

	class DrawableComponent : public Component {
	protected:
		//only instances of this class and dirivatives of this class can access this
		bool Visible;
	public:
		DrawableComponent() : Component(EC_DrawableComponent, MC_Drawable) {}
		
		std::vector<float> verticies;
		int numberOfVerticies;
		//for now each drawable will only have one texture, but I can change this later
		unsigned int texture;

	};

	using DrawableComponentPointer = std::shared_ptr<DrawableComponent>;

}