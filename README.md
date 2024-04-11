## This is a simple opengl source code for rendering .obj file now
This project is for me to learn OpenGL and CMake. My final goal is to render object in the real world picture, like augmented reality, and render object is my first step.

## For understanding how project work
I wrote some comments in CMakeLists.txt file. They are general introductions of features of each library.

## To build the project, please do the following steps:
### Ubuntu
Install glfw, which provide implements of OpenGL interface.
```
sudo apt-get install libglfw3
sudo apt-get install libglfw3-dev
```
Build Makefile in `./build` folder by cmake command. 
```
mkdir build
cd build
cmake ../
```
Compile codes by `make` command and it will generate an executable file `opengl_project`.

```
make
```
And finally, you can run the code by run `./opengl_project` file.
```
./opengl_project
```
