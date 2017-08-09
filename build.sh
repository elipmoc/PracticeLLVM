g++ -g main.cpp `llvm-config --cxxflags --ldflags --libs` -c -o main.o
g++ -g TokenStream.cpp `llvm-config --cxxflags --ldflags --libs` -c -o TokenStream.o
g++ -g Lexer.cpp `llvm-config --cxxflags --ldflags --libs` -c -o Lexer.o
g++ -g Parser.cpp `llvm-config --cxxflags --ldflags --libs` -c -o Parser.o
g++ -g CodeGen.cpp `llvm-config --cxxflags --ldflags --libs` -c -o CodeGen.o
g++ -g main.o TokenStream.o Lexer.o Parser.o CodeGen.o `llvm-config  --libs core` `llvm-config --cxxflags --ldflags --libs` -lpthread -lffi -ldl -lm -lLLVMCore -lLLVMSupport -o test
