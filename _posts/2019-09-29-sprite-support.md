---
layout: post
title: "Embedding sprites with the aid of Python"
author: Jaime Centeno
---

After finishing and adding support for my [OLED display library](https://github.com/Gecko05/SSD1351-Driver-Library) I decided to move on to the gaming section. This time I would embed sprite support into the library and a way to import sprites into the MCU flash memory, since it can be really tedious to write "by hand" all of the sprites.

![Zero](/assets/gifs/zero.gif){:class="img-responsive" width="600px" caption="A megaman X animation"}

## Using a sprite

Well, I don't quite have the perfect answer for what a sprite is, but I would define a sprite as a 2D image we use as a resource to draw each key frame of every animation and visual effect we'll see on a screen. With sprites we create an animation, our characters, environment, effects, etc. Basically everything that is visible in a 2D game. Sprites come quite handy to use when working with pixel games, it's easy to draw them, there are plenty of tools to use and they just look pretty.

![Sprite sprite](/assets/images/spritePixel.jpg)

After reading a bit about image formats I've decided to use the [.bmp format](https://en.wikipedia.org/wiki/BMP_file_format) to handle sprites, it's not an optimized format, but it makes image handling way more easy than if we used a compressed format like .png or .jpg.

## Saving up space for the sprite

Sprites and all kind of images are composed of three basic colors: red, green and blue. Image storage is a challenge, since saving up all three values of these three basic colors for each pixel of your screen could take a heavy amount of memory.

Let's say we have a sprite of 128x128 and each pixel has information about the three basic colors with a resolution of 5 bits for red and blue, and 6 bits for green; this means that each pixel would take 16 bits of memory for storage. If we calculate the total amount of memory for this single sprite, we would end up with 128x128x16 bits of memory, which is approximately 262 KB and almost a quarter of the available Flash memory on device!

![Palette](/assets/images/Palette.png){:class="img-responsive" width="400px" caption="Color palette" align="center"}

Fortunately, bitmap images have a nice method for storing pixelated images and images with a small amount of colors(up to 256). This aproach consists on making a "palette array" of all the colors used for the image, for this palette we store the real RGB bit value for each palette color. With this palette, the image is just stored as an array of indexes, where each index has a match to a color in the palette array. So, instead of taking 16 bits per pixel, we just take 8 bits, or even less if we're just using 16 colors(4 bits). This reduces the required storage per image to almost half of what we previously had!

## Creating a sprite

Now, before creating a sprite inside the SSD1351 library, we need a pixelated image as a reference. Be cautious with the size of the sprite while using this library, it's not optimized yet so you could deplete the device's memory. For future releases, a microSD will be used to store all the sprites in order to not take memory from the device.

Inside the library, a "sprite" is a structure containing a pointer to the actual sprite image as an indexed bitmap, as well as some additional information like the sprite dimensions.
For each sprite an array of unsigned 8-bit integers has to be written, which is a ton of work susceptible for a ton of human errors, right?
This is where machines come in handy and make us dependent to them. A Python script called convertsprites.py will take up all the bitmap images(created with Aseprite) passed as arguments and magically convert them to the aforementioned sprites inside .c files while incredibly reducing the amount of sweat and tears one would spend translating all of these sprites.

![Loading a sprite](/assets/images/LoadingSprites.png){:class="img-responsive" width="900px" caption="Loading a sprite"}

Once the sprites are inside the .c files, one must only compile the project using the library and use the sprites defined in loaded_sprites.c and call those beautiful sprites to be drawn on the screen with the SSD1351_draw_sprite function(It takes a sprite pointer as argument).

## What's next?

As I mentioned before, for future releases I'll include a MicroSD card to load all the sprites. Also my SSD1351 library will be used to create a simpler Lua embedded game development interface that allows to write games as if it were Pico8 and maybe someday complete an unofficial port(Although currently TIC80 seems more attractive).
As always feel free to fork and tweak this library, and if you do something with it please let me know, I would love to see it.
