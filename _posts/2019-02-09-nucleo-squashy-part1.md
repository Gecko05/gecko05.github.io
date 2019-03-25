---
layout: post
title: "PICO8-like Squashy game in a broken OLED Display"
author: Jaime Centeno
youtubeId: eNKy-4qQbHk
---


For the first entry of this page I'll talk about the process of developing a simple Squashy game for one of my favorite ST Boards, the [Nucleo L152RE](https://www.st.com/en/evaluation-tools/nucleo-l152re.html), using [FreeRTOS](https://www.freertos.org/), C language programming and [SW4STM32](https://www.st.com/en/development-tools/sw4stm32.html).

The inspiration for this project comes from the very first game example from the [PICO-8 Zine  #1](https://sectordub.itch.io/pico-8-fanzine-1) for the [PICO8](https://www.lexaloffle.com/pico-8.php) Lua based fantasy console. The first article of the zine guides you through the development of a simple Squashy game, which has pretty much the basic mechanics of a Breakout game.

![Squashy in PICO8](/assets/images/squashyPico.png){:class="img-responsive" width="400px"}

The [game](https://www.lexaloffle.com/bbs/?cat=7&carts_tab=1#sub=2&mode=carts&search=squashy) is pretty straightforward, there's a ball that bounces between the top, left and right edges of the screen, and a pad which you take control of. The sole objective is to keep bouncing the ball for as long as you can, every time the ball bounces you get +1 in your Score and if the ball touches the bottom of the screen you'll lose a life.

{% include youtubePlayer.html id=page.youtubeId %}

While my first objective was to develop a game for a Nucleo board, I intend to create something resembling a game engine for custom hardware that enables multiplayer options via Bluetooth between one or more console devices.

![Expected result](/assets/images/expected.png){:class="img-responsive" width="400px" caption="Squashy in PICO8"}

# Features

* Speed based controls
* Can restart the game
* Score
* Lives system
* Bounce beep
* Bugs

# What's missing

* Good OLED display
* PWM notes for the buzzer
* Restart position

# Hardware

* Nucleo L152RE
* 2 Pushbuttons
* OLED screen SSD1306 with I2C
* Piezo buzzer

![Hardware](/assets/images/squashyNucleo.jpg){:class="img-responsive" width="400px"}

For the hardware I'm simply using two push buttons, one piezoelectric buzzer and the cheap mono-color OLED Display SSD1306 which has a screen of 64x128 pixels. However, once I started testing stuff I noticed that my display may be broken because it doesn't show every other row of pixels. This creates some issues because small sprites of around 6x6 won't show properly and they might become illegible. Once I get my hands on a new RGB OLED Display I'll update the project to use a limited palette of 16 colors like the old times.

I didn't develop the driver for my SSD1306 from scratch, instead I cloned this simple [library](https://github.com/4ilo/ssd1306-stm32HAL) from Olivier Van den Eede which already uses STM32 HAL and contains pretty much all the abstraction I needed to build upon.

# Structure

First of all, PICO8 games need three fundamental functions; Init, Update and Draw.
Init(), as its name implies, initializes stuff.
Update() is a function that runs continuously in which you execute all calculations related to movement, hits, score, etc.
Draw() is the function that paints all the stuff on the screen.

![Pico8 basic structure](/assets/images/picoBasic.png){:class="img-responsive" width="300px"}

![Picolike tasks](/assets/images/picoInNucleo.png){:class="img-responsive" width="300px"}

The first thing I needed was a system in which I could run both Update and Draw simultaneously, since it's a more scalable approach than running one function before the other. So taking advantage of the CubeMX tool from STM32 I created a blank project with Middleware FreeRTOS enabled, which is a small Real Time Operating System that can be run on almost any 32-bit Microcontroller.

As far as I know, any RTOS can work under the concept of preemptive tasks, which are basically looped functions that run "simultaneously". For this project it means that every time fraction Draw() will do stuff then the program will swap to the Update() function and run for another time fraction, then swap back to Draw() and so on until the end of times(or until the MCU turns off) giving the illusion of running simultaneous tasks.

![Preemptive Tasks](/assets/images/preemption.jpg){:class="img-responsive" width="400px"}

Once I had the project ready I moved on to developing my three main functions. Init() was pretty straightforward, only needed to include initialization of variables, hardware, etc. While Draw() and Update() were independent tasks.

The basic drawable game element is determined by the structure *BodyNode*. *BodyNode* contains pretty much all the necessary information to know where every object should be drawn, the details of the sprite it is using and a flag to know whether the program should hide the sprite or not.

![Heart sprite in bits](/assets/images/spriteHeart.png){:class="img-responsive" width="200px"}
![Heart sprite intended](/assets/images/spriteHeartPico.png){:class="img-responsive" width="175"}

The *Sprite* type is another structure that holds an array of 16bit unsigned integers(*spriteArray*) with the monochromatic pixel information. If a pixel must be drawn then there's a 1, if not, a 0. It also holds the width and height dimensions in pixels for the sprite so the function that draws every sprite knows how to iterate over the sprite array.

Feel free to peek around, pretty much every 32bit Nucleo Board could run this game, just set up the peripherals using the STM32CubeMx Tool if you're not using a L152RE.

You can find the project's repository [here](https://github.com/Gecko05/NucleoSquashy).
