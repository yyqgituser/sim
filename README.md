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
 
 If you want to build Sim for windows, you need install [MinGW](http://www.mingw.org/) and other tools.
 
### Code examples
```
// proto type declarations for calling c function
int getchar();
int putchar(int c);
int printf(string s, ...);

int readInt() {
	int c = getchar();
	int v = 0;
	while (c >= 48 && c <= 57) {
		v = v * 10 + c - 48;
		c = getchar();
	}
	return v;
}

int fib(int n) {
    if (n <= 2) {
        return 1;
    }
    return fib(n - 1) + fib(n - 2);
}

int main() {
    int n = readInt();
    for (int i = 1; i <= n; i++) {
        printf("%d: %d\n", i, fib(i));
    }
    return 0;
}
```
More example can be found in [examples](https://github.com/yyqgituser/sim/tree/main/examples)


