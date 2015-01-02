#include <GL/glew.h>
#include <iostream>
#include <unistd.h>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/CustomProgram.hpp>
#include <glimac/FreeFlyCamera.hpp>
<<<<<<< HEAD
#include <glimac/Torch.hpp>

#include "voxel_engine/Chunk.hpp"
#include "voxel_engine/ChunkManager.hpp"
#include "physics/Event_manager.hpp"
#include "Skybox.hpp"

=======
#include "../third-party/api/MAC/include/fmod.h"
#include "../third-party/api/LINUX/include/fmod.h"
#include <glimac/Sound.hpp>
>>>>>>> sound-FMODEX

using namespace glimac;



int main(int argc, char** argv) {
<<<<<<< HEAD
	// Initialize SDL and open a window
	SDLWindowManager windowManager("iMineCraft Oui Bonsoir", 1);

	glewExperimental = GL_TRUE;
	// Initialize glew for OpenGL3+ support
	GLenum glewInitError = glewInit();
	if(GLEW_OK != glewInitError) {
		std::cerr << glewGetErrorString(glewInitError) << std::endl;
		return EXIT_FAILURE;
	}
=======
    // Musique du jeu
    FMOD_SYSTEM *music;
    FMOD_System_Create(&music);
    FMOD_System_Init(music, 2, FMOD_INIT_NORMAL, NULL);
    int playing = 0;

    // Initialize SDL and open a window
    SDLWindowManager windowManager("Test Camera FreeFly", 0);
>>>>>>> sound-FMODEX

	std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

	/*********************************
	 * HERE SHOULD COME THE INITIALIZATION CODE
	 *********************************/

	glEnable(GL_DEPTH_TEST);

	//Chargement des shaders
    FilePath applicationPath(argv[0]);

<<<<<<< HEAD
	//comme P ne change jamais on peut la declarer a l'initialisation
	glm::mat4 matrixP = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

	GeneralProgram gProgram(applicationPath);
	pointLightProgram lProgram(applicationPath);
	SkyboxProgram skyProg(applicationPath);

	// gProgram.m_Program.use();
	// lProgram.m_Program.use();
	// skyProg.m_Program.use();

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

	//Initialisation camera freefly
	FreeFlyCamera ffCam;

	// // TEST
	ChunkManager chunkmanager;
	chunkmanager.initialize("bin/assets/saves");

	// Chunk chunk;
	// chunk.init();

	// chunk.buildMesh();

	//initialisation angle
	float angleX = 0;
	float angleY = 0;
	float angleYfinal = 0;

	const float CAMERA_ROT_FACTOR = 0.05f;

	int max_fps = 60;
	float lastTime = windowManager.getTime();
	float lastTime2 = windowManager.getTime();
	int nbFrames = 0;
	
	//make me a skybox
	Skybox skybox;
	skybox.init(skyProg);

	// // make me a torch
	// Torch torch;

	// Application loop:
	bool done = false;
	while(!done) {

		chunkmanager.update(ffCam.getPosition(), ffCam.getFrontVector());
		
		// Event loop:
		event_manager(windowManager,ffCam,angleX,angleY,angleYfinal,CAMERA_ROT_FACTOR,done,*chunkmanager.getChunk(0,0,0));			

		// Measure speed
		float currentTime = windowManager.getTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 )
		{ 
		    std::cout << "fps : " << nbFrames << std::endl;
		    nbFrames = 0;
		    lastTime += 1.0;
		}

		//std::cout<<"currentTime - lastTime2 : "<< (currentTime - lastTime2) << std::endl;
		//std::cout<<"1/max_fps : "<< (1.f/max_fps) << std::endl;
		//std::cout<<"diff : "<< (1.f/max_fps) - (currentTime - lastTime2) << std::endl;
		if (currentTime - lastTime2 < (1.f/max_fps) && currentTime - lastTime2 > 0)
		{
			usleep( (unsigned int)(((1.f/max_fps) - (currentTime - lastTime2))*2000000) ) ;
		}

		lastTime2 = currentTime;

		/*********************************
		 * HERE SHOULD COME THE RENDERING CODE
		 *********************************/
		
		// glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 viewMatrix = ffCam.getViewMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		skyProg.m_Program.use();
			skybox.draw(skyProg, viewMatrix);

		gProgram.m_Program.use();
			// torch.draw(lProgram, viewMatrix);

		
		chunkmanager.render(gProgram, ffCam.getViewMatrix());


		// chunk.render(gProgram, viewMatrix, idTexture);

		// Update the display
		windowManager.swapBuffers();

	}

	skybox.destruct();

	return EXIT_SUCCESS;
=======
    GeneralProgram program(applicationPath);
    program.m_Program.use();


    Sphere sphere (1, 32, 16);

    //création vbo
    GLuint vbo;
    glGenBuffers (1, &vbo);

    //bind le vbo
    glBindBuffer (GL_ARRAY_BUFFER, vbo);

    //buffer data void glBufferData(GLenum  target,  GLsizeiptr  size,  const GLvoid *  data,  GLenum  usage);
    glBufferData (GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(ShapeVertex),
                         sphere.getDataPointer(), GL_STATIC_DRAW);

    //debind le vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //creation vao
    GLuint vao;
    glGenVertexArrays(1, &vao);

    //bind le vao 
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMALE = 1;

    glEnableVertexAttribArray( VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray( VERTEX_ATTR_NORMALE);

    glBindBuffer ( GL_ARRAY_BUFFER, vbo);

    //void glVertexAttribPointer(GLuint  index,  GLint  size,  GLenum  type,  GLboolean  normalized,  GLsizei  stride,  const GLvoid *  pointer);
    glVertexAttribPointer ( VERTEX_ATTR_POSITION, 3, GL_FLOAT,
                    GL_FALSE, 1 * sizeof(ShapeVertex), (const GLvoid*) (0*sizeof(GLfloat)));
    glVertexAttribPointer ( VERTEX_ATTR_NORMALE, 3, GL_FLOAT,
                    GL_FALSE, 1 * sizeof(ShapeVertex), (const GLvoid*) (3*sizeof(GLfloat)));

    //debind le vbo
    glBindBuffer (GL_ARRAY_BUFFER, 0);
    
    //debind le vao
    glBindVertexArray(0);

    //Initialisation camera freefly
    FreeFlyCamera ffCam;

    //initialisation angle
    float angleX = 0;
    float angleY = 0;
    float angleYfinal = 0;

    const float CAMERA_ROT_FACTOR = 0.05f;

    //comme P ne change jamais on peut la declarer a l'initialisation
    glm::mat4 matrixP = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

    // Application loop:
    bool done = false;
    while(!done) {
        // Play the music
        if(playing == 0)
        {
            playMusic(music, NULL, "./bin/assets/sound/SOR_TFM_Atmos_65.mp3");
            playing = 1;
        }

        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    done = true; // Leave the loop after this iteration
                }
            }

            //souris
            if (e.type == SDL_MOUSEMOTION)
            {
                angleX -= e.motion.xrel * CAMERA_ROT_FACTOR;
                angleY -= e.motion.yrel * CAMERA_ROT_FACTOR;
                angleYfinal -= e.motion.yrel * CAMERA_ROT_FACTOR;
                angleYfinal = std::min(90.0f, std::max(-90.0f, angleYfinal)); //pour pas passer sa tête entre ses jambes
            }
        }
        ffCam.rotateLeft(angleX);
        if (angleYfinal != 90 && angleYfinal !=-90) ffCam.rotateUp(angleY);
        angleY = 0;
        angleX = 0;
        
        //touche clavier
        if(windowManager.isKeyPressed(SDLK_z)) 
        {
            ffCam.moveFront(0.1f);
        }
        else if(windowManager.isKeyPressed(SDLK_s)) 
        {
            ffCam.moveFront(-0.1f);
        }
        else if(windowManager.isKeyPressed(SDLK_q)) 
        {
            ffCam.moveLeft(0.1f);
        }
        else if(windowManager.isKeyPressed(SDLK_d)) 
        {
            ffCam.moveLeft(-0.1f);
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glm::mat4 matrixV = ffCam.getViewMatrix();

        glm::mat4 matrixM = glm::mat4(1.0); 

        glm::mat4 matrixMV = matrixV * matrixM;

        //calcul de la matrixViewProjetée
        glm::mat4 matrixMVP = matrixP * matrixMV;

        //calcul de la normal matrix = (MVinverse)Transposée
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(matrixMV));

        glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE,  glm::value_ptr(matrixMV));
        glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE,  glm::value_ptr(matrixMVP));
        glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE,  glm::value_ptr(normalMatrix));

        //bind du vao
        glBindVertexArray(vao);
        
        glClear(GL_COLOR_BUFFER_BIT);

        //dessine triangles
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        //debind du vao
        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return EXIT_SUCCESS;
>>>>>>> sound-FMODEX
}
