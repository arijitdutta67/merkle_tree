#include "merkle_tree.hpp"

int main() {
    auto data = std::vector<string>({"Hello", "world", "3", "4", "5", "6"});
    MerkleTree tree(data);
    std::cout << "Root hash = " << tree.root() << std::endl;
    auto proof = tree.compute_merkle_proof("6");
    tree.verify_merkle_proof("6", proof);
    return 0;
}



/* 
To run the code with the command
g++ --std=c++17 -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto main.cpp && ./a.out
*/