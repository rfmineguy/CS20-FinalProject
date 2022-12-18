# CS20 Final Project
## Directions

You will be submitting your project to me via GitHub. Your repository needs to include a *README* file (that's me!) that explains how to compile and run your program. Your report as a PDF document must also be included in your repository. This submission needs to happen by the end of the final exam period for our course. For us, that occurs on **Monday (12/19) @ 11:20am**.

This *README* file is written in [Markdown](https://en.wikipedia.org/wiki/Markdown). If you're unfamiliar with this notation, [check out this guide](https://itsfoss.com/markdown-guide/).

Make sure to adhere to the full requirements as stated on [Canvas](https://clpccd.instructure.com/courses/30891/pages/final-project).

## Description
This repository host my final project for the CS20 class. It is a terminal based game that relies on the amazing terminal graphics library `FTXUI`. <br>
The game is card based, and you are essentially in a turn based two player battle. There are a few card types that perform different actions. <br> 
For example the Sword attack is a simple damage attack, the poison is another damage attack, and the health card is for healing yourself. <br>


## Build Instructions
<b> Multi liner </b>
```bash
mkdir build
cd build
cmake ..
make
```

<b> One liner </b>
```bash
mkdir build && cd build && cmake .. && make
```

## Running
```bash
cd build && ./card_battle
```

## Notes
- Tested in the cmorenolpc/lpc-cs20 docker image [SUCCESSFULLY]
