#include "gtest/gtest.h"

#include "./ShaderUtility.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <GL/glew.h>

class ShaderUtilityTests : public ::testing::Test {
protected:

	ShaderUtilityTests() {
		Shaders::ShaderPointer shaderTest(new Shaders::ShaderUtility());		
		defaultShaderProgram.reset(new Shaders::ShaderUtility());		
	}

	//Tear Down for this test fixture
	~ShaderUtilityTests() {
		defaultShaderProgram.reset();
	}

	Shaders::ShaderPointer defaultShaderProgram;

	std::string filePathVertex{ "./VertexShader.txt" };
	std::string filePathFragment{ "./FragmentShader.txt" };

	std::string badVertexShader{ "./BadVertexShader.txt" };
	std::string badFragmentShader{ "./BadFragmentShader.txt" };

	std::string noExistentPath{ "D:/BadPath/DoesntExist" };

};


TEST_F(ShaderUtilityTests, TestShaderContructor) {		
	EXPECT_EQ(0, defaultShaderProgram->GetShaderProgramID());	
}

//Testing rule of thumb, dont test the implementation, test the behavior
TEST_F(ShaderUtilityTests, TestReadingShaderFile) {
	std::array<std::string, Shaders::maxShaderType> shaderCodeFiles;

	ASSERT_EQ(true, Shaders::FileReader(filePathVertex, shaderCodeFiles[Shaders::ShaderUtility::ShaderType::VERTEX]));
	ASSERT_EQ(true, Shaders::FileReader(filePathFragment, shaderCodeFiles[Shaders::ShaderUtility::ShaderType::FRAGMENT]));

	//I was originally going to test if the text read was the same text in the file but that test is flawed since changing the code
	//while building my engine would never affect a test the test for some specific text in a spacific file
	//USE THIS AS AN EXAMPLE OF HOW TO TEST - for campminder
	EXPECT_EQ(false, shaderCodeFiles[Shaders::ShaderUtility::ShaderType::VERTEX].empty());
	EXPECT_EQ(false, shaderCodeFiles[Shaders::ShaderUtility::ShaderType::FRAGMENT].empty());

	shaderCodeFiles[Shaders::ShaderUtility::ShaderType::VERTEX].clear();
	shaderCodeFiles[Shaders::ShaderUtility::ShaderType::FRAGMENT].clear();

	EXPECT_EQ(false, Shaders::FileReader(noExistentPath, shaderCodeFiles[Shaders::ShaderUtility::ShaderType::VERTEX]));
	EXPECT_EQ(false, Shaders::FileReader(noExistentPath, shaderCodeFiles[Shaders::ShaderUtility::ShaderType::FRAGMENT]));

	EXPECT_EQ(true, shaderCodeFiles[Shaders::ShaderUtility::ShaderType::VERTEX].empty());
	EXPECT_EQ(true, shaderCodeFiles[Shaders::ShaderUtility::ShaderType::FRAGMENT].empty());
}

TEST_F(ShaderUtilityTests, TestCompilingShader) {	
	//test if these are switched
	//test when only one shader is compiled	

	sf::Window openGLContext(sf::VideoMode(800, 600), "Test Context", sf::Style::Default, sf::ContextSettings(24));
	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::GLGRAPICS::FAILED_INITIALIZING_GLEW_INIT" << std::endl;
	}
	defaultShaderProgram->LoadShaderFile(filePathVertex, filePathFragment);
	EXPECT_EQ(true, defaultShaderProgram->Compile());

	defaultShaderProgram.reset(new Shaders::ShaderUtility());
	defaultShaderProgram->LoadShaderFile(filePathFragment, filePathVertex);
	EXPECT_EQ(false, defaultShaderProgram->Compile());

	defaultShaderProgram.reset(new Shaders::ShaderUtility());
	defaultShaderProgram->LoadShaderFile(badVertexShader, filePathFragment);
	EXPECT_EQ(false, defaultShaderProgram->Compile());


	defaultShaderProgram.reset(new Shaders::ShaderUtility());
	defaultShaderProgram->LoadShaderFile(filePathVertex, badFragmentShader);
	EXPECT_EQ(false, defaultShaderProgram->Compile());
}

TEST_F(ShaderUtilityTests, TestFindingUniforms) {

}
