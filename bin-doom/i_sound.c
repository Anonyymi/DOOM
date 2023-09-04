#include <i_sound.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "doomdef.h"
#include "i_system.h"
#include "w_wad.h"
#include "z_zone.h"
#include "mmus2mid.h"

#define SAMPLECOUNT		512
#define NUM_CHANNELS		8
// It is 2 for 16bit, and 2 for two channels.
#define BUFMUL                  4
#define MIXBUFFERSIZE		(SAMPLECOUNT*BUFMUL)

#define SAMPLERATE		11025	// Hz
#define SAMPLESIZE		2   	// 16bit

// The actual lengths of all sound effects.
int 		lengths[NUMSFX];

//
// This function loads the sound data from the WAD lump,
//  for single sound.
//
void*
getsfx
( char*         sfxname,
  int*          len )
{
    unsigned char*      sfx;
    unsigned char*      paddedsfx;
    int                 i;
    int                 size;
    int                 paddedsize;
    char                name[20];
    int                 sfxlump;


    // Get the sound data from the WAD, allocate lump
    //  in zone memory.
    sprintf(name, "ds%s", sfxname);

    // Now, there is a severe problem with the
    //  sound handling, in it is not (yet/anymore)
    //  gamemode aware. That means, sounds from
    //  DOOM II will be requested even with DOOM
    //  shareware.
    // The sound list is wired into sounds.c,
    //  which sets the external variable.
    // I do not do runtime patches to that
    //  variable. Instead, we will use a
    //  default sound for replacement.
    if ( W_CheckNumForName(name) == -1 )
      sfxlump = W_GetNumForName("dspistol");
    else
      sfxlump = W_GetNumForName(name);

    size = W_LumpLength( sfxlump );

    // Debug.
    // fprintf( stderr, "." );
    //fprintf( stderr, " -loading  %s (lump %d, %d bytes)\n",
    //	     sfxname, sfxlump, size );
    //fflush( stderr );

    sfx = (unsigned char*)W_CacheLumpNum( sfxlump, PU_STATIC );

    // Pads the sound effect out to the mixing buffer size.
    // The original realloc would interfere with zone memory.
    paddedsize = ((size-8 + (SAMPLECOUNT-1)) / SAMPLECOUNT) * SAMPLECOUNT;

    // Allocate from zone memory.
    paddedsfx = (unsigned char*)Z_Malloc( paddedsize+8, PU_STATIC, 0 );
    // ddt: (unsigned char *) realloc(sfx, paddedsize+8);
    // This should interfere with zone memory handling,
    //  which does not kick in in the soundserver.

    // Now copy and pad.
    memcpy(  paddedsfx, sfx, size );
    for (i=size ; i<paddedsize+8 ; i++)
        paddedsfx[i] = 128;

    // Remove the cached lump.
    Z_Free( sfx );

    // Preserve padded length.
    *len = paddedsize;

    // Return allocated padded data.
    return (void *) (paddedsfx + 8);
}

Mix_Chunk* pcm[NUMSFX];

void I_InitSound()
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
    {
        I_Error("error initializing SDL_INIT_AUDIO");
    }

    if (Mix_Init(MIX_INIT_MID) != 0)
    {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 0);
    }


    // Initialize external data (all sounds) at start, keep static.
    fprintf( stderr, "I_InitSound: ");

    for (unsigned int i = 0; i < NUMSFX; ++i)
    {
        // Alias? Example is the chaingun sound linked to pistol.
        if (!S_sfx[i].link)
        {
          // Load data from WAD file.
          S_sfx[i].data = getsfx( S_sfx[i].name, &lengths[i] );
        }
        else
        {
          // Previously loaded already?
          S_sfx[i].data = S_sfx[i].link->data;
          lengths[i] = lengths[(S_sfx[i].link - S_sfx)/sizeof(sfxinfo_t)];
        }

        SDL_AudioSpec dst_spec;
        dst_spec.freq = 44100;
        dst_spec.format = MIX_DEFAULT_FORMAT;
        dst_spec.channels = MIX_DEFAULT_CHANNELS;
        dst_spec.samples = 0;
        dst_spec.callback = NULL;
        dst_spec.userdata = NULL;

        SDL_AudioCVT cvt;
        if (SDL_BuildAudioCVT(&cvt, AUDIO_U8, 1, 11025, dst_spec.format, dst_spec.channels, 44100) < 0)
        {
            I_Error("failed to build SDL audio converter: %s", SDL_GetError());
        }

        cvt.len = lengths[i];
        cvt.buf = malloc(cvt.len * cvt.len_mult * sizeof(unsigned char));
        memcpy(cvt.buf, S_sfx[i].data, cvt.len);

        if (SDL_ConvertAudio(&cvt) != 0)
        {
            I_Error("failed to convert raw PCM audio data to dest DEV format: %s", SDL_GetError());
        }

        pcm[i] = Mix_QuickLoad_RAW(cvt.buf, cvt.len_cvt);
        if (pcm[i] == NULL)
        {
            I_Error("failed to load WAV data into SDL2_mixer: %s", SDL_GetError());
        }
    }
    fprintf( stderr, " pre-cached all sound data\n");

    // Finished initialization.
    fprintf(stderr, "I_InitSound: sound module ready\n");
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
    char namebuf[9];
    sprintf(namebuf, "ds%s", sfxinfo->name);
    return W_GetNumForName(namebuf);
}

int I_StartSound(int id, int vol, int sep, int pitch, int priority)
{
    Mix_PlayChannel(-1, pcm[id], SDL_FALSE);

    return 0;
}

void I_StopSound(int handle)
{
    Mix_HaltChannel(-1);
}

int I_SoundIsPlaying(int handle)
{
    return 0;
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch)
{

}

void I_SetSfxVolume(int volume)
{
    float f_volume = ((float) volume) / 127.0f;
    f_volume *= (float) MIX_MAX_VOLUME;
    Mix_Volume(-1, (int) f_volume);
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
