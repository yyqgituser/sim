# Sim
A Simple programming language implementation based on LLVM.
Sim is intended as a demostration of programming lanugage design, though it has something more than [Kaleidoscope](https://llvm.org/docs/tutorial/index.html). Visitor design pattern make it more extensiable. 

### Build for Linux
1. Build and install [LLVM](https://llvm.org/docs/CMake.html), make sure [LLVM library for cmake](https://llvm.org/docs/CMake.html#id15) are properly installed.
2. Download source  
 #git clone https://github.com/yyqgituser/sim.git  
 #cd sim  
 #mkdir build  
 #cmake ..  
 #make
3. Run examples, simc only compile sim source to object file, you need a linker to generate excutable file(here we use gcc as a linker)
 #cd examples  
 #../build/simc hello.sim  
 #gcc hello.o  
 Except the .o file, simc also generate file hello.ll which include the IR code.
 
 If you want to build Sim for windows, you need install [MinGW](http://www.mingw.org/) and tools.
 

