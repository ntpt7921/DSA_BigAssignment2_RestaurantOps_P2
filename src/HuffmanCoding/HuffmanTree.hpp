#ifndef HUFFMANCODING_HUFFMANTREE_HPP
#define HUFFMANCODING_HUFFMANTREE_HPP

#include "HuffmanCoding/Heap.hpp"
#include "HuffmanCoding/HuffmanTreeNode.hpp"
#include <queue>

namespace details
{

template <typename E>
class HuffTree
{
private:
    HuffNode<E> *Root;  // Tree root
public:
    HuffTree(const E &val, int freq) { Root = new LeafNode<E>(val, freq); }

    HuffTree(HuffTree<E> *l, HuffTree<E> *r)
    {
        Root = new InternalNode<E>(l->root(), r->root());

        // set this as a way to transfer ownership of node within l and r tree to the current
        // else delete will deallocated all associated node of this tree, even if the tree does
        // not own the node anymore
        l->Root = nullptr;
        r->Root = nullptr;
    }

    ~HuffTree();

    HuffNode<E> *root() { return Root; }

    int weight() { return Root->weight(); }  // Root weight
};

template <typename E>
HuffTree<E>::~HuffTree()
{
    if (root() == nullptr)
        return;

    std::queue<HuffNode<E> *> q;
    q.push(Root);

    while (!q.empty())
    {
        HuffNode<E> *currNode = q.front();
        q.pop();

        if (!currNode->isLeaf())
        {
            // currNode is InternalNode
            auto node = dynamic_cast<InternalNode<E> *>(currNode);
            if (node->left() != nullptr)
                q.push(node->left());
            if (node->right() != nullptr)
                q.push(node->right());
        }

        delete currNode;
    }
}

template <typename E>
struct minTreeComp
{
    static bool prior(HuffTree<E> *t1, HuffTree<E> *t2)
    {
        return t1->root()->weight() < t2->root()->weight();
    }
};

// Build a Huffman tree from a collection of frequencies
template <typename E>
HuffTree<E> *buildHuff(HuffTree<E> **TreeArray, int count)
{
    heap<HuffTree<E> *, minTreeComp<E>> forest(TreeArray, count, count);

    HuffTree<char> *temp1, *temp2, *temp3;
    if (count > 0)
        temp3 = TreeArray[0];

    while (forest.size() > 1)
    {
        temp1 = forest.removefirst();
        temp2 = forest.removefirst();
        temp3 = new HuffTree<E>(temp1, temp2);

        forest.insert(temp3);  // Put the new tree back on list

        delete temp1;
        delete temp2;
    }

    return temp3;
}

};  // namespace details

#endif  // !HUFFMANCODING_HUFFMANTREE_HPP
