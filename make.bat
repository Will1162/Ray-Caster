@echo off

del out.exe

cls

:: run with cmd showing
g++ main.cpp -o out.exe -IC:\SFML\include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -O2 -g -Wall -DSFML_STATIC

:: run without cmd showing
:: g++ main.cpp -o out.exe -IC:\SFML\include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -O2 -mwindows -DSFML_STATIC

if exist out.exe (
	out.exe
)