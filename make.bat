@echo off

set showDebugCmd=1

cd build
del RayTracer-x64.exe

cls

echo Building...

if %showDebugCmd%==0 (
	g++ -std=c++17 -O2 ^
	../src/main.cpp ^
	../src/camera.cpp ^
	../src/colour.cpp ^
	../src/light.cpp ^
	../src/material.cpp ^
	../src/sphere.cpp ^
	../src/utility.cpp ^
	../src/vec3.cpp ^
	-o RayTracer-x64.exe ^
	-IC:\SFML\include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -DSFML_STATIC ^
	-mwindows
)

if %showDebugCmd%==1 (
	g++ -std=c++17 -O2 ^
	../src/main.cpp ^
	../src/camera.cpp ^
	../src/colour.cpp ^
	../src/light.cpp ^
	../src/material.cpp ^
	../src/sphere.cpp ^
	../src/utility.cpp ^
	../src/vec3.cpp ^
	-o RayTracer-x64.exe ^
	-IC:\SFML\include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -DSFML_STATIC ^
	-g -Wall
)

echo Done
echo.

if exist RayTracer-x64.exe (
	RayTracer-x64.exe
)

cd ../