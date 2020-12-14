cmake_minimum_required(VERSION 3.1)

project(simc)

enable_testing()

find_package(LLVM REQUIRED CONFIG)
	
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -s")

add_library(cstreamio cstreamio/src/UTF8.cpp)
target_include_directories(cstreamio PUBLIC cstreamio/include)

include_directories(auto)
include_directories(src)
include_directories(src/util)
include_directories(src/node)
include_directories(src/type)
include_directories(src/sym)

add_executable(simc auto/SimScannerData.cpp 
					auto/SimScanner.cpp 
					auto/SimParser.cpp 
					auto/SimParserData.cpp 
					src/Main.cpp 
					src/node/Node.cpp
					src/node/Term.cpp
					src/node/Literal.cpp
					src/node/PrimitiveTypeNode.cpp
					src/node/ArrayTypeNode.cpp
					src/node/VarDecl.cpp
					src/node/Top.cpp
					src/node/Method.cpp
					src/node/Parameter.cpp
					src/node/ArrayInitializer.cpp
					src/node/Block.cpp
					src/node/VarDeclStatement.cpp
					src/node/ExpressionStatement.cpp
					src/node/IfStatement.cpp    
					src/node/WhileStatement.cpp
					src/node/BreakStatement.cpp
					src/node/ContinueStatement.cpp
					src/node/ReturnStatement.cpp
					src/node/BinaryExpression.cpp
					src/node/UnaryExpression.cpp
					src/node/CastExpression.cpp
					src/node/Identifier.cpp
					src/node/ArrayAccess.cpp
					src/node/MethodInvocation.cpp
					src/node/Assignment.cpp
					src/node/NodeVisitor.cpp
					src/node/Location.cpp
					src/node/EmptyStatement.cpp
					src/node/XcreaseStatement.cpp
					src/node/ForStatement.cpp
					src/type/Type.cpp
					src/type/StringType.cpp
					src/type/IntType.cpp
					src/type/DoubleType.cpp
					src/type/ArrayType.cpp
					src/type/MethodType.cpp
					src/type/BoolType.cpp
					src/type/VoidType.cpp
					src/type/ErrorType.cpp					
					src/sym/Symbol.cpp
					src/sym/VarSymbol.cpp
					src/sym/GlobalVarSymbol.cpp
					src/sym/MethodSymbol.cpp					
					src/util/misc.cpp
					src/Context.cpp
					src/SimSemanticorImpl.cpp
					src/Attribute.cpp
					src/Checker.cpp
					src/Gen.cpp
					src/Compiler.cpp
)

llvm_map_components_to_libnames(llvm_libs support core irreader target x86asmparser x86codegen x86desc x86disassembler x86info)

target_link_libraries(simc cstreamio ${llvm_libs})

