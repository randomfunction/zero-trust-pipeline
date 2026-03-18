#pragma once
#include <vector>
#include <cstdint>

class FastBitmap {
    std::vector<uint64_t> L1; 
    std::vector<uint64_t> L2; 
    std::vector<uint64_t> L3; 
    uint64_t L4 = 0;          

    size_t size;

public:
    FastBitmap(size_t max_bits) : size(max_bits) {
        L1.resize((max_bits + 63) / 64, 0);
        L2.resize((L1.size() + 63) / 64, 0);
        L3.resize((L2.size() + 63) / 64, 0);
    }

    void set(size_t idx) {
        if (idx >= size) return;
        L1[idx / 64] |= (1ULL << (idx % 64));
        L2[idx / 4096] |= (1ULL << ((idx / 64) % 64));
        L3[idx / 262144] |= (1ULL << ((idx / 4096) % 64));
        L4 |= (1ULL << ((idx / 262144) % 64));
    }

    void reset(size_t idx) {
        if (idx >= size) return;
        L1[idx / 64] &= ~(1ULL << (idx % 64));
        if (L1[idx / 64] == 0) {
            L2[idx / 4096] &= ~(1ULL << ((idx / 64) % 64));
            if (L2[idx / 4096] == 0) {
                L3[idx / 262144] &= ~(1ULL << ((idx / 4096) % 64));
                if (L3[idx / 262144] == 0) {
                    L4 &= ~(1ULL << ((idx / 262144) % 64));
                }
            }
        }
    }

    int get_highest() const {
        if (L4 == 0) return -1;
        int l4_idx = 63 - __builtin_clzll(L4);
        
        uint64_t l3_val = L3[l4_idx];
        if (l3_val == 0) return -1;
        int l3_idx = l4_idx * 64 + (63 - __builtin_clzll(l3_val));
        
        uint64_t l2_val = L2[l3_idx];
        if (l2_val == 0) return -1;
        int l2_idx = l3_idx * 64 + (63 - __builtin_clzll(l2_val));
        
        uint64_t l1_val = L1[l2_idx];
        if (l1_val == 0) return -1;
        int l1_idx = l2_idx * 64 + (63 - __builtin_clzll(l1_val));
        
        return l1_idx;
    }

    int get_lowest() const {
        if (L4 == 0) return -1;
        int l4_idx = __builtin_ctzll(L4);
        
        uint64_t l3_val = L3[l4_idx];
        if (l3_val == 0) return -1;
        int l3_idx = l4_idx * 64 + __builtin_ctzll(l3_val);
        
        uint64_t l2_val = L2[l3_idx];
        if (l2_val == 0) return -1;
        int l2_idx = l3_idx * 64 + __builtin_ctzll(l2_val);
        
        uint64_t l1_val = L1[l2_idx];
        if (l1_val == 0) return -1;
        int l1_idx = l2_idx * 64 + __builtin_ctzll(l1_val);
        
        return l1_idx;
    }
};
