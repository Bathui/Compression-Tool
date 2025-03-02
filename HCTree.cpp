#include "HCTree.hpp"
#include <stack>
void HCTree::build(const vector<int>& freqs){
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> queue;

    int n = freqs.size();
    for(int i = 0; i < n; i++){
        if(freqs[i] > 0){
            HCNode* leave = new HCNode(freqs[i], static_cast<unsigned char>(i));
            queue.push(leave);
            leaves[i] = leave;
        }
    }

    while(queue.size() > 1){
        HCNode* left = queue.top(); // smaller
        queue.pop();

        HCNode* right = queue.top(); // bigger
        queue.pop();

        HCNode* p = new HCNode(left->count + right->count, left->symbol);
        p->c0 = left;
        p->c1 = right;

        left->p = p;
        right->p = p;

        queue.push(p);
    }

    if(!queue.empty())
        root = queue.top();
    else
        root = nullptr;
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out)const{
    HCNode* search = leaves[symbol]; // get corresponding node

    if(!search)
        return;

    stack<int> data;

    while(search->p){
        if(search == search->p->c1){
            data.push(1);
        }
        else{
            data.push(0);
        }

        search = search->p;
    }

    while(!data.empty()){
        out.write_bit(data.top());
        data.pop(); 
    }

}
unsigned char HCTree::decode(FancyInputStream & in) const{
    HCNode* search = root;
    if(!search)
        return 0;
    
    while(search->c0 || search->c1){
        int data = in.read_bit();
        if (data == -1)
            break;
        
        if(data == 0)
            search = search->c0;
        else
            search = search->c1;
    }

    return search->symbol;
}


void deleteHelper(HCNode* root){
    if(!root)
        return;
    deleteHelper(root->c0);
    deleteHelper(root->c1);

    delete root;
}
HCTree::~HCTree(){
    deleteHelper(root);
}