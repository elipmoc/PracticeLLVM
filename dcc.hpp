#pragma once
#include <string>
namespace practicellvm{

    //コマンドライン文字列切り出しクラス
    class OptionParser{
        std::string inputFilename;
        std::s(tring outputFilename;
        int argc;
        char **argv;
        public:
        OptionParser(int _argc,char** _argv)
            :argc(_argc),argv(_argv)
            {}
        
        void PrintHelp(){
            fprintf(stdout,"いいよこいよ");
        }

        //入力ファイル名取得
        std::string getInputFileName(){return inputFileName;} 
        //出力ファイル名取得		
		std::string getOutputFileName(){return outputFileName;} 	
        bool parseOption(){
            if(argc<2){
                fprintf(stderr,"引数が足りません");
                return false;
            }

            for(int i=1;i<argc;i++){
                if(argv[i][0]=='-'){
                    if(argv[i][1]=='o' && argv[i][2]=='\0')
                        outputFilename.assign(argv[++i]);
                    else if(argv[i][1]=='h' && argv[i][2]=='\0'){
                        PrintfHelp();
                        return false;
                    }
                    else {
                        fprintf(stderr,"不明なオプションです");
                        return false;
                    }
                }
                else{
                    inputFileName.assign(argv[i]);
                }
            }

        }
        }

}