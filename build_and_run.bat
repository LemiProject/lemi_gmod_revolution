@echo off
msbuild ./lemi_gmod_revolution.sln /p:Platform="x86" /p:Configuration="Release"
start steam://rungameid/4000