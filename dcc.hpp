#pragma once
#include <string>
namespace practicellvm{

    //コマンドライン文字列切り出しクラス
    class OptionParser{
        std::string inputFileName;
        std::string outputFileName;
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
        std::string GetInputFileName(){return inputFileName;} 
        //出力ファイル名取得		
		std::string GetOutputFileName(){return outputFileName;} 	
        bool parseOption(){
            if(argc<2){
                fprintf(stderr,"引数が足りません");
                return false;
            }

            for(int i=1;i<argc;i++){
                if(argv[i][0]=='-'){
                    if(argv[i][1]=='o' && argv[i][2]=='\0')
                        outputFileName.assign(argv[++i]);
                    else if(argv[i][1]=='h' && argv[i][2]=='\0'){
                        PrintHelp();
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
            //outputFileNameの自動生成
            if(outputFileName.empty()){
                const auto & ifn=inputFileName;
                const int len=ifn.length();
                if(len>2 && ifn[len-3]=='.' && ifn[len-2]=='d' && ifn[len-1]=='c'){
                    outputFileName=std::string(ifn.begin(),ifn.end()-3);
                    outputFileName+=".ll";
                }
                else{
                    outputFileName=ifn;
                    outputFileName+=".ll";
                }
            }
            return true;
        }
    };
}