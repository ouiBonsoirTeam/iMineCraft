#include <glimac/Sound.hpp>

void playSample(FMOD_SYSTEM *systeme, FMOD_CHANNEL *canal, FMOD_SOUND *son, const char *dir, float volume)
{
	FMOD_RESULT resultat;

	// Ouverture du son
	resultat = FMOD_System_CreateSound(systeme, dir, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESAMPLE, 0, &son);
	if (resultat != FMOD_OK)
	{
		fprintf(stderr, "Impossible de lire %s\n", dir);
		exit(EXIT_FAILURE);
	}

	FMOD_System_PlaySound(systeme, FMOD_CHANNEL_FREE, son, 0, NULL);
	FMOD_Channel_SetVolume(canal, volume);
}

void playMusic(FMOD_SYSTEM *systeme, FMOD_SOUND *son, const char *dir)
{
	FMOD_RESULT resultat;

	// Ouverture de la musique
	resultat = FMOD_System_CreateSound(systeme, dir, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &son);
	if (resultat != FMOD_OK)
	{
		fprintf(stderr, "Impossible de lire %s\n", dir);
		exit(EXIT_FAILURE);
	}

	FMOD_Sound_SetLoopCount(son, 1);	// Musique en loop

	FMOD_CHANNELGROUP *canal;
	FMOD_BOOL etat;
	FMOD_System_GetMasterChannelGroup(systeme, &canal);
	FMOD_ChannelGroup_GetPaused(canal, &etat);

	FMOD_System_PlaySound(systeme, FMOD_CHANNEL_FREE, son, 0, NULL);
}
