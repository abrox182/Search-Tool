#include "search.h"

SearchEngine::SearchEngine(){
    corpus = {};
}

SearchEngine::~SearchEngine(){
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){

    for (int i=0; i<sentence.size(); i++) {
        sentence[i] = tolower(sentence[i]);
    }
    
    corpus.push_back({book_code, page, paragraph, sentence_no, sentence});

}

void preprocessPattern(int *shiftArray, int *badCharacterHeuristic, char *pattern, int m)
{
    int i=m, j=m+1;
    badCharacterHeuristic[i]=j;

    while(i>0)
    {
        while(j<=m && pattern[i-1] != pattern[j-1])
        {
            if (shiftArray[j]==0)
                shiftArray[j] = j-i;

            j = badCharacterHeuristic[j];
        }
        i--;j--;
        badCharacterHeuristic[i] = j;
    }
    
    j = badCharacterHeuristic[0];
    for(i=0; i<=m; i++)
    {
        if(shiftArray[i]==0)
            shiftArray[i] = j;

        if (i==j)
            j = badCharacterHeuristic[j];
    }
}


Node* SearchEngine::search(string pattern, int& n_matches) {

    // Convert pattern to lowercase too

    for (int i=0; i<pattern.length(); i++) {
        pattern[i] = tolower(pattern[i]);
    }

    n_matches = 0;
    int M = pattern.size();
    Node *rootnode = new Node();
    Node *currnode = rootnode;

    int badCharacterHeuristic[M+1], shiftArray[M+1];

    for(int i=0;i<M+1;i++) {
        shiftArray[i]=0;
    }

    // Boyer Moore algo

    preprocessPattern(shiftArray, badCharacterHeuristic, &pattern[0], M);

    for(int i=0; i<corpus.size(); i++) {

        int N = corpus[i].sentence.size();
        int s=0, j;

        while(s <= N-M)
        {
            j = M-1;

            while(j >= 0 && pattern[j] == corpus[i].sentence[s+j])
                j--;

            if (j<0)
            {
                currnode->book_code = corpus[i].book_code;
                currnode->page = corpus[i].page;
                currnode->paragraph = corpus[i].paragraph;
                currnode->sentence_no = corpus[i].sentence_no;
                currnode->offset = s;
                
                currnode->right = new Node();
                currnode->right->left = currnode;
                currnode = currnode->right;

                n_matches++;
                s += shiftArray[0];
            }
            else
                s += shiftArray[j+1];
        }
    }

    if (n_matches == 0) {
        delete rootnode;
        return NULL;
    }
    else {
        currnode->left->right = NULL;
        delete currnode;
        return rootnode;
    }
}


/*

void traverse(Node* node) {
    int k = 1;
    while(node != NULL) {
        cout << k << ". " << node->book_code << "|" << node->page << "|" << node->paragraph << "|" << node->sentence_no << "|" << node->offset << endl;
        Node* nextNode = node->right;
        node = nextNode;
        k++;
    }
}


// ------------ DEBUG ZONE
#include <fstream>
#include <sstream>
using namespace std;

int main() {
    string filepath = "corpus.txt";
    SearchEngine* se = new SearchEngine();
    ifstream file(filepath);
    string line;
    while (std::getline(file, line)) {
        int pos = line.find(") ");
        string sent = line.substr(pos+2, line.size() - pos - 1);
        string params = line.substr(2, pos-2);

        int a = stoi(params.substr(0, params.find('\'')));
        params = params.substr(params.find(',')+1, params.size()-params.find(','));
        int b = stoi(params.substr(0, params.find(',')));
        params = params.substr(params.find(',')+1, params.size()-params.find(','));
        int c = stoi(params.substr(0, params.find(',')));
        params = params.substr(params.find(',')+1, params.size()-params.find(','));
        int d = stoi(params.substr(0, params.find(',')));
        params = params.substr(params.find(',')+1, params.size()-params.find(','));
        int e = stoi(params);

        se->insert_sentence(a,b,c,d,sent);

    }

    int n = 0;

    Node* res = se->search("lump", n);
    traverse(res);

    while (res != NULL) {
        Node* nex = res->right;
        delete res;
        res = nex;
    }

    cout << n << endl;

    res = se->search("talk with him", n);
    traverse(res);

    while (res != NULL) {
        Node* nex = res->right;
        delete res;
        res = nex;
    }

    cout << n << endl;

    res = se->search("inexp", n);
    traverse(res);

    while (res != NULL) {
        Node* nex = res->right;
        delete res;
        res = nex;
    }

    cout << n << endl;

    delete se;
    
}
*/