g++ -g main.cpp `llvm-config --cxxflags` -c -o main.o
g++ -g TokenStream.cpp `llvm-config --cxxflags` -c -o TokenStream.o
g++ -g Lexer.cpp `llvm-config --cxxflags` -c -o Lexer.o
g++ -g Parser.cpp `llvm-config --cxxflags` -c -o Parser.o
g++ -g CodeGen.cpp `llvm-config --cxxflags` -c -o CodeGen.o
g++ -g main.o TokenStream.o Lexer.o Parser.o CodeGen.o `llvm-config --libs lto` `llvm-config --libs core` `llvm-config --ldflags` `llvm-config --libs` -lpthread -lffi -ldl -lm -lLLVMCore -lLLVMSupport -lLLVMTarget -lz -o test.out 
