# Steady Books: Editorial

## Reformulating "steady"

Fix a block $[l, r]$ and a book at position $j \in [l, r]$. Because the sort is
stable, the books that end up to the left of book $j$ inside the block are
exactly the books $i \in [l, r]$ that are *smaller* than $j$ in the total order

$$ (a_i, i) \prec (a_j, j) \iff a_i < a_j \ \text{or}\ (a_i = a_j \text{ and } i < j). $$

Let $S$ be their number. Book $j$ moves from position $j$ to position $l + S$, so its
displacement is

$$ d = (l + S) - j = S - (j - l). $$

Book $j$ is steady iff $d$ is even, i.e. iff $S \equiv j - l \pmod 2$.

Define the prefix counter $C_j(x) = \#\{\, i \le x : (a_i, i) \prec (a_j, j) \,\}$.
Then $S = C_j(r) - C_j(l-1)$, and writing $x = l - 1$ the condition becomes

$$ C_j(r) \equiv C_j(x) + (j - 1 - x) \pmod 2 \qquad (x \in [0, j-1],\ r \in [j, n]). $$

The left-hand side depends only on $r$, the right-hand side only on $x$. Hence, for
a fixed $j$, the number of blocks in which book $j$ is steady is

$$ \sum_{b \in \{0,1\}} L_b \cdot R_b, \qquad
R_b = \#\{ r \in [j, n] : C_j(r) \bmod 2 = b \}, \quad
L_b = \#\{ x \in [0, j-1] : (C_j(x) + (j-1) + x) \bmod 2 = b \}. $$

(We used $j-1-x \equiv (j-1) + x \pmod 2$.) The answer is the sum of this quantity
over all $j$. This is the key observation: the parity condition *factorises* into
an independent left part and right part, so we never have to enumerate blocks.

## Computing all $C_j$ with one sweep

$C_j$ is the prefix-count function of the set $P_j = \{\, i : (a_i, i) \prec (a_j, j) \,\}$.
Process the books in increasing order of the key $(a_j, j)$. When we reach book $j$,
the set of already processed books is exactly $P_j$. "Inserting" a position $p$ into
the set adds $1$ to $C(x)$ for all $x \ge p$, i.e. **flips the parity of $C$ on the
suffix $[p, n]$**.

So we need a data structure over the positions $x \in \{0, 1, \dots, n\}$ that supports

* flip the bit $C(x) \bmod 2$ for all $x$ in a range,
* count, in a range, the positions with $C(x) \bmod 2 = b$ (for $R_b$),
* count, in a range, the positions with $(C(x) + x) \bmod 2 = b'$ (for $L_b$, with
  $b' = b \oplus ((j-1) \bmod 2)$).

A lazy segment tree does this. Each node stores four counters
`cnt[p][q]` = number of positions in the node's range with parity of $C$ equal to $p$
and parity of the position $x$ equal to $q$. A range flip swaps the rows
`cnt[0][*]` and `cnt[1][*]` and toggles a lazy flag; the position parity $q$ never
changes. Both query types are sums of the appropriate counters over a range.

Algorithm:

1. Sort the indices $1..n$ by $(a_j, j)$.
2. Build the tree over $x \in [0, n]$ with all $C(x) = 0$.
3. For each $j$ in sorted order:
   * query the counters over $[j, n]$ to obtain $R_0, R_1$;
   * query the counters over $[0, j-1]$ to obtain $L_0, L_1$
     (`L_b = sum over q of cnt[b ^ ((j-1)&1) ^ q][q]`);
   * add $L_0 R_0 + L_1 R_1$ to the answer;
   * flip the suffix $[j, n]$ (book $j$ is now "smaller" than all later books).
4. Print the answer.

## Complexity

Sorting is $O(n \log n)$; each of the $n$ steps does two range queries and one range
update on the segment tree, $O(\log n)$ each. Total $O(n \log n)$ time and $O(n)$
memory. For $n = 3\cdot 10^5$ the reference solution runs in about $0.4$ s.

The answer is at most $\sum_j j\,(n - j + 1) = \frac{n(n+1)(n+2)}{6} \approx 4.5\cdot 10^{15}$
(attained by a sorted array), so a 64-bit integer suffices and no modulus is needed.

## Pitfalls

* **Ties.** Treating equal heights as mutually "not smaller" (querying a whole group
  of equal values before inserting any of them) breaks stability: an equal book to
  the left of $j$ *must* be counted as smaller. This gives wrong answers on every
  test with repeated values (tests 1, 3, 5, 7, 9, 11, 12).
* **The position term.** It is tempting to test whether the *rank* $S$ is even;
  the correct condition compares $S$ with $j - l$. The $x$-parity counters in the
  tree exist exactly to handle this term.
* **Brute force.** Any per-block simulation is at least $\Theta(n^2)$ and cannot pass
  $n = 3\cdot 10^5$.
