#include "Compiler.h"

#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "TextReader.h"
#include "TextWriter.h"
#include "UTF8.h"

#include "SimScanner.h"
#include "SimHandler.h"
#include "SimHandlerAdapter.h"
#include "SimDefaultBufferedScanner.h"
#include "SimSemanticorAdapter.h"
#include "SimNoMatchException.h"
#include "SimParseException.h"
#include "SimParser.h"
#include "SimSemanticorImpl.h"
#include "SimToken.h"
#include "Attribute.h"
#include "Checker.h"
#include "Gen.h"

#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace lex;
using namespace parser;
using namespace llvm;

Compiler::Compiler() {
}

Compiler::~Compiler() {
}

bool Compiler::compile(string fileName) {
	clock_t t1 = clock();
	shared_ptr<Context> ctx = make_shared<Context>();
	shared_ptr<Node> ast = parse(fileName);
	if (!ast) {
		return false;
	}
	if (!attribute(ast, ctx)) {
		return false;
	}
	if (!check(ast, ctx)) {
		return false;
	}
	initLLVM();
	if (!gen(fileName, ast, ctx)) {
		return 1;
	}
	clock_t t2 = clock();

	cout << "successfully compiled in " << 1000 * (t2 - t1) / CLOCKS_PER_SEC << " ms" << endl;
	return true;
}

shared_ptr<Node> Compiler::parse(string fileName) {
	std::fstream fin(fileName, std::ios::in | std::ios::binary);
	if (!fin.is_open()) {
		cout << "can't open file: " << fileName << endl;
		return 0;
	}

	try {
		TextReader reader(&fin, utf8_decoder);
		reader.skipUTF8BOM();
		SimHandlerAdapter handler;
		SimScanner scanner(&reader, &handler);

		SimSemanticorImpl semanticor;
		SimDefaultBufferedScanner bscanner(&scanner);
		SimParser parser(&bscanner, &semanticor);

		parser.parse();
		shared_ptr<Node> ast = semanticor.getAst();

		fin.close();
		return ast;
	} catch (SimNoMatchException &e) {
		cout << e.getLine() << " line, " << e.getColumn() << " column, lexical error" << endl;
	} catch (SimParseException &e) {
		cout << e.getLine() << " line, " << e.getColumn() << " column, parse error: " << e.getMsg()
				<< endl;
	}
	return 0;
}

bool Compiler::attribute(shared_ptr<Node> node, shared_ptr<Context> ctx) {
	::Attribute attr(ctx);
	node->accept(&attr);
	return attr.result == 0;
}

bool Compiler::check(shared_ptr<Node> node, shared_ptr<Context> ctx) {
	Checker check(ctx);
	node->accept(&check);
	return check.result == 0;
}

bool Compiler::gen(string fileName, shared_ptr<Node> node, shared_ptr<Context> ctx) {
	Gen gen(fileName, ctx);
	node->accept(&gen);
	return gen.result == 0;
}

void Compiler::initLLVM() {
	// Initialize the target registry etc.
	//InitializeAllTargetInfos();
	LLVMInitializeX86TargetInfo();
	//InitializeAllTargets();
	LLVMInitializeX86Target();
	//InitializeAllTargetMCs();
	LLVMInitializeX86TargetMC();
	//InitializeAllAsmParsers();
	LLVMInitializeX86AsmParser();
	//InitializeAllAsmPrinters();
	LLVMInitializeX86AsmPrinter();
}

