# StormRaiser's Universal Permuting Machine

Version: alpha 0.2

## What is this
In a word, this is a multifunctional digital Rubik's Cube, a program that is
capable of simulating a broad range of twisty puzzles.

The goal of this project is to find a systematic approach by which these puzzles could be modeled, to provide friendly user interaction to play with them, and ultimately, to automatically discover a set of algorithms to solve them.

There have been a lot of Rubik's Cube programs around, so why do we need another? Yes, we have simulators, solvers and timers, but they mostly focus on the speed solving aspect of these puzzles. Being too lazy to memorize those hundreds of algorithms, I am not a speedcubing player. I prefer to exercise my mind rather than my hand, so I collect twisty puzzles with different shape, structure and mechanism, and try to solve them when there are no solutions readily available from the Internet.

Here comes the problem. Many of them are expensive, some clever designs are not available for sale, and you just can't carry your whole collection around with you. So it would be good if we could have a simulator with which you could play all those puzzles at no price.

And after some search, I found [Virtual Magic Polyhedra](http://users.skynet.be/moz071262/Applets/Magic%20Polyhedra/). They have a really huge collection of puzzles there, including pretty much everything I have seen, and it seems that many twisty puzzle enthusiasts are using it. But it lacked the programmability that I am seeking. If you are a designer, it is desirable to test your design before putting it in to production. Or you may just want to implement a unique virtual puzzle. So this is the primary goal of this project: provide a syntax in which the large variety of twisty puzzle could be described concisely.

Actually what I wish to have is way more. I would like my program to be able to solve any puzzle you give it. And by solve, I don't mean taking a initial scrambled state and searching for a sequence of move that could restore the puzzle to the solved state. I mean giving a handful of algorithms with which a human player could solve any possible state, and proving that the other states are not possible. Well, this is just a wish. But there must be a possibility to do that with AI and some careful application of group theory.

## How to build
This program uses Qt for GUI, [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) for geometry related calculations, flex/bison for the parser of the "cube description language" developed for this project, and [trimesh2](http://gfx.cs.princeton.edu/proj/trimesh2/) for handling mesh file input.

The part of trimesh2 code used in this project has been included here. Eigen is not included. If you don't have it already, you will need to download it from its website.

This is my first project intended to be made publicly available. I did not have many experience in making my code successfully build on someone else's computer. If you are having trouble building this, I might not be able to help.

To build this program on Linux, just make sure you have Qt5, flex and bison installed. If you have Eigen, add its path to the Qt project file `src/strcube.pro`. If you don't have it, download it and place the `Eigen` folder under `src` directory. Now you can build these code by entering `build` directory and execute

`qmake strcube.pro`

`make`

On windows things get tricky. If you want to use GNU toolchain which is our case, you should use MinGW since Qt has MinGW version only which I think is not compatible with Cygwin. But the bison program provided by MinGW is outdated, while the one provided by Cygwin is up to date. So you have to use gcc from MinGW but flex/bison from Cygwin. To reduce confusion, just use the 32-bit MinGW package provided by Cygwin.

Again you need Qt5 and Eigen. You may also need to change the gcc executable names in `src/strcube.pro`. When everything is ready, enter build directory and execute

`qmake strcube.pro`

`make`

## How To Use

### The UI
The UI is still very simple. The display area to the left is where the simulated puzzle is shown and where you interact with it. Left click to operate on the puzzle, right click and drag to rotate, scroll mouse wheel to zoom. The drop box in the lower-left corner is used to switch between display modes. All other functions are accessed through the control panel to the right of the window.

"Load and play" loads a cube description file and runs the simulation in the display area to the left.

"New display" creates an additional display window. These windows are controlled independently. They could be set to syncronize rotation with the display area in the main window.

"Load" loads a cube description file and displays it in a code tab where you may edit the file. "New" cheates a new empty code tab, and "Save" saves the file in the current code tab. Cube description files loaded this way will not be parsed and simulated automaticly, you need to click "Play" to run the simulation.

"Scramble" scrambles the cube, and "Reset" resets it.

Below the set of buttons is the terminal. Most informations will be displayed here. You may type in commands to control the puzzle.

### The Language
As mentioned above, you can build your own puzzles. For this purpose we have designed a cube description language. It functions like a markup language, but with a syntax similar to a general purpose imperative language. I could have used xml, but these cube description files are intended to be concise and primarily written by human, and you won't want to bother with writing those xml tags, which might way exceed the actual useful part in terms of length.

The language design has not been fully implemented yet, but it sould suffice to write a simple puzzle like the Rubik's Cube. The specification of the lange would not be long, but to be able to use it you really have to understand some ideas behind the design, and that might need some tens of pages of documentation with carefully designed examples. I'm not planning to write such a documentation before a full release of this software since I would probably be the only one who will actually write these cube specification files. If you want to see how it works now, just open one `.cub` file and try to read it. If your programming skill enables you to build these code, the syntax should seem straight forward. And if you play Rubik's Cube, you could guess some of the semantics.

## Version Histroy

###alpha 0.2 - 2015.9.6
*	Code editing capabilities, with syntax highlighting
*	Multiple display windows
*	Additional display modes
*	New language semantics implemented: bandage

###alpha 0.1 - 2015.8.29
*	Initial release
*	Parser for full language specification
*	Implemented basic language semantics: tag, symmetry, geometry, transformations, auto, block, block alias, position, position alias, operation, binding
