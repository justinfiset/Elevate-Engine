cd ../
call vendor\premake\premake5.exe ninja --os=emscripten
ninja Editor_Debug

pause