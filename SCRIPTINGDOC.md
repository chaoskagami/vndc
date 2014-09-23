VNDC Script format documentation
=====

Functions:
 1.  bgload
 2.  choice
 3.  cleartext
 4.  delay
 5.  if and fi
 6.  goto and label
 7.  jump
 8.  music
 9.  random
 10. setimg
 11. setvar and gsetvar
 12. sound
 13. text
 14. save
 15. debug commands
    1. help
    2. resume
    3. quit
    4. debugsave
 16. Save format

BGLOAD
-----

bgload sets a background image.

bgload takes two arguments, one of which is optional.

	bgload bg [fadeout]

The parameter 'bg' should be a file in backgrounds.

The second parameter, 'fadeout', should be a length in frames to fade into the new background.

The game always operates at 60fps.

CHOICE
-----

choice presents choices to the user on screen.

`
	choice ch1|ch2|ch3...
`

Each choice is separated by a '|' (pipe) character. The result of the choice is stored into a variable named 'selected', which can be queried with if.

Optionally, I implement an extension of this form:
`
	choice ch1|ch2|... >var
`
Instead of storing to selected, 'var' will be used instead.

CLEARTEXT
-----

cleartext clears all of the text on-screen.

	cleartext [mod]

A modifier can be specified with mod. This is supposed to be fadeout or something.

Strangely, this command does not actually appear in the conversion of fate-stay night.

Also, I ignore mod. It seems to be useless...maybe.

DELAY
-----

delay will sit around and wait a certain number of frames.

	delay frames

'frames' is the number of frames to delay. Again, we operate at 60fps.

IF AND FI (Control Flow)
-----

Technically, two commands. if and fi are used for execution control. if the result of if is true, all statements are executed until a matching fi.

	if var op val

'var' is a variable name. This can be a setvar, choice etc.

op should be one of the following:

	<	- less than
	<=	- less than or equal
	==	- equal
	!=	- not equal
	>=	- more than or equal
	>	- more than

I have no clue how many of these VNDS implements, but I do them all.

-----

fi simply ends an if statement.

`fi`

Note that I am a bit smarter than vnds here. `if` and `fi` CAN be embedded. So this is valid:

`
	if selected == 2
		if previous >= 5
			text 'You win!'
		fi
		if previous <= 5
			text 'You lose!'
		fi
	fi
`

The behavior of VNDS would be to `'if previous <= 5'` on failure of `selected == 2`. This is really not proper: vnds just conditional jumps. Frankly, a jumpeq would be far more effective. I keep an if-count instead of an in-if bool value, so there's proper control flow.

GOTO
-----

goto finds a label and continues execution from there.

	goto label

In my implementations, label is actually completely non-existent as a script command. Instead, goto finds labels by re-seeking through the file.

Goto cannot jump to another file. That's not how it works. Use jump instead.

JUMP
-----

jump transfers control to another script file:

	jump script

My implementation provides an extra form of jump:

	jump script [line]

Which goes directly to line in the file. This is utilized for saving. As such, you can't turn off that extension. Period.

MUSIC
-----

music loads a audio file from sound, and plays it in a loop as music.

	music file

You can pass '~' as file to stop playing all music like so:

	music ~

RANDOM
-----

random saves a random number to a variable.

	random var low high

low and high are the range the random number is in, inclusive.

SETIMG
-----

setimg displays an image from foreground at a position specified.

	setimage file x y

G/SETVAR (variables)
-----

setvar and gsetvar are used to set variables. gsetvar specifically sets the 'global' state, that is, common to all saves. In my implementation - that is moot. Thus, gsetvar just calls setvar.

`
	[g]setvar var mod value
`

mod can be one of the following:

	`=`	Set var to value
	`+`	Add value to var
	`-`	Subtract value from var

Also, if value is not specified, you can also do this as the mod:

	`~`	Reset to 0.

One more thing that was not all over the place in vnds, but typically at the beginning.

`
	setvar ~ ~
`

Which means reset all vars. This ignores globals - e.g. prefixed with a lowercase `g`.

As well, my extension provides the ability to set a variable to another variable.

`
	[g]setvar var op var2
`

Again, this only works in extensions mode.

SOUND
-----

sound plays a sound, and optionally a number of times.

`
	sound file [times]
`

`times` is implicitly 1 if not specified. Pass -1 for infinity.

You can do

`
	sound ~
`

To stop all playing sounds.

TEXT
-----

text is the meaty huge function. It outputs text, as you probably expect.

`
	text This is text dee doo...
`

There are a bunch of variants with special meaning. I don't know why, but there are:

	`text`			- (no params) Clear screen
	`text !`		- Wait for input and clear all.
	`text ~`		- Clears all text (like cleartext)
	`text @...`		- Don't wait for click after text is output.

I've implemented a few extensions that help generally
with NVL games.

	`---`				- At the beginning of a line,
					  clears before text output.
	`"`				- In the sound function
					  if lookahead finds a text
					  with quotes, it's assumed
					  that sound is a voice and
					  linked to the text. AKA
					  smart voice stopping.

DEBUG
-----

I've implemented a few debug commands to help out and just generally be useful (cheats? :P)

Make sure you start up with the -b parameter, and start it from a console. Send it a SIGINT (^C).

The console should now prompt you with this;

`
	[scr command] $
`

You can enter any command above; Some won't work right. choice probably won't work properly, because mouse is disabled.

No commands run in this state will advance the program, but flags set will affect it.

HELP
-----

help will print everything available.

RESUME
-----

resume (renamed from stop) will disable debugging and kill the console. You can SIGINT again at any point to come back to the debug state.

QUIT
-----

quit sets the quit state - it's equivalent to ESC in the form of a console command.

DEBUGSAVE
-----

debugsave will save to a mandatory file specified;

	save file

See section 15 for info on what the a save actually is.

This function bypasses all checks. It just dumps it. You can write a save anywhere if you have write perms.

SAVE FORMAT
-----

Saves are actually a script generated by the save function. So, for example, this is a save:

`
	setvar selected = 2
	music bgm03.ogg
	bgload qmrtb.jpg
	jump fate01-00.scr 261
`

It restores any variables needed, loads the current music and bg, and jumps to the location we were previously at in the script.

Saves should be loaded with the -s parameter until I get better gui stuff in place.
