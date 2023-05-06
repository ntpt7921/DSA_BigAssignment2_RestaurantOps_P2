#ifndef HUFFMANCODING_HUFFMANTREE_HPP
#define HUFFMANCODING_HUFFMANTREE_HPP

#include "HuffmanCoding/Heap.hpp"
#include "HuffmanCoding/HuffmanTreeNode.hpp"
#include "Queue/QueueLFCO.hpp"
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
struct CompareTreeByFreqWithASCIIFallback
{
    bool operator()(HuffTree<E> *t1, HuffTree<E> *t2) const
    {
        if (t1->root()->weight() != t2->root()->weight())
            return t1->root()->weight() < t2->root()->weight();
        else
        {
            // if both is leaf node, compare content by data
            if (t1->root()->isLeaf() && t2->root()->isLeaf())
            {
                auto l1 = dynamic_cast<LeafNode<E> *>(t1->root());
                auto l2 = dynamic_cast<LeafNode<E> *>(t2->root());
                return l1->val() < l2->val();
            }
            // else both node is considered equiv
            else
                return false;
        }
    }
};

// Build a Huffman tree from a collection of frequencies
template <typename E>
HuffTree<E> *buildHuff(const std::vector<HuffTree<E> *> treeList)
{
    QueueLFCO<HuffTree<E> *, CompareTreeByFreqWithASCIIFallback<E>> forest(treeList);

    HuffTree<char> *temp1, *temp2, *temp3;
    if (treeList.size() > 0)
        temp3 = treeList.front();

    while (forest.size() > 1)
    {
        temp1 = forest.peek();
        forest.pop();
        temp2 = forest.peek();
        forest.pop();
        temp3 = new HuffTree<E>(temp1, temp2);

        forest.push(temp3);  // Put the new tree back on list

        delete temp1;
        delete temp2;
    }

    return temp3;
}

};  // namespace details

#endif  // !HUFFMANCODING_HUFFMANTREE_HPP
