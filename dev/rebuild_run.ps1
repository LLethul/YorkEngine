rm -r build
mkdir build

C:\msys64\msys2_shell.cmd -defterm -no-start -mingw64 -here -c "make -f Shaders.mk"
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -SC:/Users/isaac/Documents/Proj/C++/York -Bc:/Users/isaac/Documents/Proj/C++/York/build -G "Unix Makefiles"
cd build
make
cd ..
build/York.exe