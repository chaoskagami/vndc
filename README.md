vndc - VNDC is Not a Direct Clone (of VNDS ;P)
====

tl;dr - To install, do the standard autotools dance, e.g. `./autogen.sh && ./configure && make -j4 && make install`

This is under heavy changes at the moment after about 2 years of general deadness. In general, I'm surprised the code aged as well as it did. Going to to get it working on 3DS and Android hopefully.

-----

For generic platforms you need: SDL2, SDL2_mixer, SDL_image, and SDL_ttf.

You should get a binary named vndc.something in bin. Run vndc -h if you don't know how to run it. Have VNDS files ready. This includes foreground, sound, background, script, and default.ttf

Also important: Unzip zips first. VNDC can't read zipped vnds data files.

-----

A picture says a thousand words. Have one.

![Screenie of Current State](https://raw.githubusercontent.com/chaoskagami/vndc/data/screenshot.png)

(While this screenie is two years old, there's no practical difference at the moment.)

This is a clone of vnds. Actually, it's an enhanced (objectively) version. Why? For one, the licensing (MIT). Other reasons include: proper `if` statements, extended `cat`-like variable storing syntax, seeking from disk rather than memory, etc etc. Need I go on?

Currently, I can say confidently that it runs on Linux and Windows (with proper compilers - read: mingw, I hate VC and will make no effort to support it). It will probably also work on *bsd, if you can get SDL2, SDL_mixer, SDL_image, and SDL_ttf. I'll test it on mac using my MacBook '03 sometime soon. I have not tried building with clang. Lemme know how that goes.

The ultimate goal is for this to run on anything that supports SDL2, Android, and 3DS' with CFW. And inevitably, to replace the paid VNDS from android market which still has control flow problems and people swear by. Note that I'll not do iOS or Mac version myself - I don't have any access to a compiler for any modern version of OSX and I can't exactly build an interpreter unless I'm targeting Jailbreak-only, anyways. But given a proper macports tree, you shouldn't have a terribly difficult time building it against SDL2.
