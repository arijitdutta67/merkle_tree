# Merkle Tree
This is a simple proof of concept implementation of a Merkle tree in C++. It uses sha256 hash function from the `openssl` library. To run the code,
1. Install `openssl` 
2. In my Mac, I type the following command to run the code 

`g++ --std=c++17 -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto merkle_tree.cpp && ./a.out`
