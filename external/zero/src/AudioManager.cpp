#include "Zero.hpp"

    // Sets up.
    AudioManager::AudioManager() {
        if((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) != MIX_INIT_OGG) {
            // Failed to load.
            fprintf(stderr, "[W] Failed on Mix_Init for OGG format.\n");
            fprintf(stderr, "[W] Internal message: %s\n", Mix_GetError());
        }

        if((Mix_Init(MIX_INIT_FLAC) & MIX_INIT_FLAC) != MIX_INIT_FLAC) {
            // Failed to load.
            fprintf(stderr, "[W] Failed on Mix_Init for FLAC format.\n");
            fprintf(stderr, "[W] Internal message: %s\n", Mix_GetError());
        }

        if((Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) != MIX_INIT_MP3) {
            // Failed to load.
            fprintf(stderr, "[W] Failed on Mix_Init for MP3 format.\n");
            fprintf(stderr, "[W] Internal message: %s\n", Mix_GetError());
        }

        uint16_t fmt = 0;
        int freq = 0, chan = 0;
        Mix_QuerySpec(&freq, &fmt, &chan);

        if (Mix_OpenAudio(freq, fmt, chan, 1024)) {
            printf("[Mix:err] %s\n", Mix_GetError());
            exit(-7);
        }

        Mix_AllocateChannels(64); // 32 Sfx + 1 Music

        MusicStore = NULL;
        MusicCount = 0;
        SfxStore = NULL;
        SfxCount = 0;
    }

    // Cleans up everything.
    AudioManager::~AudioManager() {
        Mix_HaltChannel(-1);
        Mix_HaltMusic();

        while (SfxCount > 0) {
            Mix_FreeChunk(SfxStore[SfxCount-1]);
            --SfxCount;
        }

        while (MusicCount > 0) {
            Mix_FreeMusic(MusicStore[MusicCount-1]);
            --MusicCount;
        }

        free(SfxStore);
        free(MusicStore);

        Mix_CloseAudio();

        while(Mix_Init(0))
            Mix_Quit();
    }

    int AudioManager::LoadSfx(char* fname) {
        ++SfxCount;
        SfxStore = (Mix_Chunk**)realloc(SfxStore, sizeof(Mix_Chunk*) * SfxCount);

        memset(&SfxStore[SfxCount-1], 0, sizeof(Mix_Chunk*));

        SfxStore[SfxCount-1] = NULL;

        SfxStore[SfxCount-1] = Mix_LoadWAV(fname);

        if(!SfxStore[SfxCount-1]) {
            --SfxCount;
            SfxStore = (Mix_Chunk**)realloc(SfxStore, sizeof(Mix_Chunk*) * SfxCount);
            return -1;
        }

        return SfxCount - 1;
    }

    int AudioManager::LoadMusic(char* fname) {
        ++MusicCount;
        MusicStore = (Mix_Music**)realloc(MusicStore, sizeof(Mix_Music*) * MusicCount);

        memset(&MusicStore[MusicCount-1], 0, sizeof(Mix_Music*));

        MusicStore[MusicCount-1] = NULL;

        MusicStore[MusicCount-1] = Mix_LoadMUS(fname);

        if(!MusicStore[MusicCount-1]) {
            --MusicCount;
            MusicStore = (Mix_Music**)realloc(MusicStore, sizeof(Mix_Music*) * MusicCount);
            return -1;
        }

        return MusicCount - 1;
    }

    void AudioManager::PlaySfx(int index) {
        if(index == -1)
            return;

        if (index > SfxCount-1)
            return; // Not a valid index. No-op.

        Mix_PlayChannel(-1, SfxStore[index], 0);
    }

    void AudioManager::PlaySfx(int index, int count) {
        if(index == -1)
            return;

        if (index > SfxCount-1)
            return; // Not a valid index. No-op.

        Mix_PlayChannel(-1, SfxStore[index], count);
    }

    void AudioManager::PlayMusic(int index) {
        if(index == -1)
            return;

        // FIXME - Implement this.
        Mix_PlayMusic(MusicStore[index], 0);
    }

    void AudioManager::PlayMusicLoop(int index) {
        if(index == -1)
            return;

        // FIXME - Implement this.
        Mix_PlayMusic(MusicStore[index], -1);
    }

    void AudioManager::PauseMusic() {
        // FIXME - Implement this.
        Mix_PauseMusic();
    }

    void AudioManager::UnloadSfx(int index) {
        if(index == -1)
            return;

        Mix_FreeChunk(SfxStore[index]);
    }

    void AudioManager::UnloadMusic(int index) {
        if(index == -1)
            return;

        Mix_FreeMusic(MusicStore[index]);
    }

    // Unload all sfx
    void AudioManager::FlushSfx() {
        Mix_HaltChannel(-1);

        while (SfxCount > 0) {
            Mix_FreeChunk(SfxStore[SfxCount-1]);
            --SfxCount;
        }

        free(SfxStore);
        SfxStore = NULL;
    }

    // Unload all music.
    void AudioManager::FlushMusic() {
        Mix_HaltMusic();

        while (MusicCount > 0) {
            Mix_FreeMusic(MusicStore[MusicCount-1]);
            --MusicCount;
        }

        free(MusicStore);
        MusicStore = NULL;
    }
