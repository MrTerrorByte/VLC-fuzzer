# VLC-fuzzer

This is a small program that uses radamsa to fuzz video files and then load them in vlc player to try to make it crash. If it crashes, we can then use a debugger on vlc playing that particular file to see where it crashes and if there is an exploitable bug.

I wrote this as a short, fun project when I was in SIGPony at UIUC.
