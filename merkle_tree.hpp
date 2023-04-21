#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <cmath>

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
    }
    
    // Compute the root of the Merkle tree
    
    string root() const {
        int last_index = tree.size() - 1;
        return tree[last_index];
    }

    // Compute membership proof
    struct Merkle_proof {
        size_t index;
        std::vector<string> hash_path;
    };
    Merkle_proof compute_merkle_proof(const string& input_value) {
        Merkle_proof proof;
        auto element = sha256(input_value);
        auto it = std::find(tree.begin(), tree.end(), element);
        if (it != tree.end()) {
            size_t index = std::distance(tree.begin(), it);
            std::cout << "Element found at index " << index << ", computing proof." << std::endl;
            proof.index = index;
            size_t levels =  log2(tree.size() + 1); 
            std::vector<int> reverse_bits;
            reverse_bits.resize(levels - 1, 0);
            proof.hash_path.resize(levels - 1, string(SHA256_DIGEST_LENGTH, '0'));
            for (size_t j = size_t(0); j < levels -1; j++) {
                reverse_bits[j] = (index >> j) & 1; 
            }
            if(reverse_bits[0] == 0) {
                std::cout << "hash path index = " << index + 1 << std::endl;
                proof.hash_path[0] = tree[index + 1];
            }
            else {
               std::cout << "hash path index = " << index - 1 << std::endl; 
               proof.hash_path[0] = tree[index - 1];
            }
            int offset = 1 << (levels - 1);
            int j = 1;
            for (int i = levels - 2; i > 0; i--) {
            if(reverse_bits[j] == 0) {
                std::cout << "hash path index = " << offset + (index/(2*j)) + 1 << std::endl;
                proof.hash_path[j] = tree[offset + (index/(2*j)) + 1];
            }
            else {
               std::cout << "hash path index = " << offset + (index/(2*j)) - 1 << std::endl; 
               proof.hash_path[j] = tree[offset + (index/(2*j)) - 1];
            }
                offset += (1 << i);
                j += 1;
            }
            return proof;


        } else {
            // Element not found
            std::cout << "Element not found, returning invalid proof" << std::endl;
            Merkle_proof proof;
            return proof;
        }
    }

    // Verification of Merkle proof

    void verify_merkle_proof(const string& input_value, const Merkle_proof& proof) {
        auto element = sha256(input_value);
        auto& index = proof.index;
        auto& hash_path = proof.hash_path;
        size_t levels =  log2(tree.size() + 1); 
        std::vector<int> reverse_bits;
        reverse_bits.resize(levels - 1, 0);
        for (size_t j = size_t(0); j < levels -1; j++) {
            reverse_bits[j] = (index >> j) & 1;
        }
        string temp_hash;
        if(reverse_bits[0] == 0) {
                temp_hash = sha256(element + hash_path[0]);
            }
            else {
               temp_hash = sha256(hash_path[0] + element);
            }
            int j = 1;
            for (int i = levels - 2; i > 0; i--) {
                if(reverse_bits[j] == 0) {
                    temp_hash = sha256(temp_hash + hash_path[j]);
                }
                else {
                    temp_hash = sha256(hash_path[j] + temp_hash);
                }
                j += 1;
            }
            if (temp_hash == root()) {
                std::cout << "Proof is correct, membership verified!" << std::endl;
            }
            else {
               std::cout << "Proof is not correct, membership could not be verified!" << std::endl; 
            }


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

