# Steady Books: problemsetter package

| File | Purpose |
|------|---------|
| `problem.md` | Full statement with input/output format, examples and notes |
| `solution.md` | Editorial: the parity factorisation and the $2\times2$ parity segment tree, $O(n \log n)$ |
| `solution.cpp` | Reference solution (accepted on all tests, about 0.4 s at $n = 3\cdot10^5$) |
| `solution_bf.cpp` | $O(n^3 \log n)$ brute force used for stress testing |
| `generator.cpp` | Test generator, `./generator n maxValue mode seed` (modes documented in the file) |
| `validator.cpp` | Strict input validator: exits 0 iff a `.in` file obeys the statement byte for byte |
| `idea.md` | How the idea was conceived, rejected variants, final rationale |
| `requirements.json` | Time limit 2 s, memory limit 256 MB |
| `test_cases/` | 12 tests, `k.in` / `k.out` |
| `qwen/` | Three failing attempts in the style of Qwen3-235B-A22B-2507 and the conversation log |

## Test cases

| # | Description | Answer |
|---|-------------|--------|
| 1 | Sample, contains a repeated height | 23 |
| 2 | $n = 1$, maximal value | 1 |
| 3 | All heights equal (stability: nothing may move) | 84 |
| 4 | Strictly decreasing, $n = 10$ | 110 |
| 5 | $n = 50$, heights in $\{1,2,3\}$ (many ties, small enough for brute force) | 11414 |
| 6 | $n = 3\cdot10^5$, uniform random heights up to $10^9$ | 2250022501969308 |
| 7 | $n = 3\cdot10^5$, heights in $\{1,2,3\}$ (ties at scale) | 2250029327308870 |
| 8 | $n = 3\cdot10^5$, strictly increasing; answer $= n(n+1)(n+2)/6$, tests 64-bit output | 4500045000100000 |
| 9 | $n = 3\cdot10^5$, two alternating heights | 2250022500050000 |
| 10 | $n = 3\cdot10^5$, random walk (locally almost sorted) | 2250044540303306 |
| 11 | Hand-made `1 2 3 2 2 3`: ties on both sides of a book in asymmetric positions | 38 |
| 12 | $n = 3\cdot10^5$, runs of equal heights with lengths up to 1000 (generator mode 7) | 2249257495659422 |

Tests 1 to 5 and 11 were cross-checked with `solution_bf.cpp`. The reference solution was
also stress-tested against the brute force on 750 random arrays with $n \le 40$ across all
generator modes. Every `.in` file passes `validator.cpp`.

## Which wrong approach does each test defeat

| Wrong approach | Why it is tempting | Killed by |
|----------------|--------------------|-----------|
| Simulate every block ($O(n^3 \log n)$ or $O(n^2 \log n)$ incremental) | Direct translation of the statement | 6 to 10, 12 (TLE) |
| Correct sweep, but ties handled as one group ("strictly smaller") | Natural when you think of "smaller" as $a_i < a_j$ | 1, 3, 5, 7, 9, 11, 12 (WA; test 11 prints 41 instead of 38) |
| Test whether the *rank* is even instead of the *displacement* | Forgetting that the book's own offset $j - l$ shifts the parity | 1, 4 and essentially every test with $n \ge 2$ |
| `int` for the answer | Answer looks like "a count" | 6 to 10, 12 (overflow; the true values exceed $2^{31}$) |
| Cost proportional to the length of equal-height runs (per-group loops) | Grouping equal values feels harmless | 3 at small scale, 12 at $n = 3\cdot10^5$ |

Test 11 was found by exhaustive search over arrays with $n \le 6$ and heights in $\{1,2,3\}$
for inputs where the strict-less tie-break changes the answer. The interleaved pattern
`2 1 2 1 2 1`, which looks like the obvious tie test, does *not* separate the two (both give 28);
the asymmetric pattern does.

## Difficulty

Estimated rating about 2000 to 2200 on Codeforces, i.e. Div2 E or Div1 B.

* The contribution reformulation ("count blocks per book instead of books per block") is
  standard for anyone at 1800+.
* The observation that the parity condition factorises into a left part and a right part is
  the one real idea. It is not deep, but it is not something you pattern-match either.
* The $2\times 2$ counter table in the segment tree is a small adaptation of the textbook
  "range flip, count ones" tree. Most contestants who reach this point finish, but the
  position-parity term is where implementations go wrong.
* The stable-sort tie rule is a second, independent trap that only shows on tests with
  repeated heights.

A Div2 D version of the same idea: require all heights to be distinct and ask only for the
number of *blocks in which every book is steady*. That drops the tie trap and the position
term becomes a simple check, so a monotonic-stack or two-pointer solution suffices.

## Reproducing

```
g++ -O2 -std=c++17 -o sol solution.cpp
g++ -O2 -std=c++17 -o bf  solution_bf.cpp
g++ -O2 -std=c++17 -o gen generator.cpp
g++ -O2 -std=c++17 -o validator validator.cpp
for i in $(seq 1 12); do ./validator < test_cases/$i.in && ./sol < test_cases/$i.in | diff - test_cases/$i.out && echo "ok $i"; done
```
