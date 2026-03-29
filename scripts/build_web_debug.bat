cd ../
call vendor\premake\premake5.exe ninja --os=emscripten
ninja Debug

pause