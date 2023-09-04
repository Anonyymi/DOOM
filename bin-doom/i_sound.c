#include <i_sound.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "doomdef.h"
#include "i_system.h"
#include "mmus2mid.h"

void I_InitSound()
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
    {
        I_Error("error initializing SDL_INIT_AUDIO");
    }
}

void I_UpdateSound(void)
{

}

void I_SubmitSound(void)
{

}

void I_ShutdownSound(void)
{

}


//
//  SFX I/O
//

void I_SetChannels()
{

}

int I_GetSfxLumpNum(sfxinfo_t* sfxinfo)
{
    return 0;
}

int I_StartSound(int id, int vol, int sep, int pitch, int priority)
{
    return 0;
}

void I_StopSound(int handle)
{

}

int I_SoundIsPlaying(int handle)
{
    return 0;
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch)
{

}


//
//  MUSIC I/O
//

Mix_Music * mus;

void I_InitMusic(void)
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
    {
        I_Error("error initializing SDL_INIT_AUDIO");
    }

    if (Mix_Init(MIX_INIT_MID) != 0)
    {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 0);
    }
}

void I_ShutdownMusic(void)
{
    Mix_CloseAudio();
    Mix_Quit();
}

void I_SetMusicVolume(int volume)
{
    float f_volume = ((float) volume) / 127.0f;
    f_volume *= (float) MIX_MAX_VOLUME;
    Mix_VolumeMusic((int) f_volume);
}

void I_PauseSong(int handle)
{
    Mix_PauseMusic();
}

void I_ResumeSong(int handle)
{
    Mix_ResumeMusic();
}

int I_RegisterSong(void *data)
{
    MIDI midi;
    if (mmus2mid(data, &midi, 0, 1) != 0)
    {
        I_Error("failed to convert MUS to MID");
    }

    UBYTE * midi_data = NULL;
    int midi_len = 0;
    if (MIDIToMidi(&midi, &midi_data, &midi_len) != 0)
    {
        I_Error("failed to convert structural MIDI to raw MIDI");
    }

    SDL_RWops * buf = SDL_RWFromMem(midi_data, midi_len);
    if (buf == NULL)
    {
        I_Error("failed to load MIDI data into memory");
    }

    mus = Mix_LoadMUSType_RW(buf, MUS_MID, SDL_TRUE);
    if (mus == NULL)
    {
        I_Error("failed to load MIDI data into SDL2_mixer");
    }

    free(midi_data);
    free_mididata(&midi);

    return 0;
}

void I_PlaySong(int handle, int looping)
{
    Mix_PlayMusic(mus, looping > 0 ? -1 : 1);
}

void I_StopSong(int handle)
{
    Mix_HaltMusic();
}

void I_UnRegisterSong(int handle)
{
    Mix_FreeMusic(mus);
}
