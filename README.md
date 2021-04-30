# Self-balancing trees

Implemented 
* map
* multimap
* set
* multiset

with one of the following self-balancing trees
* Red-black tree
* AVL tree
* B tree (including 2-3 tree and 2-3-4 tree)

STL-like iterator pattern.

Run unit test with [GoogleTest](https://github.com/google/googletest)

## Usage
Run unit test
```
make test
```

To use the library, simply include the headers and use the following type.
```C++
map<KeyType, ValueType, Tree> 
```
