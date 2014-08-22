#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP


	class AudioManager {
		public:
			// Ctors and dtors
			AudioManager();
			~AudioManager();

			// Load index.
			int LoadSfx(char* fname);
			int LoadMusic(char* fname);

			// Playing functions.
			void PlaySfx(int index);
			void PlaySfx(int index, int count);
			void PlayMusic(int index);
			void PlayMusicLoop(int index);
			void PauseMusic();

			// Unload Functions
			void UnloadSfx(int index);
			void UnloadMusic(int index);

			// Complete unload functions
			void FlushSfx();
			void FlushMusic();
		private:
			int SfxCount = 0, MusicCount = 0;
			Mix_Chunk** SfxStore = NULL;
			Mix_Music** MusicStore = NULL;
	};


#endif
