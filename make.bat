@echo off

del RayTracer-x64.exe

cls

:: run with cmd showing
g++ main.cpp -o RayTracer-x64.exe -IC:\SFML\include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -O2 -g -Wall -DSFML_STATIC

:: run without cmd showing
:: g++ main.cpp -o out.exe -IC:\SFML\include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -O2 -mwindows -DSFML_STATIC

if exist RayTracer-x64.exe (
	RayTracer-x64.exe
)