g++ -g main.cpp `llvm-config --cxxflags --ldflags --libs` -c -o main.o
g++ -g TokenStream.cpp `llvm-config --cxxflags --ldflags --libs` -c -o TokenStream.o
g++ -g Lexer.cpp `llvm-config --cxxflags --ldflags --libs` -c -o Lexer.o
g++ -g Parser.cpp `llvm-config --cxxflags --ldflags --libs` -c -o Parser.o
g++ -g main.o TokenStream.o Lexer.o Parser.o `llvm-config --cxxflags --ldflags --libs` -ldl -o test.exe
