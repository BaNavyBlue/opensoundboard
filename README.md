Notes on compiling:

If you're using an older version of gcc/g++ to compile

**#include <filesystem>**

will need to be

**#include <experimental/filesystem>**

for calling the filesystem if you're not specifying namespace alias's
you will have to use std::experimental::filesystem to use any of the filesystem stuff.

Besides needing OpenGL, you will need SDL2_mixer, SDL2_image, SDL2_ttf.
you will also need the freetype2 library


sudo apt install libglm-dev

sudo apt-get install -y libfreetype6-dev

sudo apt-get install -y libsdl2-mixer-dev

sudo apt-get install libsdl2-image-dev

sudo apt-get install libsdl2-ttf-dev
