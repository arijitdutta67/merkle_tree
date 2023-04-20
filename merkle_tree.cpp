#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

using namespace std;

class MerkleTree {
public:
    // Constructor
    MerkleTree(const vector<string>& data) {
        // Calculate the number of levels (d+1) in the tree, d = depth
        int n = data.size();
        int levels = 1;
        while (n > 1) {
            levels++;
            n = (n + 1) / 2;
        }
        // Initialize the tree with empty hashes (2^(d+1) - 1)
        tree.resize((1 << levels) - 1, string(SHA256_DIGEST_LENGTH, '0'));
        // Calculate the leaf node hashes
        for (int i = 0; i < data.size(); i++) {
            string hash = sha256(data[i]);
            tree[i] = hash;
        }
        std::cout << "levels = " << levels << std::endl; //3
        // Calculate the rest of the tree
        int offset_lo = 0;
        int offset_hi = 1 << (levels - 1);
        for (int i = levels - 2; i >= 0; i--) {
            for(int j = 0; j <= (1 << i); j += 2) {
                string left_child = tree[offset_lo + j];
                string right_child = tree[offset_lo + j + 1];
                string combined_hash = left_child + right_child;
                tree[offset_hi + j/2] = sha256(combined_hash);
            }
            offset_lo = offset_hi;
            offset_hi += (1 << i);
        }

        // for (int i = 0; i < tree.size(); i++) {
        //    std::cout << "tree = " << tree[i] << ", i = " << i << std::endl; 
        // }
    }
    string root() const {
        int last_index = tree.size() - 1;
        return tree[last_index];
    }
    private:
    std::vector<string> tree;
    int levels;
    std::string sha256(const std::string& str)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(str.c_str()), str.length(), hash);

        std::stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }

        return ss.str();
    }
};

int main() {
    auto data = std::vector<string>({"Hello", "world", "3", "4"});
    MerkleTree tree(data);
    std::cout << "Root hash = " << tree.root() << std::endl;
    return 0;
}



/*Steps of Todos
1. Produce a Merkle proof
2. Verify a Merkle proof
3. To run the code with the command
g++ --std=c++17 -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto merkle_tree.cpp && ./a.out
Hashes
Hello = 185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969
world = 486ea46224d1bb4fb680f34f7c9ad96a8f24ec88be73ea8e5a6c65260e9cb8a7
3 = 4e07408562bedb8b60ce05c1decfe3ad16b72230967de01f640b7e4729b49fce
4 = 4b227777d4dd1fc61c6f884f48641d02b4d121d3fd328cb08b5531fcacdabf8a
H(185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969, 486ea46224d1bb4fb680f34f7c9ad96a8f24ec88be73ea8e5a6c65260e9cb8a7)
=4fb8d657287a2bde741f350b773fbe6f79fa6ac2ab0fe45dc20ef919c21a611e
H(4e07408562bedb8b60ce05c1decfe3ad16b72230967de01f640b7e4729b49fce, 4b227777d4dd1fc61c6f884f48641d02b4d121d3fd328cb08b5531fcacdabf8a)
= 13656c83d841ea7de6ebf3a89e0038fea9526bd7f686f06f7a692343a8a32dca
H(4fb8d657287a2bde741f350b773fbe6f79fa6ac2ab0fe45dc20ef919c21a611e, 13656c83d841ea7de6ebf3a89e0038fea9526bd7f686f06f7a692343a8a32dca)
eba77b94d139e0d801f30a90faa46ff05eb73ab67fb28ddccc01a2298979c583


*/