# How the problem was developed

## Goal

I wanted a Div1 B / Div2 E level problem whose statement is very elementary
(sorting a block, parity of a distance) but whose efficient solution needs one real
observation plus a data structure that has to be adapted slightly, not copied.
Problems whose solutions collapse into a well-known template ("count subarrays with
property X via monotonic stack") are easily pattern-matched; I wanted the
pattern-matching to lead to a *wrong* template.

## Starting point: contribution of one element over all subarrays

The generic engine "sum over all subarrays = sum over elements of the number of
subarrays in which the element has property P" is powerful only when P can be
split into an independent left-endpoint part and right-endpoint part. Properties
about the *rank* of an element inside a subarray have this shape: the number of
smaller elements in $[l, r]$ is a difference of prefix counts, and modulo 2 a
difference is an XOR, which factorises perfectly.

The first concrete idea was therefore:

> *Variant A.* Sum, over all subarrays, of the alternating sum
> $b_1 - b_2 + b_3 - \dots$ of the sorted subarray.

This is solvable by exactly the sweep-by-value / suffix-flip technique. I rejected
it for two reasons: (1) "alternating sum of a sorted sequence" is a phrase that
appears in several existing problems (subsequence versions on LeetCode, related
Codeforces tasks), so it is not search-proof; (2) the sign depends on rank only,
so the segment tree is the textbook "range flip, count ones" tree with nothing to
adapt.

## Adding the position

To make the condition involve both *where the element is* and *what its rank is*,
I compared the rank with the element's offset inside the subarray. The difference
of the two is precisely the displacement caused by sorting, which has a natural
story ("the book moves by an even number of slots" / "stays on a slot of the same
colour"). This gave:

> *Variant B.* Over all subarrays, count elements whose displacement under sorting
> is even.

Now the parity condition is $C_j(r) \equiv C_j(l-1) + (j - l) \pmod 2$. It still
factorises, but the left-side query must combine the parity of the prefix count with
the parity of the *position* $l - 1$. The segment tree must therefore keep a
$2\times 2$ table (count parity $\times$ position parity) per node instead of a
single counter. This is a small but genuine adaptation, and the place where a hasty
solution goes wrong. I also verified there is no closed form: the number of steady
elements is $\frac{1}{2}\bigl(k + \sum_j (-1)^{j + \sigma(j)}\bigr)$ where $\sigma$
is the sorting permutation, and $\sum_j (-1)^{j+\sigma(j)}$ has no shortcut in terms
of simple statistics of the subarray.

## Rejected variants

* **Sum of heights of steady books** instead of their number. Requires a modulus
  (values up to $10^9$ times $\sim 4.5\cdot 10^{15}$ triples) and adds nothing
  algorithmically. Counting keeps the output exact in 64 bits and the statement cleaner.
* **Displacement divisible by 3** (or a given $k$). For $k = 3$ the node table becomes
  $3\times 3$ with a rotation lazy tag. That is doable, but the statement loses its natural
  "same colour slot" story. For general $k$ the technique breaks down entirely.
* **Per-length output** ("for every block length $k$ print the total"). The pairs
  $(l, r)$ would be coupled by $r - l + 1 = k$, destroying the factorisation; this
  jumps far past Div1 B difficulty.
* **Distinct heights only.** Would remove the stability subtlety, which is one of the
  two intended traps and is what separates a careful solution from a careless one.
* **Point updates between queries.** The sweep is ordered by value, so updates would
  require a completely different (much heavier) approach.

## Why stable sorting

Allowing equal heights makes the total order $(a_i, i)$ essential. A solution that
processes a group of equal values "all queries first, then all inserts" (the natural
thing to do when one thinks of "strictly smaller") is wrong: with stable sorting an
equal element on the left counts as smaller and an equal element on the right does
not. This asymmetry is easy to miss, is not caught by tests with distinct values, and
was deliberately kept. Test 3 (all equal) and tests 5, 7, 9 (many repeated values)
target it; the first sample also contains a repeated height so that a contestant
can notice the rule on the example.

## Final formulation rationale

* $n \le 3\cdot 10^5$ separates $O(n \log n)$ from $O(n^2)$ and $O(n \sqrt n \log n)$
  approaches cleanly; the reference solution uses about a fifth of the limit.
* Values up to $10^9$ force coordinate handling through sorting rather than
  counting arrays, and keep the "many duplicates" and "all distinct" regimes both
  reachable.
* The answer fits in a signed 64-bit integer, so no modulus is needed and the
  strictly increasing test has a checkable closed form $n(n+1)(n+2)/6$.
