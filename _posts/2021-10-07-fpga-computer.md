---
layout: post
title: "Developing a computer inside an FPGA"
author: Jaime Centeno
---

Almost a year ago(on and off) I started working on a new project to improve my understanding of computer architecture and VHDL programming. I've not always been interested in using FPGAs, unfortunately, at university I didn't even understand what the difference between a processor and an FPGA was. 

As I started learning more about low level programming and videogames, I came up with this idea of developing an emulator for the NES inside an FPGA to play on the "original" hardware withouth having to find and maintain one. However, my computer architecture and VHDL skills at the time were almost non-existent, so I decided to start with a simpler project.

After searching for projects, I came across [this article](https://hackaday.com/2016/03/16/crawl-walk-run-planning-your-first-cpu-design/) from Al Williams on [Hackaday](https://hackaday.com/). Al suggested starting with a basic computer (Blue), one that Caxton C.Foster specified in his really old book "Computer Architecture". I was unable to find this book physically at an affordable price, but eventually I found it on archive.org, what a great site! 

So I started programming, I created [this repository](https://github.com/Gecko05/BlueFPGA) and starting translater the simplest parts of the logic gates diagrams to VHDL programs. Sites like nandland.com and vhdlwhiz.com have been especially helpful in teaching me the basics of VHDL development and get me ready to write this project.

## The simple stuff

The first thing I did was read through the specification and writing down notes on how the computer really works. There are a lot of nice diagramas and tables which I copied into the repositories REAME file to keep the important stuff handy and organized. 

Some relevant information about Blue:
* 4096 words of addressed core storage of 16 bits per word.
* Words in data storage as treated as 15-bit integers plus sign.
* Instructions are 4 bit OP code with 12 bit address (4096).
* 16-bit Acc.
* 16-bit instruction register.
* 12-bit program counter.
* 12-bit Memory Address Register, 16-bit Memory Buffer Register and a 16-bit Z register that are not accessible to the programmer.
* 16 different instructions for the 4-bit OPCode.
* If the highest bit is 1, the number is negative.

As you can see Blue is a really simple computer, perfect for beginners in computer architecture. To be honest, I wasn't expecting things inside a computer to be as simple as they are, with a bunch of logic diagrams, we're able to build the basic components of a computer.

At this point, my motivation peaked and I started to get a lot of "aha" moments, I thought everything was sorted out for me inside these logic diagrams and I was ready to jump into the code and start programming.

## The pain of initialization

Setting up the tools and stuff you need to actually do the coding can be really tedious. For this project I ordered an FPGA from Numato, specifically, the MimasV2. It comes with a nice board full of IO components to test your designs and has worked really well so far. The difficult thing was setting up the software I needed to write the code, flash the FPGA and sorting out how things worked overall. It had been a while since I last used Xilinx ISE, and I had never used it before on a Linux machine. 

Once I got ISE ready to go, I tried to figure out how to actually flash the FPGA with the binary file that comes out of the projects. At first I tried to run a Python utility "provided" by Numato to flash their devices, but I just couldn't get it running. The best tool to flash the FPGA runs only on Windows, so after giving up on the Linux tool I decided to try and run the Windows tool using Wine. Once  I figured out how the COM ports behave on Wine, I was able to use it without any problems.

After doing a few beginner level FPGA projects, I felt ready to jump into programming the clock module for Blue.

## Translations

While I was programming the clock module, I started to notice things wouldn't be as straightforward as copying all the logic gate diagrams to VHDL statements, or at least, that wouldn't be the easiest way to do it. I wanted to have a middleground between copying exactly the diagrams I found and simplifying things to better fit with VHDL statements. From what I can gather, the clock module of Blue is a bit unconventional, even the author suggests doing things in a different way. The important stuff is that a cycle in Blue is made of 8 clock steps, there's a signal for each step which will be used to define the process of performing each instruction. More diagrams describe what needs to be done on each step for every instruction in Blue.

The clock for Blue in VHDL is basically a process loop that at a set minor frequency(50/8 MHz) rotates a bit on the clock steps signal. For debugging purposes I added a pulse signal that matches the major frequency(50 MHz).

I wanted to start with the simpler instructions for this project, so the first step was coding all the necessary stuff to get the NOP, HALT, and JUMP instructions ready. 

As soon as I finished programming registers like the Memory Buffer Register, Memory Address Register and Program Counter, I began programming what I call the Control Unit, although I'm not entirely sure if this is the right term. The Control Unit would be the part that orchestrates the interactions between all the registers, instructions and power options. Currently in my code this part of Blue is a mess, there are too many lines of code for my test and I'm sure I can isolate things a little better, especially the instructions decoding part. 

When I began programming the CU, I noticed I wouldn't be able to actually program a Bus that connects between several registers as described in the book, at least not with my current skill level. I searched around the web on how to program such thing, but a lot of people seemed to advice towards using multiplexing instead of actually programming buses, so I went with this approach. 

## Not complete yet

After a lot of struggles and almost 45 hours of free time later, I had this basic "computer" that could interpret NOP, HALT, and JUMP instructions! It wasn't much, but it's honest work. 

Currently, my Blue implementation is able to process ADD instructions, I started coding the ALU and two different states for double cycle instructions like ADD. I've learned a lot about VHDL along the way and I wish to finish coding Blue, however, there are some challenges I need to figure out. As I start adding more instructions, debugging becomes a more complicated process, those signal diagrams from the simulations are a bit confusing and I wonder what other ways exist to debug the whole thing. 

In order to ease things in the future, I plan on adding a debugging feature to dump the content of the RAM and each individual register via serial communication, and then arrange that data in some sort of user interface. This should help me debug programs and the actual behavior of the computer. I'm also working on a C++ software emulator, which I'll describe in a later post, which is surprisingly way easier to implement than the actual VHDL thing.

Feel free to use the project's repository as you wish, this is a learning exercise I really recommend doing to anyone interested in basic computer architecture and FPGAs. I'm still far away from my actual goal of coding a hardware emulator for the NES, but it's been a really fun project to do so far and hopefully I'll be able to finish it soon. Also I started taking this course by the LinuxFoundationX that teaches how to program a RISC V processor, haven't finished it yet, but it seems to be a good way to start with computer architecture design https://www.edx.org/course/building-a-risc-v-cpu-core