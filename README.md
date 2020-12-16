# Sim: A Simple programming language based on LLVM
Sim is intended as a demostration of programming lanugage design, though it has something more than [Kaleidoscope](https://llvm.org/docs/tutorial/index.html). [Visitor design pattern](https://en.wikipedia.org/wiki/Visitor_pattern) makes it more extensiable. 

## Table of Content
- [Build for Linux](#build-for-linux)
- [Code example](#code-example)
- [Features](#features)

### Build for Linux
1. Build and install [LLVM](https://llvm.org/docs/CMake.html), make sure [LLVM library for cmake](https://llvm.org/docs/CMake.html#id15) are properly installed.
2. Download source  
 #git clone https://github.com/yyqgituser/sim.git  
 #cd sim  
 #mkdir build  
 #cmake ..  
 #make  
 #sudo make install

3. Run examples, simc only compile sim source to object file, you need a linker to generate excutable file(here we use gcc as a linker)  
 #cd examples  
 #simc hello.sim  
 #gcc hello.o -o hello  
 #./hello
 
 Except the .o file, simc also generate file hello.ll which includes IR code.
 
 If you want to build Sim for windows, you need install [MinGW](http://www.mingw.org/) and other tools.
 
### Code example
```
// proto type declarations for calling C function
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
More examples can be found in [examples](https://github.com/yyqgituser/sim/tree/main/examples)

### Features
1. Static type check
  Supported type: int double, string, boolean, void. int is a 32 bits signed integer and double is 64 bits. string is a zero-ended 8 bits array.
  
2. Grammar, refer to [grammar and lex files](https://github.com/yyqgituser/sim/tree/main/grammar)

3. Control flow
  Support if/else, while/for just like normal C syntax.

4. Literal
   int: 123
   double: 1.23 1e10
   boolean: true false  
   string: "a string"  
   array: {1, 2, 3}

5. Expression
Support common C-like operators: || && == != < > <= >= | & << >> + - * / % ^ ! ()  
Don't support shortcut evaluation of logic expression.  
Don't support '?' expression
Don't support '++' and '--' as operators, but support them as statement. for example:
```
a = b++; // illegal
i++; // legal
```
Type cast are only supported between numbers, in this case, only int <-> double convertion.

6. Sub function
Support user defined function. Function declaration without body is allowed, it will be treated as external function, and it rely on the linker to find and link that function into the final executable file. In the way Sim program can call some C library function.

7. Array and string
Array type and string type variable are implemented as reference.

8. ***Everything in stack!***  
Sim don't have runtime heap, except global variables, all variables are allocated from stack.

9. Dead code detection  
Sim compiler will report error if it detected some unreachable code.
```
int a;
if(n == 1) {
	return 1;
} else {
	return 2;
}
a = 1; // error
```




