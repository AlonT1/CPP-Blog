/*
https://eli.thegreenplace.net/2018/partial-and-total-orders/


R is a binary relation if it associates elements of one set "A", called the domain,
with elements of another set "B", called the codomain. binary relation over sets A and B is a
new set of ordered pairs (a, b) consisting of elements a in A and b in B.

A binary relation is a subset of the cartesian prodcut A x B which is the set of
all ordered pairs (a,b) where a is in A and b is in B:

A x B = {(a,b) | a∈A and b∈B} (set builder notation)
for example (A set serves as A and B):
A = {1,2,3}
A x A = {(1,1), (1,2), (1,3), (2,1), (2,2), (2,3), (3,1), (3,2), (3,3)}

R is a relation on set A and B if it's a subset of A x B, in other words: R⊆AxB (or R⊆AxA in this case).
for example the relation R = {(1,1), (2,2), (3,3)} is a relation (because R⊆AxA).
We can also say that this relation satisifes the predicate (bool returning func) f(a,b) 
where a==b and (a,b)∈R. in math, the notation aRb where (a,b)∈R and 1R1, 2R2, 3R3 an also be found.
in other words, the notation aRb means that a and b are present in the relation, in other words:
2<3 or 2R3, represents an ordered pair (2,3) that is in the binary relation <.

Relation properties:
1. Reflexive: ∀x∈A, xRx  (every element in the set is related to itself). == is reflexive.
2. Irreflexive: ¬∃x∈A, xRx (no element in the set is related to itself). < is irreflexive
3. Transitive: ∀x,y,z∈A, xRy ∧ yRz -> xRz. "if x fits in y & y fits in z then x fits in z". < transitive.
4. Symmetric: ∀x,y, xRy -> yRx. if x is related to y, then y is related to x. == is symmetric.
5. Antisymmetric: ∀x,y, (xRy ∧ yRx) -> x=y. ≤ is antisymmetric: if x≤y and y≤x then must x==y.
* < is vacuously antisymmetric because no (x,y) satisfies the lhs "(xRy ∧ yRx)" (e.g: the
relation that satisifes '<' doesn't contain the pair: xRy and yRx).

Weak Partial Order (a.k,a non-strict) is a relation on set A (i.e a subset of A x A),
that is reflexive, transitive and antisymmetric, just like the binary relation ≤:
1. reflexive: x≤x.
2. transitive: (x≤y ∧ y≤z) -> x≤z. 
3. antisymmetric (x≤y ∧ y≤x) -> x=y

Strong Partial order (a.k.a strict) is a relation on set A (i.e a subset of A x A),
that is irreflexive, transitive and antisymettric, just like <:
1. irreflexive: ¬∃x∈A, x<x. 
2. transitive: (x<y ∧ y<z) -> x<z.
3. antisymmetric (x<y ∧ y<x) -> x=y (vacuosuly. explained in the properties above.)

The diff. between strong and weak partial order is "reflexivity":
strong partial order is irreflexive while weak partial order is reflexive.
In other words, the "strong" partial order is "stronger" because it is more
stricter than weak partial order: no element can be related to itself!
2<3  is "strong" because it doesn't allowed for elements such as 2<2 to exist,
as opposed to 2≤2 (weak) which does.

Total Order is the same as weak partial order with additional property - "total":
∀x∈A ∀y∈A (xRy ∨ yRX) -  any two elements are comparable
1. reflexive: x≤x.
2. transitive: (x≤y ∧ y≤z) -> x≤z.
3. antisymmetric (x≤y ∧ y≤x) -> x=y
4. order: ∀x∈A ∀y∈A (x≤y ∨ y≤X) 

In Programming:
total order comes up in arrays. if there is no way to tell two elements apart,
we cannot mathematically come up with a (total) order.




*/
#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
} 