![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)

# Red Black Tree

![rb_tree_example](img/tree.png)

## General information

### Red-Black Tree Definition

A **Red-Black Tree** is a self-balancing binary search tree that maintains specific properties to ensure efficient operations. It is characterized by the following invariants:

1. **Node Coloring**: Every node is either **red** or **black**
2. **Root Property**: The root node is always **black**
3. **Leaf Property**: All leaves (NIL nodes) are considered **black**
4. **Red Property**: No two consecutive red nodes can appear in any path (a red node cannot have red children)
5. **Black Height Property**: Every path from a given node to any of its descendant NIL nodes must contain the same number of black nodes

### Time Complexity

| Operation | Average Case | Worst Case |
|-----------|--------------|------------|
|  Search   |  O(log n)    |  O(log n)  |
|  Insert   |  O(log n)    |  O(log n)  |
|  Delete   |  O(log n)    |  O(log n)  |
|  Space    |  O(n)        |  O(n)      |

This implementation provides a complete Red-Black Tree with iterator support and range query capabilities.

## Comparison with `std::set`
