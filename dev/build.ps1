C:\msys64\msys2_shell.cmd -defterm -no-start -mingw64 -here -c "make -f Shaders.mk"
cd build
make
cd ..
rm York.exe
mv build/York.exe .