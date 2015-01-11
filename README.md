Oui Bonsoir's iMineCraft
==========
An IMAC Engineering School OpenGL Project.
----------
![Oui Bonsoir](http://f.cl.ly/items/3P1I1q0Z2P1S3Y1q2J2W/base.png "")
[*Triton Texture Map Pack*](http://tritonsite.weebly.com "")


##Supported OS (require OpenGL 3+)
	- Linux Ubuntu
	- Mac OSX (tested on Yosemite)
	- Windows soon
  
  
##Features
1. Chunk Manager - infinite world generation in real time
2. Physics - colision in all directions
3. OBJ loader - a lander, a crowbar, a horse and a bed
4. Texture Map - multiple block type
5. Lightning - directional light, point light
6. Occlusion - precalculated shadow generation with an occlusion texture map
7. Billboarding - for the point light and the HUD
8. Skybox - 
9. Sound design ([*Half-Life 2 - All Sounds Replaced With My Voice Mod*](http://www.fileplanet.com/220777/220000/fileinfo/Half-Life-2---All-Sounds-Replaced-With-My-Voice-Mod- "")) - ambient sound, different step sound, construct/destruct sound
10. Inventory - 
11. Save game - save constructed/destructed blocks, player location and inventory
  
##How to play
	- ZQSD : move
	- SPACE : jump or fly with your jetpack
	- LEFT CLICK : destruct a block
	- RIGHT CLICK : construct selected block
	- 123456789 (not num pad) : inventory selection
	- E : place the point light on the aim
	- T : spawn a bed
	- Y : teleport to the bed, or to the lander if no bed created
  
  
##Prior installation to play the game
###For Mac users, please install brew, this is a really cool package manager
```sh
$ ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
  
####You need CMake
*Linux :*
```sh
$ sudo apt-get install cmake
```
*Mac OSX :*
```sh
$ brew install cmake
```
  
####You need some libs
**GLEW**  
*Linux :*
```sh
$ sudo apt-get install glew
```
*Mac OSX :*
```sh
$ brew install glew
```
  
**SDL2**  
*Linux :*
```sh
$ sudo apt-get install libsdl2-dev
```
*Mac OSX :*
```sh
$ brew install sdl2
```
  
**SDL2 MIXER**  
*Linux :*
```sh
$ sudo apt-get install libsdl2-mixer-dev
```
*Mac OSX :*
```sh
$ brew install sdl2_mixer
```
  
  
##Now you can install the game !
####Create your ``iMineCraft`` foler and clone the repo inside
```sh
$ git clone https://github.com/ouiBonsoirTeam/iMineCraft.git
```
####Create a ``iMinecraft-build`` folder next to ``iMineCraft`` and do the following
```sh
$ cd ../iMinecraft-build
```
####Build the project with
```sh
$ cmake ../iMineCraft
```
####Create the executable with
```sh
$ make -j
```
####Launch the game with
```sh
$ ./bin/Imacraft
```
  
  
###You can access your saved game in
``bin/assets/saves``
  
  
Have fun and be carefull, lava hurts !