rm -r build
mkdir build
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -SC:/Users/isaac/Documents/Proj/C++/York -Bc:/Users/isaac/Documents/Proj/C++/York/build -G "Unix Makefiles"
rm compile_commands.json
mv build/compile_commands.json .