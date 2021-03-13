# ES8cli

Configure your Boss ES-8 loop switcher from the command line. Read, change and write patches.

This is not a replacement for the official ES-8 Edit software from Boss, which provides a graphical user interface for more intuitive editing. ES8cli has advantages for bulk editing and can be used in scripts.

## License 
Licensed under a MIT-style license. See LICENSE for further information.

## Compatibility

es8cli runs on macOSX Catalina and very likely on other macOS versions.
Windows and Linux are currently not supported. Future support is a possibility.

es8cli has been tested with firmware version 2.05. Other 2.xx versions will likely also work. Firmware versions	1.xx will not work.

Keep in mind the disclaimers in the license. If using es8cli does not work for you, damages your computer, ES-8 or any other device, burns down the house, kills someone, sets off the end of the world, cancels your favorite TV show or causes any other problems, it's on you. Use at your own risk.

## Usage
```
usage: es8cli [--help|--list-midi|<options> <commands>]

  --help:         Show this help text
  --list-midi:    List MIDI input and output devices

Options:

  --verbose:      Output more information

  --midi-in <N>:  Index of the MIDI input port to use
  --midi-out <N>: Index of the MIDI output port to use

Commands:

  select [patch|filename]:                          Select a patch from the ES-8 or a file
  copy [patch|filename] [patch|filename]:           Copy a patch from one location to another (the ES-8 or a file)
  store [patch|filename]:                           Store the currently used patch to the ES-8 or a file
  view [patch|filename]:                            View information about a patch from the ES-8 or a file
  display:                                          View information about the currently used patch
  name [patch name]:                                Set the name of the patch (32 characters max.)
  loops [-|12345678V]:                              Set loops on/off ('-' for no loops on, '34V' for loops 3, 4 and V on)
  input [1|2]:                                      Set input to use
  output [1|2|BOTH]:                                Set output(s) to use
  patchmidichannel [index] [channel|OFF]:           Set the MIDI channel for a patch MIDI setting
  patchmidipc [index] [PC|OFF]:                     Set the program change for a patch MIDI setting
  patchmidicc [index] [Ctl-index] [CC|OFF] [value]: Set a CC for a patch MIDI setting 

Examples:

  Show information about patch 44:
    view 44

  Change the name of patch 44 on the ES-8:
    select 44 name "New name" store 44

  Backup a patch:
    copy 44 mybackup.es8

  Restore a patch:
    copy mybackup.es8 44 

  Duplicate a patch on the ES-8:
    copy 44 45

  Change the input of a patch:
    select 44 input 2 store 44

  Load a patch from a file and store it to two different patches with differnt loops activated:
    select mybackup.es8 loops 146 store 44 loops 148V store 45

  Send a program change and CC on MIDI channel 5 when activating a patch:
    select 44 patchmidichannel 1 5 patchmidipc 1 23 patchmidicc 1 1 85 127 store 44

  Also send a program change and CC on MIDI channel 7 when activating the patch:
    select 44 patchmidichannel 2 7 patchmidipc 2 3 patchmidicc 2 1 11 66 store 44

  Do not send a CC:
    select 44 patchmidicc 1 1 OFF 0 store 44

  Do not use patch MIDI block 1 at all:
    select 44 patchmidichannel 1 OFF store 44
```
