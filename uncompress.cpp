#include "HCTree.hpp"

void readHeader(FancyInputStream& in, vector<int>& freqs){
    int uniqueSymbol = in.read_int();
    // cout<<uniqueSymbol<<endl;
    for(int i = 0; i < uniqueSymbol; i++){
        int symbol = static_cast<unsigned char>(in.read_byte());
        
        int freq =  
           (static_cast<unsigned int>(in.read_byte()) << 8) | 
           static_cast<unsigned int>(in.read_byte());
        freqs[symbol] = freq;
    }

}

void decompress(char* input, char* output){
    FancyInputStream in(input);
    if(!in.good()){
        cerr<<"Error: open failed: "<< input<<endl;
        return;
    }
    
    int check = in.read_bit();
    if(check == -1){
        FancyOutputStream out(output);
        return;
    }
        // cout<<"check"<<endl;

    in.reset();

    FancyOutputStream out(output);
    //read header
    vector<int> freqs(256, 0); // get the frequency table used in building tree
    int n = freqs.size();
    readHeader(in, freqs);

    int sum = 0;
    for(int i = 0; i < n; i++){
        sum += freqs[i];
    }
    // cout<<sum<<endl;
    HCTree h;
    h.build(freqs);

    if(!out.good()){
        cerr<<"Error: open failed: "<< input<<endl;
        return;
    }
    
    for(int i = 0; i < sum; i++){
        unsigned char symbol = h.decode(in);
        out.write_byte(symbol);
    }
    
}
int main(int argc, char* argv[]){
    try{
        if(argc != 3)
            throw invalid_argument("Incorrect parameters\nUSAGE: " + string(argv[0]) + " <original_file> <compressed_file>\n");
        
        char* input = argv[1];
        char* output = argv[2];

        decompress(input,output);
    }
    catch(invalid_argument& error){
        cerr<<error.what();
        return 1;
    }
}