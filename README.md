# Stanford-Bunny

## Introduction
A simple drawing of the Stanford bunny using OpenGL.

A parallel and a point light source are used.

The point light source is displayed in the window as a white cube.

## Environment
- linux/windows
- OpenGL 3.3
- GLSL 330

## How to run
In the directory, type the command below:

- make
- ./bunny

If you still have problems running the program although under the required environment ( it actually happened to me XDDD:) ), try typing the following command:

- MESA\_GL\_VERSION\_OVERRIDE=3.3 MESA\_GLSL\_VERSION\_OVERRIDE=330 ./bunny
