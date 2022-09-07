# RayTracer
	
## About the project

A simple raytracer built to fill some time over the summer break. There is nothing groundbreaking, but it works.
There are a lot of features missing and performance is not that good, but I want to add more features as time goes on and work on improving performance.

## To-do list
- Multiple light source support
- Coloured lights
- Individual sphere material properties, not global
- Sphere position animation, not just static
- Ambient occlusion
- Anti-aliasing
- Multi threaded CPU support
- Potential GPU support (unlikley)
- Windows larger than ~500,000 total pixels (weird bug)
- Support for shapes other than spheres

## Building from source

### Step 1 - Download SFML

- Install the latest version of SFML from [here](https://www.sfml-dev.org/download.php).
	- Version 2.5.1 is used in this project, the newest at the time of writing.
- Place the contents in `C:\SFML`

### Step 2 - Install MinGW

- Install the recommended version of MinGW/g++ from the download page of the version of SFML you are using.
![image](https://user-images.githubusercontent.com/39223201/188864875-3a92ec8f-da12-4eb8-a4ae-73a43a67f513.png)

### Step 3 - Compile

- Use the built in `make.bat` file or running the following command in the same directory as the source
	- `g++ main.cpp -o out.exe -IC:\SFML\include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -O2 -g -Wall -DSFML_STATIC`
	
## Sources

- Northeastern University's concise steps and formulas on making a basic ray tracing engine ([link](https://www.ccs.neu.edu/home/fell/CS4300/Lectures/Ray-TracingFormulas.pdf))
- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley
