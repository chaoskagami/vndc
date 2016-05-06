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

This is a clone of vnds. Actually, it's an enhanced (objectively) version. Why? A better interpreter without artificial limits. It could be potentially possible to use this as a base for a commercial VN if you feel like changing things a bit. This is why I used the MIT license; I'm a free-use guy, not copyleft. ;)

This came about after converting Fate/Stay night to vnds format. I started poking through the scripts' format. My first thought was 'this format is simple'. So I went and coded a VNDS interpreter. I named it VNDC, and made the name recursive for lulz.

Currently, I can say confidently that it runs on Linux and Windows (with proper compilers - read: mingw, I hate VC and will make no effort to support it). It will probably also work on *bsd, if you can get SDL2, SDL_mixer, SDL_image, and SDL_ttf. I'll test it on mac using my MacBook '03 sometime soon. I have not tried building with clang. Lemme know how that goes.

The ultimate goal is for this to run on anything that supports SDL2, Android, and 3DS' with CFW. Note that I'll not do iOS or Mac myself - I don't have any access to a compiler for any modern version of OSX and I can't exactly build an interpreter unless I'm targeting Jailbreak-only, anyways. But given a proper macports tree, you shouldn't have a terribly difficult time building it against SDL2.
