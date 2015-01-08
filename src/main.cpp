#include <GL/glew.h>
#include <iostream>
#include <unistd.h>

#include <stdlib.h>
#include <time.h>

#include <glimac/SDLWindowManager.hpp>
#include <SDL2/SDL_mixer.h>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <glimac/CustomProgram.hpp>
#include <glimac/FreeFlyCamera.hpp>
#include <glimac/Torch.hpp>
#include <glimac/Geometry.hpp>
#include <glimac/Sound.hpp>
#include <glimac/tiny_obj_loader.h>

#include "voxel_engine/Chunk.hpp"
#include "voxel_engine/ChunkManager.hpp"
#include "physics/Event_manager.hpp"
#include "voxel_engine/Block.hpp"
#include "Skybox.hpp"
#include "Light.hpp"
#include "Helmet.hpp"

using namespace glimac;

int main(int argc, char** argv) 
{
	// Initialize SDL and open a window
	SDLWindowManager windowManager("iMineCraft Oui Bonsoir", 1);

	glewExperimental = GL_TRUE;
	// Initialize glew for OpenGL3+ support
	GLenum glewInitError = glewInit();
	if(GLEW_OK != glewInitError) 
	{
		std::cerr << glewGetErrorString(glewInitError) << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

	/*********************************
	 * HERE SHOULD COME THE INITIALIZATION CODE
	 *********************************/

	Mix_Music *music = nullptr;
	std::vector<Mix_Chunk*> mix_chunk = initsound(music);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//For transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Chargement des shaders
    FilePath applicationPath(argv[0]);

	GeneralProgram gProgram(applicationPath);
	LightsProgram lightsProg(applicationPath);
	HelmetProgram hellProg(applicationPath);
	SkyboxProgram skyProgram(applicationPath);
	GeometryProgram geoProgram(applicationPath);

	//Load texture
	std::unique_ptr<Image> texturePointer;
	texturePointer = loadImage("../iMineCraft/assets/textures/glass_1024.png");
	if(texturePointer == NULL)
	{
		std::cerr << "Error while charging texture." << std::endl;
	}

	GLuint idTexture;
	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D,  idTexture);

	glTexImage2D(GL_TEXTURE_2D,  0,  GL_RGBA,  texturePointer->getWidth(),  
					texturePointer->getHeight(),  0,  GL_RGBA,  GL_FLOAT,  texturePointer->getPixels());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D,  0);

	std::string savesFolder = "bin/assets/saves/";
	ChunkManager chunkmanager;
	chunkmanager.initialize(savesFolder);

	FreeFlyCamera ffCam;
	chunkmanager.update(ffCam.getPosition(), ffCam.getFrontVector());

	ffCam.setPosition(glm::vec3(0,chunkmanager.getNoiseValue(0,0)+5,0));

	float angleYCurrent = 0;

	int crouch = 0;

	float breakCube = 0;

	const float CAMERA_ROT_FACTOR = 0.05f;

	int max_fps = 60;
	float lastTime = windowManager.getTime();
	float lastTime2 = windowManager.getTime();
	int nbFrames = 0;
	
	Skybox skybox;
		skybox.init(skyProgram);

	Geometry lander;
		lander.init(geoProgram, lander, "Lander.obj", true, "Lander.png");

	Geometry crowbar;
		crowbar.init(geoProgram, crowbar, "crowbar.obj", true, "metal01.jpg");

/*
	Geometry horse0;
		horse0.init(geoProgram, horse0, "horse_obj_0.obj", true, "horse_texture.png");
	Geometry horse1;
		horse1.init(geoProgram, horse1, "horse_obj_1.obj", true, "horse_texture.png");
	Geometry horse2;
		horse2.init(geoProgram, horse2, "horse_obj_2.obj", true, "horse_texture.png");
	Geometry horse3;
		horse3.init(geoProgram, horse3, "horse_obj_3.obj", true, "horse_texture.png");
	Geometry horse4;
		horse4.init(geoProgram, horse4, "horse_obj_4.obj", true, "horse_texture.png");
	Geometry horse5;
		horse5.init(geoProgram, horse5, "horse_obj_5.obj", true, "horse_texture.png");
	Geometry horse6;
		horse6.init(geoProgram, horse6, "horse_obj_6.obj", true, "horse_texture.png");
	Geometry horse7;
		horse7.init(geoProgram, horse7, "horse_obj_7.obj", true, "horse_texture.png");
	Geometry horse8;
		horse8.init(geoProgram, horse8, "horse_obj_8.obj", true, "horse_texture.png");
	Geometry horse9;
		horse9.init(geoProgram, horse9, "horse_obj_9.obj", true, "horse_texture.png");
	Geometry horse10;
		horse10.init(geoProgram, horse10, "horse_obj_10.obj", true, "horse_texture.png");
	Geometry horse11;
		horse11.init(geoProgram, horse11, "horse_obj_11.obj", true, "horse_texture.png");
	Geometry horse12;
		horse12.init(geoProgram, horse12, "horse_obj_12.obj", true, "horse_texture.png");
	Geometry horse13;
		horse13.init(geoProgram, horse13, "horse_obj_13.obj", true, "horse_texture.png");
	Geometry horse14;
		horse14.init(geoProgram, horse14, "horse_obj_14.obj", true, "horse_texture.png");
	Geometry horse15;
		horse15.init(geoProgram, horse15, "horse_obj_15.obj", true, "horse_texture.png");
	Geometry horse16;
		horse16.init(geoProgram, horse16, "horse_obj_16.obj", true, "horse_texture.png");
	Geometry horse17;
		horse17.init(geoProgram, horse17, "horse_obj_17.obj", true, "horse_texture.png");
	Geometry horse18;
		horse18.init(geoProgram, horse18, "horse_obj_18.obj", true, "horse_texture.png");
	Geometry horse19;
		horse19.init(geoProgram, horse19, "horse_obj_19.obj", true, "horse_texture.png");
	Geometry horse20;
		horse20.init(geoProgram, horse20, "horse_obj_20.obj", true, "horse_texture.png");
	Geometry horse21;
		horse21.init(geoProgram, horse21, "horse_obj_21.obj", true, "horse_texture.png");
	Geometry horse22;
		horse22.init(geoProgram, horse22, "horse_obj_22.obj", true, "horse_texture.png");
	Geometry horse23;
		horse23.init(geoProgram, horse23, "horse_obj_23.obj", true, "horse_texture.png");
*/

	Geometry sheep01;
		sheep01.init(geoProgram, sheep01, "sheep01.obj", true, "white.png");
	Geometry sheep02;
		sheep02.init(geoProgram, sheep02, "sheep02.obj", true, "white.png");
	Geometry sheep03;
		sheep03.init(geoProgram, sheep03, "sheep03.obj", true, "white.png");

	srand(time(NULL));

	Light sun = Light(glm::vec3(1,1,1), glm::vec3(-0.5,0.5,-0.5));

	Torch torch(glm::vec3(3,glm::round(chunkmanager.getNoiseValue(3,4))+5,4));

	// define current BlockType
	BlockType currentBlockType = BlockType_Earth;

	Inventory invent;

	Helmet helmet;

	int horseCurrent =0;
	int cnt =0;

	// Application loop:
	bool done = false;
	while(!done) 
	{
	
		chunkmanager.update(ffCam.getPosition(), ffCam.getFrontVector());
		
		// Event loop:
		event_manager(windowManager,ffCam,angleYCurrent,CAMERA_ROT_FACTOR,done,chunkmanager, invent, crouch, currentBlockType, 
						mix_chunk, breakCube, torch);

		// Measure speed
		float currentTime = windowManager.getTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 )
		{ 
		    //std::cout << "fps : " << nbFrames << std::endl;
		    nbFrames = 0;
		    lastTime += 1.0;
		}

		if (currentTime - lastTime2 < (1.f/max_fps) && currentTime - lastTime2 > 0)
		{
			usleep( (unsigned int)(((1.f/max_fps) - (currentTime - lastTime2))*2000000) ) ;
		}

		lastTime2 = currentTime;

		/*********************************
		 * HERE SHOULD COME THE RENDERING CODE
		 *********************************/

		glm::mat4 viewMatrix = ffCam.getViewMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyProgram.m_Program.use();
			skybox.draw(skyProgram, viewMatrix);

		geoProgram.m_Program.use();
			crowbar.drawCrowbar(geoProgram, crowbar, ffCam, breakCube);
			lander.draw(geoProgram, lander, viewMatrix, glm::vec3(4,glm::round(chunkmanager.getNoiseValue(4,8))+0.5,8), glm::vec3(0.5, 0.5, 0.5), 0, glm::vec3(1.0, 1.0, 1.0));
			
			/*
			if (horseCurrent==0)
				horse0.draw(geoProgram, horse0, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==1)
				horse1.draw(geoProgram, horse1, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==2)
				horse2.draw(geoProgram, horse2, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==3)
				horse3.draw(geoProgram, horse3, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==4)
				horse4.draw(geoProgram, horse4, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==5)
				horse5.draw(geoProgram, horse5, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==6)
				horse6.draw(geoProgram, horse6, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==7)
				horse7.draw(geoProgram, horse7, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==8)
				horse8.draw(geoProgram, horse8, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==9)
				horse9.draw(geoProgram, horse9, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==10)
				horse10.draw(geoProgram, horse10, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==11)
				horse11.draw(geoProgram, horse11, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==12)
				horse12.draw(geoProgram, horse12, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==13)
				horse13.draw(geoProgram, horse13, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==14)
				horse14.draw(geoProgram, horse14, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==15)
				horse15.draw(geoProgram, horse15, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==16)
				horse16.draw(geoProgram, horse16, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==17)
				horse17.draw(geoProgram, horse17, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==18)
				horse18.draw(geoProgram, horse18, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==19)
				horse19.draw(geoProgram, horse19, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==20)
				horse20.draw(geoProgram, horse20, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==21)
				horse21.draw(geoProgram, horse21, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==22)
				horse22.draw(geoProgram, horse22, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==23)
				horse23.draw(geoProgram, horse23, viewMatrix, glm::vec3(ffCam.getPosition().x+5,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+5,ffCam.getPosition().z+5))+0.5,ffCam.getPosition().z+5), glm::vec3(2, 2, 2), ffCam.getPhi(), glm::vec3(0.0, 1.0, 0.0));
			*/

			if (horseCurrent==0)
				sheep01.draw(geoProgram, sheep01, viewMatrix, glm::vec3(ffCam.getPosition().x+1,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+1,ffCam.getPosition().z+1))+1,ffCam.getPosition().z+1), glm::vec3(0.5, 0.5, 0.5), ffCam.getPhi()-45, glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==1)
				sheep02.draw(geoProgram, sheep02, viewMatrix, glm::vec3(ffCam.getPosition().x+1,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+1,ffCam.getPosition().z+1))+1,ffCam.getPosition().z+1), glm::vec3(0.5, 0.5, 0.5), ffCam.getPhi()-45, glm::vec3(0.0, 1.0, 0.0));
			if (horseCurrent==2)
				sheep03.draw(geoProgram, sheep03, viewMatrix, glm::vec3(ffCam.getPosition().x+1,glm::round(chunkmanager.getNoiseValue(ffCam.getPosition().x+1,ffCam.getPosition().z+1))+1,ffCam.getPosition().z+1), glm::vec3(0.5, 0.5, 0.5), ffCam.getPhi()-45, glm::vec3(0.0, 1.0, 0.0));
			
			if (cnt==7)
			{
				horseCurrent = (horseCurrent+1)%3;
				cnt =0;
			}
			else ++cnt;

		lightsProg.m_Program.use();
			torch.translatePos(glm::sin(windowManager.getTime()) * glm::vec3(0,0.02,0));
			torch.computeLight(lightsProg, ffCam);
			sun.initMaterial(glm::vec3(1,1,1), glm::vec3(1,1,1), 2.f);
			sun.computeLight(lightsProg, ffCam.getViewMatrix());

			chunkmanager.render(lightsProg, ffCam.getViewMatrix());

		gProgram.m_Program.use();
			torch.drawBillboard(gProgram, ffCam);

		hellProg.m_Program.use();
			helmet.setPosition(ffCam.getPosition() + glm::vec3(ffCam.getFrontVector().x * 0.15, ffCam.getFrontVector().y * 0.15, ffCam.getFrontVector().z * 0.15 ));
			helmet.drawBillboard(hellProg, ffCam);

		// Update the display
		windowManager.swapBuffers();
	}

	// Unload and save the displayed map
	chunkmanager.unloadWorld();
	
	skybox.destruct();
	crowbar.destruct();

	glDeleteTextures(1, &idTexture);
	deletesound(mix_chunk, music);

	return EXIT_SUCCESS;
}
