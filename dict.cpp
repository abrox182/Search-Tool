// Do NOT add any other includes
#include "dict.h"

Dict::Dict(){

    root = new TrieNode();

}

Dict::~Dict() {
    // iteratively delete all trie nodes, using stack

    vector<TrieNode*> nodeStk;
    nodeStk.push_back(root);

    while (nodeStk.size() != 0) {
        TrieNode* curr = nodeStk.back();
        nodeStk.pop_back();

        for (int i=0; i < 128; i++) {
            if (curr->children[i] != NULL) {
                nodeStk.push_back(curr->children[i]);
            }
        }

        delete curr;
    }
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence) {

    string word;
    TrieNode* currNode = root;

    std::vector<TrieNode*> nodeStk;
    nodeStk.push_back(currNode);

    for (int i=0; i<sentence.size(); i++) {
        // find the appropriate trie index, simultaneously check if its nonseparator

        int charIndex = tolower(sentence[i]);

        if (charIndex != '.' &&
        charIndex != ',' &&
        charIndex != '-' &&
        charIndex != ':' &&
        charIndex != '!' &&
        charIndex != '"' &&
        charIndex != '\'' &&
        charIndex != '(' &&
        charIndex != ')' &&
        charIndex != '?' &&
        charIndex != '[' &&
        charIndex != ']' &&
        charIndex != ';' &&
        charIndex != '@' &&
        charIndex != ' ') {

            sentence[i] = tolower(sentence[i]);

            if (!currNode->children[charIndex]) {

                currNode->children[charIndex] = new TrieNode;
                currNode->children[charIndex]->wordCount = 0;

                // populate children (lol)

                for (int i = 0; i < 128; i++)
                    currNode->children[charIndex]->children[i] = nullptr;

                nodeStk.push_back(currNode->children[charIndex]);

            }

            currNode = currNode->children[charIndex];
            word.push_back(sentence[i]);

        } 

        else {
            if (!word.empty()) {

                currNode->wordCount++;
                word.clear();
                currNode = root;
                nodeStk.push_back(currNode);

            }
        }
    }
    
    if (!word.empty()) {

        currNode->wordCount++;

    }
}


int Dict::get_word_count(string word) {

    TrieNode *currNode = root;

    for (int i=0; i<word.size(); i++) {

        // get appropriate index

        int index = tolower(word[i]);

        // traverse to that index

        if (currNode->children[index] == NULL) {
            // it doesn't exist
            return 0;
        }
        
        currNode = currNode->children[index];

    }

    if (currNode != NULL) {
        return currNode->wordCount;
    }
    else {
        return 0;
    }
    
}



void Dict::dump_dictionary(string filename) {
    ofstream fileStream;
    fileStream.open(filename);

    std::vector<TrieNode*> nodeStk;
    std::vector<string> wordStk;

    nodeStk.push_back(root);
    wordStk.push_back("");

    while (nodeStk.size() != 0) {

        TrieNode* currNode = nodeStk.back();
        nodeStk.pop_back();

        string currWord = wordStk.back();
        wordStk.pop_back();

        if (currNode->wordCount != 0) {
            fileStream << currWord << ", " << currNode->wordCount << "\n";
        }
            
        for (int i = 0; i < 128; i++) {

            if (currNode->children[i] != NULL) {

                nodeStk.push_back(currNode->children[i]);
                wordStk.push_back(currWord + char(i));

            }
        }
    }

    fileStream.close();
}