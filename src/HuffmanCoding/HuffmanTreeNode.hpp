#ifndef HUFFMANCODING_HUFFMANTREENODE_HPP
#define HUFFMANCODING_HUFFMANTREENODE_HPP

namespace details
{

template <typename E>
class HuffNode
{
public:
    virtual ~HuffNode() { }

    virtual int weight() = 0;
    virtual bool isLeaf() = 0;
};

template <typename E>
class LeafNode : public HuffNode<E>
{
private:
    E it;     // Value
    int wgt;  // Weight

public:
    LeafNode(const E &val, int freq)
    {
        it = val;
        wgt = freq;
    }

    ~LeafNode() { }

    int weight() { return wgt; }

    bool isLeaf() { return true; }

    E val() { return it; }
};

template <typename E>
class InternalNode : public HuffNode<E>
{
private:
    HuffNode<E> *lc;  // Left child
    HuffNode<E> *rc;  // Right child
    int wgt;          // Subtree weight

public:
    InternalNode(HuffNode<E> *l, HuffNode<E> *r)
    {
        wgt = l->weight() + r->weight();
        lc = l;
        rc = r;
    }

    ~InternalNode() { }

    int weight() { return wgt; }

    bool isLeaf() { return false; }

    HuffNode<E> *left() const { return lc; }

    void setLeft(HuffNode<E> *b) { lc = (HuffNode<E> *) b; }

    HuffNode<E> *right() const { return rc; }

    void setRight(HuffNode<E> *b) { rc = (HuffNode<E> *) b; }
};

};  // namespace details

#endif  // !HUFFMANCODING_HUFFMANTREENODE_HPP
