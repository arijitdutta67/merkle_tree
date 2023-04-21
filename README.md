# Merkle Tree
This is a simple proof of concept implementation of a Merkle tree in C++. It uses sha256 hash function from the `openssl` library. To run the code,
1. Install `openssl`
2. Find the path for `openssl`, for Mac, I did
`brew --prefix openssl`, which gives `/opt/homebrew/opt/openssl@3`.
3. In my Mac, I type the following command to run the code 
`g++ --std=c++17 -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto main.cpp && ./a.out`
