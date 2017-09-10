#include "stdafx.h"
#include <cstdlib>
#include <string>


bool Active;

void ShutDown() { Active = false; }

float mixValue = 0.0;


//might want to change this to take a function
//void OnKeyDown(Keys key) {
//	switch (key)
//	{
//	case Keys::Escape:
//		std::cout << "Escape was pressed" << std::endl;
//		break;
//	case Keys::A:
//		std::cout << "A was pressed" << std::endl;
//		break;
//	default:
//		break;
//	}
//}


void testStackStuff() {
	//test basic allocator clone
	//scopeTest();

	//testAddingElements();

	//testStackAllocator();
}


//int main(int argc, char* argv[]) {	
//	
//
//   //###################### WINDOW STUFF #################################
//	//auto windowInstance = std::shared_ptr<jess::CustomWindow>(new jess::CustomWindow(1280, 720, "jess window"));
//	////might not need this -> this is setting a static pointer
//	//jess::CustomWindow::setInstance(windowInstance.get());
//
//	//windowInstance->init();
//	//windowInstance->onClose = &ShutDown;
//	////this is only going to fill the key subscriber
//	//windowInstance->Subscribe(&OnKeyDown);
//	//windowInstance->Subscribe(&onWheelScroll);
//	//windowInstance->hideMouseCursor();
//	//
//	//Active = true;	
//	////###################### WINDOW STUFF #################################
//	//glewInit();
//	////initOther();
//	////Shader ourShader("VertexShader.txt", "FragmentShader.txt");
//	//Shader ourShader("SqVertexShader.txt", "SqFragmentShader.txt");	
//	//makeSqaure(&ourShader);
//	//SCREEN_W = (float)windowInstance->width;
//	//SCREEN_H = (float)windowInstance->height;
//	////setUpCoordSystem(windowInstance->width, windowInstance->height);
//	////testTranslation();
//	//
//
//	//while (Active) {
//	//	glViewport(0, 0, windowInstance->width, windowInstance->height);		
//	//	//drawHelloTriangle(&ourShader);		
//	//	drawHelloSquare(&ourShader, mixValue);
//
//	//	windowInstance->update();
//	//}
//	
//	return 0;
//}