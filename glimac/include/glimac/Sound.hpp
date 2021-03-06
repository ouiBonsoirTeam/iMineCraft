#pragma once

#include <SDL2/SDL_mixer.h>
#include <vector>
#include <iostream>

std::vector<Mix_Chunk*> initsound(Mix_Music *music)
{
    Mix_OpenAudio(44100,AUDIO_S16SYS,1,20000);
    Mix_AllocateChannels(20);
    Mix_Volume(-1,MIX_MAX_VOLUME/2);

    //Mix_Music *music;
    std::vector<Mix_Chunk*> mix_chunk;

    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/run.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/grass.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/snow.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/stone.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/lava.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/jetpack.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/cube_select.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/add.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/remove.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/ice.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/wood.wav"));
    mix_chunk.push_back(Mix_LoadWAV("./bin/assets/sound/nocube.wav"));

    music = Mix_LoadMUS("./bin/assets/sound/ambiance.wav");
    if(!music) {
    std::cout<<"Impossible charger music"<<std::endl;
    }
    Mix_PlayMusic(music, -1);

    return mix_chunk;
}

void deletesound(std::vector<Mix_Chunk*> mix_chunk, Mix_Music *music)
{
    for (unsigned int i = 0; i < mix_chunk.size(); ++i)
    {
        Mix_FreeChunk(mix_chunk.at(i));
    }
    
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
}