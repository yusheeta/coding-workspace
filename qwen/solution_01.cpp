// Qwen attempt 1 (real output of Qwen3.7-Plus on chat.qwen.ai, thinking off, code interpreter active).
// Outcome: FAILED. Correct on tests 1-5 and 11, time limit exceeded on tests 6-10 and 12 (O(n^2)).
// Kept verbatim below.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // For each position i (0-indexed), we count the number of blocks [l, r]
    // containing i where book i is steady.
    // Book i is steady in block [l, r] iff:
    //   E(l, i) == F(r, i)
    // where:
    //   E(l, i) = (l + C(l, i)) % 2, C(l, i) = |{j in [l, i-1] : a[j] <= a[i]}|
    //   F(r, i) = (i + D(r, i)) % 2, D(r, i) = |{j in [i+1, r] : a[j] < a[i]}|
    //
    // We compute e_counts[i][v] = number of l in [0, i] with E(l, i) = v
    // and f_counts[i][v] = number of r in [i, n-1] with F(r, i) = v
    // Contribution from i = e_counts[i][0] * f_counts[i][0] + e_counts[i][1] * f_counts[i][1]

    // To compute e_counts efficiently:
    // E(l+1, i) = E(l, i) XOR (a[l] > a[i])
    // E(0, i) = |{j in [0, i-1] : a[j] <= a[i]}| % 2
    //
    // We process positions from left to right. For each position i, we need to know
    // how many l in [0, i] have E(l, i) = 0 or 1.
    //
    // Key insight: E(l, i) depends on the parity of the number of elements > a[i] in [0, l-1].
    // Let's define for each position i:
    //   G(l, i) = |{j in [0, l-1] : a[j] > a[i]}| % 2
    // Then E(l, i) = E(0, i) XOR G(l, i).
    // And E(0, i) = |{j in [0, i-1] : a[j] <= a[i]}| % 2.
    //
    // Note that |{j in [0, i-1] : a[j] <= a[i]}| + |{j in [0, i-1] : a[j] > a[i]}| = i.
    // So E(0, i) = (i - |{j in [0, i-1] : a[j] > a[i]}|) % 2 = (i % 2) XOR G(i, i).
    //
    // Thus E(l, i) = (i % 2) XOR G(i, i) XOR G(l, i).
    //
    // G(l, i) = |{j in [0, l-1] : a[j] > a[i]}| % 2.
    // As l increases, G flips when a[l-1] > a[i].
    //
    // To count e_counts efficiently, we can use a BIT indexed by values.
    // Process positions from left to right. For each position i, we query the BIT
    // to find information about elements > a[i].
    //
    // Actually, let's think differently. We need to count, for each i, how many l in [0, i]
    // have a specific parity of "number of elements > a[i] in [0, l-1]".
    //
    // This is equivalent to: as we scan from l=0 to l=i, track the running parity of
    // "elements seen so far that are > a[i]". Count how many times this parity equals
    // a target value.
    //
    // The challenge is that the threshold a[i] varies.
    //
    // Alternative approach: Use coordinate compression and process in order of values.
    // But the parity depends on positions, not just values.
    //
    // Let me try a different decomposition.
    // 
    // Total answer = sum over all blocks [l,r] of (number of steady books)
    //              = sum over all positions i of (number of blocks containing i where i is steady)
    //
    // For position i, the condition is:
    // l + C(l,i) + D(r,i) ≡ i (mod 2)
    //
    // Let's define:
    //   left_part(l, i) = (l + C(l, i)) % 2
    //   right_part(r, i) = (i + D(r, i)) % 2
    //
    // Condition: left_part(l, i) == right_part(r, i)
    //
    // For fixed i, left_part depends only on l, right_part depends only on r.
    // So we can count independently.
    //
    // left_part(l, i) = (l + |{j in [l, i-1] : a[j] <= a[i]}|) % 2
    // As l goes from 0 to i:
    //   left_part(0, i) = |{j in [0, i-1] : a[j] <= a[i]}| % 2
    //   left_part(l+1, i) = left_part(l, i) XOR (a[l] > a[i])
    //
    // right_part(r, i) = (i + |{j in [i+1, r] : a[j] < a[i]}|) % 2
    // As r goes from i to n-1:
    //   right_part(i, i) = i % 2
    //   right_part(r+1, i) = right_part(r, i) XOR (a[r+1] < a[i])
    //
    // To compute these efficiently, I'll use a BIT-based approach.
    // 
    // For left_part: Process positions from left to right. Maintain a BIT that tracks
    // positions. For position i, query how many positions j < i have a[j] > a[i].
    // But we need the parity distribution, not just the count.
    //
    // Hmm, let me think about this more carefully.
    //
    // Actually, I realize there might be a simpler O(n log n) approach using the observation
    // that we only care about parity.
    //
    // Let me implement an O(n^2) solution first to verify correctness, then optimize.
    
    // For now, let's implement the O(n^2) solution which should work for small n.
    // But n can be up to 3*10^5, so we need O(n log n).
    
    // Let me think about the efficient solution more carefully.
    //
    // Key insight: We can decompose the problem by processing elements in a specific order
    // and using a BIT to maintain parity information.
    //
    // For left_part(l, i):
    // E(l, i) = (l + C(l, i)) % 2 where C(l, i) = |{j in [l, i-1] : a[j] <= a[i]}|
    //
    // E(l+1, i) = E(l, i) XOR (a[l] > a[i])
    //
    // So E(l, i) = E(0, i) XOR (XOR of (a[j] > a[i]) for j in [0, l-1])
    //
    // Let H(l, i) = XOR of (a[j] > a[i]) for j in [0, l-1] = |{j in [0, l-1] : a[j] > a[i]}| % 2
    //
    // E(l, i) = E(0, i) XOR H(l, i)
    //
    // E(0, i) = |{j in [0, i-1] : a[j] <= a[i]}| % 2
    //         = (i - |{j in [0, i-1] : a[j] > a[i]}|) % 2
    //         = (i % 2) XOR H(i, i)
    //
    // So E(l, i) = (i % 2) XOR H(i, i) XOR H(l, i)
    //
    // We want to count how many l in [0, i] have E(l, i) = 0.
    // E(l, i) = 0 iff H(l, i) = (i % 2) XOR H(i, i)
    //
    // H(l, i) = |{j in [0, l-1] : a[j] > a[i]}| % 2
    //
    // As l goes from 0 to i, H(l, i) starts at 0 and flips each time a[l-1] > a[i].
    //
    // To count efficiently, note that H(l, i) depends on which elements in [0, l-1] are > a[i].
    // If we process positions from left to right and maintain a BIT indexed by compressed values,
    // we can track this.
    //
    // But the issue is that we need the distribution of H values for all l, not just H(i, i).
    //
    // NEW IDEA: Let's think about the problem differently.
    //
    // For each position i, define:
    //   greater_before[i] = |{j < i : a[j] > a[i]}|
    //   less_after[i] = |{j > i : a[j] < a[i]}|
    //
    // These can be computed using a BIT in O(n log n).
    //
    // But we need more than just the total counts; we need the parity distribution as we vary l and r.
    //
    // Let me think about whether there's a closed-form formula or a simpler characterization.
    //
    // Actually, I recall that for problems involving parity of displacements after sorting,
    // there's often a relationship with inversions.
    //
    // Let me try to implement a solution based on the following observation:
    // For position i, the number of blocks where it's steady can be computed by considering
    // the parity of certain counts.
    //
    // After careful analysis, I believe the key is to use a BIT to maintain prefix parities.
    //
    // Let me implement an efficient solution:

    // Coordinate compression
    vector<int> sorted_a = a;
    sort(sorted_a.begin(), sorted_a.end());
    sorted_a.erase(unique(sorted_a.begin(), sorted_a.end()), sorted_a.end());
    int m = sorted_a.size();
    
    auto get_rank = [&](int val) {
        return lower_bound(sorted_a.begin(), sorted_a.end(), val) - sorted_a.begin();
    };
    
    vector<int> rank_a(n);
    for (int i = 0; i < n; ++i) {
        rank_a[i] = get_rank(a[i]);
    }
    
    // BIT for counting
    struct BIT {
        int n;
        vector<int> tree;
        BIT(int n) : n(n), tree(n + 1, 0) {}
        void update(int i, int delta) {
            for (++i; i <= n; i += i & (-i))
                tree[i] += delta;
        }
        int query(int i) { // sum [0, i]
            int s = 0;
            for (++i; i > 0; i -= i & (-i))
                s += tree[i];
            return s;
        }
        int query_range(int l, int r) { // sum [l, r]
            if (l > r) return 0;
            return query(r) - (l > 0 ? query(l - 1) : 0);
        }
    };
    
    // Compute e_counts and f_counts for each position
    // e_counts[i][v] = number of l in [0, i] with E(l, i) = v
    // f_counts[i][v] = number of r in [i, n-1] with F(r, i) = v
    
    // For efficiency, let's compute these using the recurrence relations.
    // E(l+1, i) = E(l, i) XOR (a[l] > a[i])
    // F(r+1, i) = F(r, i) XOR (a[r+1] < a[i])
    
    // To compute e_counts[i], we need to know the sequence of flips for E.
    // The flips occur at positions l where a[l] > a[i].
    
    // Similarly for f_counts[i], flips occur at positions r where a[r+1] < a[i].
    
    // For an efficient solution, I'll use the following approach:
    // 1. Precompute for each position i, the number of elements > a[i] before each position.
    // 2. Use this to determine the parity distribution.
    
    // Actually, let me think about this more carefully.
    // 
    // For position i, as l goes from 0 to i:
    // E(l, i) starts at E(0, i) and flips at positions where a[l] > a[i].
    // 
    // Let positions_greater = {l in [0, i-1] : a[l] > a[i]}, sorted.
    // If there are k such positions, then E flips k times.
    // 
    // The number of l with E(l, i) = 0 depends on:
    // - E(0, i)
    // - The positions of the flips
    // 
    // Specifically, if the flip positions are p_1 < p_2 < ... < p_k:
    // - For l in [0, p_1]: E = E(0, i)
    // - For l in [p_1+1, p_2]: E = E(0, i) XOR 1
    // - For l in [p_2+1, p_3]: E = E(0, i)
    // - ...
    // - For l in [p_k+1, i]: E = E(0, i) XOR (k % 2)
    // 
    // Wait, that's not quite right. Let me reconsider.
    // 
    // E(l+1, i) = E(l, i) XOR (a[l] > a[i])
    // So E flips at position l+1 if a[l] > a[i].
    // 
    // Starting from E(0, i):
    // - E(0, i) is the initial value.
    // - E(1, i) = E(0, i) XOR (a[0] > a[i])
    // - E(2, i) = E(1, i) XOR (a[1] > a[i])
    // - ...
    // 
    // So E(l, i) = E(0, i) XOR (XOR of (a[j] > a[i]) for j in [0, l-1])
    //            = E(0, i) XOR H(l, i)
    // 
    // where H(l, i) = |{j in [0, l-1] : a[j] > a[i]}| % 2
    // 
    // H(0, i) = 0
    // H(l+1, i) = H(l, i) XOR (a[l] > a[i])
    // 
    // To count how many l in [0, i] have E(l, i) = 0:
    // E(l, i) = 0 iff H(l, i) = E(0, i)
    // 
    // So we need to count how many l in [0, i] have H(l, i) = E(0, i).
    // 
    // H(l, i) starts at 0 and flips each time a[l-1] > a[i].
    // 
    // If there are k elements > a[i] in [0, i-1], then H flips k times.
    // The positions of flips are the positions l where a[l-1] > a[i], i.e., l-1 is a position with a[l-1] > a[i].
    // So flips occur at l = j+1 for each j in [0, i-1] with a[j] > a[i].
    // 
    // Let these positions be q_1 < q_2 < ... < q_k where q_m = j_m + 1 and a[j_m] > a[i].
    // 
    // Then:
    // - For l in [0, q_1 - 1] = [0, j_1]: H = 0
    // - For l in [q_1, q_2 - 1] = [j_1 + 1, j_2]: H = 1
    // - For l in [q_2, q_3 - 1] = [j_2 + 1, j_3]: H = 0
    // - ...
    // - For l in [q_k, i]: H = k % 2
    // 
    // Wait, let me be more careful. H(l, i) = |{j in [0, l-1] : a[j] > a[i]}| % 2.
    // 
    // For l = 0: H(0, i) = 0 (empty set).
    // For l = 1: H(1, i) = |{j in [0, 0] : a[j] > a[i]}| % 2 = (a[0] > a[i]) % 2.
    // For l = 2: H(2, i) = |{j in [0, 1] : a[j] > a[i]}| % 2.
    // ...
    // For l = i: H(i, i) = |{j in [0, i-1] : a[j] > a[i]}| % 2.
    // 
    // So H(l, i) changes when we include a new position l-1 in the range [0, l-1].
    // H(l, i) = H(l-1, i) XOR (a[l-1] > a[i]).
    // 
    // Starting from H(0, i) = 0:
    // - H(1, i) = 0 XOR (a[0] > a[i]) = (a[0] > a[i])
    // - H(2, i) = H(1, i) XOR (a[1] > a[i])
    // - ...
    // 
    // So H(l, i) flips at position l if a[l-1] > a[i].
    // 
    // The flip positions are l = 1, 2, ..., i where a[l-1] > a[i].
    // Equivalently, the flip positions are l = j+1 for j in [0, i-1] where a[j] > a[i].
    // 
    // Let's say there are k such positions, at l-values q_1 < q_2 < ... < q_k.
    // Then:
    // - For l in [0, q_1 - 1]: H = 0
    // - For l in [q_1, q_2 - 1]: H = 1
    // - For l in [q_2, q_3 - 1]: H = 0
    // - ...
    // - For l in [q_k, i]: H = k % 2
    // 
    // The lengths of these segments are:
    // - Segment 0: q_1 - 0 = q_1 positions (l from 0 to q_1 - 1)
    // - Segment 1: q_2 - q_1 positions (l from q_1 to q_2 - 1)
    // - ...
    // - Segment k: i - q_k + 1 positions (l from q_k to i)
    // 
    // To count how many l have H(l, i) = v:
    // Sum the lengths of segments where H = v.
    // 
    // This requires knowing the exact positions q_1, ..., q_k.
    // 
    // For an efficient solution, I need to avoid enumerating these positions for each i.
    // 
    // NEW IDEA: Maybe I can compute the answer using a different decomposition that doesn't
    // require knowing the exact flip positions.
    // 
    // Let me think about the total contribution from position i:
    // contrib_i = e0_i * f0_i + e1_i * f1_i
    // 
    // where e0_i + e1_i = i + 1 and f0_i + f1_i = n - i.
    // 
    // So contrib_i = e0_i * f0_i + (i + 1 - e0_i) * (n - i - f0_i)
    //              = e0_i * f0_i + (i + 1)(n - i) - (i + 1)f0_i - e0_i(n - i) + e0_i * f0_i
    //              = 2 * e0_i * f0_i + (i + 1)(n - i) - (i + 1)f0_i - e0_i(n - i)
    // 
    // This doesn't seem to simplify things.
    // 
    // Let me try a different approach. Maybe I can compute the answer by considering
    // the contribution of each pair of positions.
    // 
    // Actually, I think the key insight is that we can use a BIT to maintain the parity
    // information as we process positions.
    // 
    // Let me implement an O(n log n) solution using the following approach:
    // 1. For each position i, compute e0_i and f0_i using BIT queries.
    // 2. Sum up the contributions.
    // 
    // For e0_i:
    // e0_i = number of l in [0, i] with E(l, i) = 0
    //      = number of l in [0, i] with H(l, i) = E(0, i)
    // 
    // H(l, i) = |{j in [0, l-1] : a[j] > a[i]}| % 2
    // 
    // As l goes from 0 to i, H(l, i) starts at 0 and flips at positions where a[l-1] > a[i].
    // 
    // If I process positions from left to right and maintain a BIT that tracks positions
    // with a[j] > current threshold, I can query the BIT to find the parity.
    // 
    // But the threshold a[i] varies, so I need a different approach.
    // 
    // Actually, let me think about this problem using offline queries.
    // 
    // For each position i, I need to know:
    // - How many l in [0, i] have |{j in [0, l-1] : a[j] > a[i]}| % 2 = E(0, i)
    // 
    // This is equivalent to:
    // - How many l in [0, i] have |{j in [0, l-1] : a[j] > a[i]}| % 2 = (i % 2) XOR H(i, i)
    // 
    // where H(i, i) = |{j in [0, i-1] : a[j] > a[i]}| % 2.
    // 
    // Hmm, this is getting complex. Let me try a different approach.
    // 
    // After careful thought, I believe the most efficient approach is to use a BIT
    // indexed by compressed values and process positions in a specific order.
    // 
    // For now, let me implement an O(n sqrt(n)) solution using Mo's algorithm or similar.
    // Actually, that might be too slow.
    // 
    // Let me try to implement an O(n log n) solution based on the following observation:
    // 
    // For position i, the condition E(l, i) = F(r, i) separates l and r.
    // So I can compute e_counts and f_counts independently.
    // 
    // For e_counts[i]:
    // E(l, i) = (l + C(l, i)) % 2 where C(l, i) = |{j in [l, i-1] : a[j] <= a[i]}|
    // 
    // As l goes from 0 to i, E(l, i) changes based on a[l] > a[i].
    // 
    // If I maintain a BIT indexed by compressed values, I can query how many elements
    // in [0, l-1] are > a[i]. But I need this for all l, not just a specific l.
    // 
    // Hmm, this is still challenging.
    // 
    // Let me try a different decomposition. Maybe I can think about the problem in terms
    // of counting inversions with specific parities.
    // 
    // Actually, I think I need to step back and reconsider the problem.
    // 
    // After more thought, I believe the key is to use the fact that we're working mod 2
    // and find a way to batch process the queries.
    // 
    // For now, let me implement an O(n^2) solution and see if it passes for small inputs.
    // If not, I'll optimize.
    
    long long total = 0;
    
    // O(n^2) solution
    for (int i = 0; i < n; ++i) {
        int h = a[i];
        
        // Compute e_counts
        int e_counts[2] = {0, 0};
        int c = 0; // C(l, i)
        for (int l = 0; l <= i; ++l) {
            if (l == 0) {
                c = 0;
                for (int j = 0; j < i; ++j) {
                    if (a[j] <= h) c++;
                }
            } else {
                if (a[l - 1] <= h) c--;
            }
            int e = (c + l) % 2;
            e_counts[e]++;
        }
        
        // Compute f_counts
        int f_counts[2] = {0, 0};
        int d = 0; // D(r, i)
        for (int r = i; r < n; ++r) {
            if (r == i) {
                d = 0;
            } else {
                if (a[r] < h) d++;
            }
            int f = (i + d) % 2;
            f_counts[f]++;
        }
        
        total += (long long)e_counts[0] * f_counts[0] + (long long)e_counts[1] * f_counts[1];
    }
    
    cout << total << "\n";
    
    return 0;
}
