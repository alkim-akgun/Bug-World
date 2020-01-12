# Bug World
Bug World simulation game. Join the [tournament](tour)! See the [leaderboard](tour/leaderboard.txt).

## How to Play

Write your own buggy assembly code and generate its machine description using the [assembler](asm) or just write the machine description. Then create your own map or use the [random map generator](rndmap) to generate one for you. Test them using the [simulator](sim) and participate in the [tournament](tour).

## Project

### Introduction
[asm](asm) is the assembler that takes buggy assembly code and emits bug machine description. (It depends on [types](types)).

[sim](sim) is the simulator that takes a world map and two machine descriptions and simulates a game. (It depends on [game](game)).

[tour](tour) is the tournament that takes multiple maps and multiple machine descriptions and simulates a tournament. (It depends on [game](game)).

([game](game) depends on [types](types)).

To get familiar, simply,

build with `make`, `make example` demonstrates an example with provided sample data ([sim](sim) has more `example_` options for `make`; check out its [`Makefile`](sim/Makefile)). Inspect the `Makefile`s in each folder. Use `--help` argument to find out how to use `asm`/`sim`/`tour`.

## Game

### Introduction

Two nations (Red and Black) of bugs compete to collect the greater amount of food on a 2-D map.

### Rules

1. Map is hexagonal (even though its representation in `.world` files and map reports does not look so) and consists of cells.
2. The game starts with one bug for each home cell in the map, with various obstacles and food spread around the map.
3. A cell may be obstructed by an obstacle (a rock). If it is not, a bug can move to that cell occupying it, if it is not already occupied by another bug.
4. Each bug executes 1 instruction per cycle. After a `move` instruction, a bug must rest for 14 cycles.
5. If a bug is surrounded by 5 members of the opposing nation, it gets killed and turns to 3 units of food.
6. A bug can mark/unmark a cell with/of one of its 6 different nation-specific markers.
7. A bug can sense its close neighbourhood.
8. A bug can carry 1 unit of food.
9. A cell can contain multiple units of food.
10. The winner is the side with the greater amount of food on their home cells at the end.

Rules are more easily understood by writing buggy assembly or bug machine description. Read below to find out.


### Buggy Assembly Language
Buggy code is written to a file with `.buggy` extension. `;` comments what comes after in that line. The language rules are as follows.

`[label]:`; label this location in code (don't forget the colon (`:`) at the end).

`sense [sense_direction] [condition] else [label]`; sense this direction, if the condition fails, go to label.

`mark [marker]`; mark this cell with marker.

`unmark [marker]`; erase the marker in this cell.

`pickup else [label]`; pick up food, if can't, go to label.

`drop`; drop food (if not carrying food, nothing happens).

`turn [LR]`; turn left or right.

`move else [label]`; move forward, if can't, go to label.

`flip [u_int] else [label]`; generate a random number from 0 to u_int (exclusive), if the number is not zero, go to label.

Each of these rules is one line in the code. Following are the argument options.

`[label]` is any one word (no spaces).

`[sense_direction]` is one of the following: `here`, `ahead`, `leftahead`, `rightahead`.

`[condition]` is one of the following: `friend`, `foe`, `friendwithfood`, `foewithfood`, `food`, `rock`, `marker[i]`, `foemarker`, `home`, `foehome`. `[i]` in `marker[i]` is a number from 1 to 6 (inclusive).

`LR` is one of the following: `left`, `right`.

`u_int` is an unsigned integer.

The flow of instructions is top-down, except the `goto [label]` instruction will take it to `[label]`. See an example [here](asm/data/example/example.buggy); execute `make example` in [asm](asm) to see the machine description it generates.

### Bug Machine Description

Bug machine description is written in a file with `.bug` extension. `;` comments what comes after in that line. Most of the instructions have the same semantics as in the buggy assembly language. The description rules are as follows.

`sense [sense_direction] [state] [state] [condition]` (First `[state]` is the (success) state to enter if senses `[condition]`, the second (fail) `[state]` is the one otherwise).

`mark [marker] [state]`

`unmark [marker] [state]`

`pickup [state] [state]` (First `[state]` is the (success) state to enter if picks up food, the second (fail) `[state]` is the one otherwise).

`drop [state]`

`turn [LR] [state]`

`move [state] [state] ` (First `[state]` is the (success) state to enter if moves forward, the second (fail) `[state]` is the one otherwise).

`flip [u_int] [state] [state]` (First `[state]` is the (success) state to enter if the generated random number is 0, the second (fail) `[state]` is the one otherwise).

`direction [direction] [state] [state]` (First `[state]` is the (success) state to enter if facing direction `[direction]`, the second (fail) `[state]` is the one otherwise).

Each of these instructions is one line in the code. Following are the argument options.

`[sense_direction]` is one of the following: `here`, `ahead`, `leftahead`, `rightahead`.

`[state]` is an unsigned integer. At state *i*, *ith* instruction in the `.bug` file is executed, starting from 0.

`[marker]` is an integer from 1 to 6 (inclusive). Note that it is only the number, not prefixed with `marker` as in the assembly language.

`[LR]` is one of the following: `left`, `right`.

`[u_int]` is an unsigned integer.

`[direction]` is an integer from 1 to 6 (inclusive). The numbers indicate east, south-east, south-west, west, north-west and north-east in this order.

A bug, at each state, will execute the instruction corresponding to that state, and each instruction sets the next state for that bug. See an example of machine description [here](sim/data/example/example1.bug) or [here](sim/data/example/example2.bug) or anywhere in [here](tour/data/bugs). For instructions with two `[state]` arguments (`sense`, `pickup`, `move`, `flip`, `direction`), the first one is the success state and the second one is the fail state always.

### World

A world file has an extension `.world`. A world with width `W` and height `H` follows this format:

1. First line is `W`
2. Second line is `H`
3. `H` lines representing the map follow, each with `W` characters.
3.1. Each character is one of the following: `.`, `#`, `+`, `-`, `[1-9]`; `[1-9]` is any digit from 1 to 9 (inclusive).
     - `.` denotes an unobstructed cell.
     - `#` denotes an obstructed cell.
     -`+` denotes a red bug home.
     - `-` denotes a black bug home.
     - `[1-9]` denotes the number of units of food in that cell.

## Acknowledgement

Based on the project assignment of Spring 2018 Software Engineering course at Jacobs University Bremen instructed by Prof. Dr. Peter Baumann, which is based on the programming contest of the 7th International Conference on Functional Programming (ICFP), organized by the Programming Languages Research group at the University of Pennsylvania.
