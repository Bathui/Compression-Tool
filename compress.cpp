#include "HCTree.hpp"
#include <stdexcept>
#include <string>

void writeHeader(FancyOutputStream& out, vector<int> & freqs){
    int uniqueSymbol = 0;
    for(int i = 0; i < 256; i++){
        if(freqs[i] > 0)
            uniqueSymbol++;
    }
    // cout<<uniqueSymbol<<endl;
    out.write_int(uniqueSymbol);

    for(int i = 0; i < 256; i++){
        if(freqs[i] > 0){
            out.write_byte(static_cast<unsigned char>(i));
            
            out.write_byte(static_cast<unsigned char>((freqs[i] >> 8) & 0xff));
            out.write_byte(static_cast<unsigned char>(freqs[i] & 0xff));
        }
    }

}
void compress(char* input, char* output){
    vector<int> freqs(256, 0);
    bool isEmpty = true;
    FancyInputStream in(input);

    if(!in.good()){
        cerr<<"Error: open failed: "<< input<<endl;
        return;
    }

    int byte = in.read_byte();
    while(byte != -1){
        isEmpty = false;
        freqs[byte]++;
        byte = in.read_byte();
    }

    if(isEmpty){
        FancyOutputStream out(output);
        return;
    }
    HCTree huf;
    huf.build(freqs);

    FancyOutputStream out(output);
    if(!out.good()){
        cerr<<"Error: open failed: "<< output<<endl;
        return;
    }

    //following is the header
    writeHeader(out, freqs);

    in.reset();
    byte = in.read_byte(); 
    while(byte != -1){
        huf.encode(static_cast<unsigned char>(byte), out);
        byte = in.read_byte();
    }
    out.flush();

}

int main(int argc, char*argv[]){
    try{
        if(argc != 3)
            throw invalid_argument("Incorrect parameters\nUSAGE: " + string(argv[0]) + " <original_file> <compressed_file>\n");
        
        char* input = argv[1];
        char* output = argv[2];

        compress(input,output);
    }
    catch(invalid_argument& error){
        cerr<<error.what();
        return 1;
    }
}