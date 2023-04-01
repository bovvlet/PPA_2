all:
	bison -d -v -o parser.cpp parser.y && flex --header-file=lexer.hpp -o lexer.cpp lexer.l && g++ lexer.cpp ConstantPropagation/ConstantPropagation.cpp main.cpp -o program && ./program
