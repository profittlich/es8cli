#!/bin/bash

M="--midi-in 1 --midi-out 1"

# Bank 1

./es8cli $M select 800 name "Clean" store 1 display
./es8cli $M select 800 name "Fuzz" loops 24V store 2 display
./es8cli $M select 800 name "Drive" loops 34V store 3 display
./es8cli $M select 800 name "Crunch" patchmidipc 1 2 patchmidicc 2 2 7 90 store 4 display
./es8cli $M select 800 name "HiGain" patchmidipc 1 3 store 5 display
./es8cli $M select 800 name "Clean Flange" loops 46V store 6 display
./es8cli $M select 800 name "Clean Chorus" patchmidichannel 3 4 patchmidicc 3 1 20 127 store 7 display
./es8cli $M select 800 name "HiGain Flange" patchmidipc 1 3 loops 46V store 8 display

# Bank 2

./es8cli $M select 800 name "Clean" store 9 display
./es8cli $M select 800 name "Fuzz" loops 24V store 10 display
./es8cli $M select 800 name "Drive" loops 34V store 11 display
./es8cli $M select 800 name "Pushed Crunch" loops 34V patchmidipc 1 2 patchmidicc 2 2 7 90 store 12 display
./es8cli $M select 800 name "Pushed HiGain" loops 34V patchmidipc 1 3 store 13 display
./es8cli $M select 800 name "Clean HX1" loops 47V store 14 display
./es8cli $M select 800 name "Clean HX2" loops 48V store 15 display
./es8cli $M select 800 name "HiGain HX2" patchmidipc 1 3 loops 48V store 16 display

# Notes

# 1: === Select 800 ===
# Name: TEMPLATE        
# Loops: ---4----V
# Input: IN_1
# Output: OUT_1
# MIDI: 1:(CH: 5 PC: 1 CC1: 85 127) 2:(CH: 4 PC: 1 CC1: 69 0 CC2: 7 107) 
#               Amp Ch.    FX Loop         HX Preset  Snapshot   Volume

# Loops
# Loop V: Always on
# Loop 1: Comp
# Loop 7: HX 1
# Loop 2: Fuzz
# Loop 3: OD
# Loop 8: HX 2
# Loop 4: Amp 1
# Loop 5: Amp 2
# Loop 6: Mod
# Output: HX 3

# 5150 MIDI
# Green CH 5 PC 1
# Blue CH 5 PC 2
# Red CH 5 PC 3
# Loop CH 5 CC 85

# HX MIDI
# Snapshot CH 4 CC 69
# Output volume CH 4 CC 7

# Chorus Bypass CH 4 CC 20
# Flanger Bypass CH 4 CC 20
# Phaser Bypass CH 4 CC 20
# Vibrato Bypass CH 4 CC 20
# Tremolo Bypass CH 4 CC 20
# Vibe Bypass CH 4 CC 20
# Modulation 2 Bypass CH 4 CC 20

# Delay Bypass CH 4 CC 20
# Delay 2 Bypass CH 4 CC 20

# Fuzz Bypass CH 4 CC 20
# Fuzz 2 Bypass CH 4 CC 20
# Drive Bypass CH 4 CC 20
# Drive 2 Bypass CH 4 CC 20

# Pitch Bypass CH 4 CC 20
# Pitch 2 Bypass CH 4 CC 20
# Synth Bypass CH 4 CC 20
# Synth 2 Bypass CH 4 CC 20

# Compressor Bypass CH 4 CC 20

# EQ Bypass CH 4 CC 20
# Acoustic Sim Bypass CH 4 CC 20
# Filter Bypass CH 4 CC 20
# Wah Bypass CH 4 CC 20

# Reverb Bypass CH 4 CC 20

