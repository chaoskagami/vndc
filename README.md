vndc - VNDC is Not a Direct Clone (of VNDS ;P)
====

tl;dr

Install SDL2, SDL2_mixer, SDL_image, SDL_ttf if running a unix. Run ./build in the source dir for targets. Choose the appropriate one.

You should get a binary named vndc.something in bin. Run vndc -h if you don't know how to run it. Have VNDS files ready. This includes foreground, sound, background, script, and default.ttf

Also important: Unzip zips first.

-----

A picture says a thousand words. Have one.

![Screenie of Current State](https://raw.github.com/chaoskagami/vndc/data/screenshot.png)

This is a clone of vnds. Actually, it's an enhanced (in my opinion) version.

After converting Fate/Stay night to vnds format, I started poking through the scripts' format. My first thought was 'this format is simple'.

So I went and coded a VNDS interpreter. I named it VNDC, and made the name recursive for lulz.

Currently, I can say confidently that it runs on Linux. It will probably also work on *bsd, if you can get SDL2, SDL_mixer, SDL_image, and SDL_ttf. I'm going to tweak stuff in the next release to build on windows, and I'll test it on mac using my MacBook '03 in the one after that. I have not tried building with clang. Lemme know how that goes.

The ultimate goal is for this to run on Linux, Windows, Mac, Android, and (jailbroken) iOS. Note that I'll never make a binary for iOS unjailbroken because A) apple and B) this is an interpreter.
