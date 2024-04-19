@echo off

@REM by default compiling from cmd, its release mode build 
@REM and when done from visual studio project its debug mode build
cl.exe /c /EHsc /I "..\\..\\..\\..\\external\\" OGL.cpp
rc.exe OGL.rc
link.exe OGL.obj OGL.res /LIBPATH:"..\\..\\..\\..\\glew\\lib\\Release\\x64\\"