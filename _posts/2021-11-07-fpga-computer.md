---
layout: post
title: "Developing the Blue computer in an FPGA"
author: Jaime Centeno
---

Almost a year ago I started working on a new project to improve my understanding of computer architecture and VHDL programming. I've not always been interested in using FPGAs, unfortunately, at university I didn't even understand what the difference between a processor and an FPGA was. 

As I started learning more about low level programming and videogames, I came up with this idea of developing an emulator for the NES inside an FPGA to play on the "original" hardware withouth having to find and maintain one. However, my computer architecture and VHDL skills at the time were almost non-existent, so I decided to start with a simpler project.

After searching for projects, I came across [this article](https://hackaday.com/2016/03/16/crawl-walk-run-planning-your-first-cpu-design/) from Al Williams on [Hackaday](https://hackaday.com/). Al suggested starting with a basic computer (Blue), one that Caxton C.Foster specified in his really old book "Computer Architecture". I was unable to find this book physically at an affordable price, but eventually I found it on archive.org, what a great site! 

So I started programming, I created [this repository](https://github.com/Gecko05/BlueFPGA) and starting translater the simplest parts of the logic gates diagrams to VHDL programs. Sites like nandland.com and vhdlwhiz.com have been especially helpful in teaching me the basics of VHDL development and get me ready to write this project.

## The simple stuff

The first thing I did was read through the specification and writing down notes on how the computer really works. There are a lot of nice diagramas and tables which I copied into the repositories REAME file to keep the important stuff handy and organized. 

Some relevant information about Blue:
    - 4096 words of addressed core storage of 16 bits per word
    - Words in data storage as treated as 15-bit integers plus sign
    - Instructions are 4 bit OP code with 12 bit address (4096).
    - 16-bit Acc
    - 16-bit instruction register
    - 12-bit program counter
    - 12-bit Memory Address Register, 16-bit Memory Buffer Register and a 16-bit Z register that are not accessible to the programmer.
    - 16 different instructions for the 4-bit OPCode.
    - If the highest bit is 1, the number is negative.

As you can see Blue is a really simple computer, perfect for beginners in computer architecture. To be honest, I wasn't expecting things inside a comptuer to be as simple as they are, with a bunch of logic diagrams, Caxton C.Foster is able to describe the basic components of a complex machine.

At this point, my motivation peaked and I started to get a lot of "aha" moments, I thought everything was sorted out for me inside these logic diagrams and I was ready to jump into the code and start programming.

## The pain of initializing

Setting up the tools and stuff you need to actually do the coding can be really tedious. For this project I ordered an FPGA from numato, specifically, the MimasV2. It's comes with a nice board full of io components to test your designs and has worked really well so far.