// Qwen attempt 2 (real output of Qwen3.7-Plus on chat.qwen.ai, thinking off, code interpreter active).
// Outcome: PASSED all 12 tests (1.4 s on the slowest). Two +/-1 sign segment trees, values grouped;
// left tree counts strictly greater, right tree strictly smaller, so ties are handled correctly.
// Kept verbatim below as the record of that run.

#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    int n;
    vector<long long> tree;
    vector<char> lazy;
    
    SegTree(int size) : n(size), tree(4 * size, 0), lazy(4 * size, 0) {
        build(1, 0, n - 1);
    }
    
    void build(int node, int l, int r) {
        if (l == r) {
            tree[node] = 1;
            return;
        }
        int mid = (l + r) / 2;
        build(2 * node, l, mid);
        build(2 * node + 1, mid + 1, r);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
    
    void push(int node, int l, int r) {
        if (lazy[node]) {
            tree[2 * node] *= -1;
            lazy[2 * node] ^= 1;
            tree[2 * node + 1] *= -1;
            lazy[2 * node + 1] ^= 1;
            lazy[node] = 0;
        }
    }
    
    void update_range(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            tree[node] *= -1;
            lazy[node] ^= 1;
            return;
        }
        push(node, l, r);
        int mid = (l + r) / 2;
        update_range(2 * node, l, mid, ql, qr);
        update_range(2 * node + 1, mid + 1, r, ql, qr);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
    
    long long query_range(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return tree[node];
        push(node, l, r);
        int mid = (l + r) / 2;
        return query_range(2 * node, l, mid, ql, qr) + query_range(2 * node + 1, mid + 1, r, ql, qr);
    }
    
    void flip(int l, int r) {
        if (l > r) return;
        update_range(1, 0, n - 1, l, r);
    }
    
    long long query(int l, int r) {
        if (l > r) return 0;
        return query_range(1, 0, n - 1, l, r);
    }
    
    long long point_query(int idx) {
        return query(idx, idx);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<int> a(n + 1); // 1-indexed
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    
    // Group positions by value
    map<int, vector<int>> groups;
    for (int i = 1; i <= n; ++i) {
        groups[a[i]].push_back(i);
    }
    
    // Compute S_E
    vector<long long> S_E(n + 1, 0);
    {
        SegTree seg(n + 1); // indices 0 to n
        
        // Process in decreasing order of value
        vector<int> sorted_values;
        for (auto& [val, positions] : groups) {
            sorted_values.push_back(val);
        }
        sort(sorted_values.rbegin(), sorted_values.rend());
        
        for (int val : sorted_values) {
            auto& positions = groups[val];
            
            // Query first
            for (int i : positions) {
                long long T_E = seg.query(0, i - 1);
                long long v_im1 = (i - 1 >= 0) ? seg.point_query(i - 1) : 1;
                long long sign = (i % 2 == 0) ? 1 : -1;
                S_E[i] = sign * v_im1 * T_E;
            }
            
            // Then flip
            for (int i : positions) {
                seg.flip(i, n);
            }
        }
    }
    
    // Compute S_F
    vector<long long> S_F(n + 1, 0);
    {
        SegTree seg(n + 1); // indices 0 to n
        
        // Process in increasing order of value
        vector<int> sorted_values;
        for (auto& [val, positions] : groups) {
            sorted_values.push_back(val);
        }
        sort(sorted_values.begin(), sorted_values.end());
        
        for (int val : sorted_values) {
            auto& positions = groups[val];
            
            // Query first
            for (int i : positions) {
                long long sum_v = seg.query(i, n);
                long long v_i = seg.point_query(i);
                long long sign = (i % 2 == 0) ? 1 : -1;
                S_F[i] = sign * v_i * sum_v;
            }
            
            // Then flip
            for (int i : positions) {
                seg.flip(i, n);
            }
        }
    }
    
    // Compute answer
    long long term1 = 0;
    for (int i = 1; i <= n; ++i) {
        term1 += (long long)i * (n - i + 1);
    }
    
    long long term2 = 0;
    for (int i = 1; i <= n; ++i) {
        term2 += S_E[i] * S_F[i];
    }
    
    long long answer = (term1 + term2) / 2;
    cout << answer << "\n";
    
    return 0;
}
