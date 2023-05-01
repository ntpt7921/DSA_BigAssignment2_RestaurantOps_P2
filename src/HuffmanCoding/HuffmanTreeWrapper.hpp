#ifndef HUFFMANCODING_HUFFMANTREEWRAPPER_HPP
#define HUFFMANCODING_HUFFMANTREEWRAPPER_HPP

#include "HuffmanCoding/HuffmanTree.hpp"
#include "HuffmanCoding/HuffmanTreeNode.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename T>
class HuffmanTreeWrapper
{
protected:
    bool hasGeneratedCoding;
    details::HuffTree<T> *huffTree;
    std::unordered_map<T, std::string> dict;

    // helper function
    void traverseTreeAndExtractCoding(details::HuffNode<T> *currNode, std::string &path);

public:
    HuffmanTreeWrapper();
    HuffmanTreeWrapper(const std::vector<std::pair<T, std::size_t>> &elemAndFreqs);
    ~HuffmanTreeWrapper();

    void calculateCoding(const std::vector<std::pair<T, std::size_t>> &elemAndFreqs);
    std::string getCoding(const T &value);
    void clear();
};

template <typename T>
HuffmanTreeWrapper<T>::HuffmanTreeWrapper() : hasGeneratedCoding(false), huffTree(nullptr)
{
}

template <typename T>
HuffmanTreeWrapper<T>::HuffmanTreeWrapper(
    const std::vector<std::pair<T, std::size_t>> &elemAndFreqs)
    : hasGeneratedCoding(false), huffTree(nullptr)
{
    calculateCoding(elemAndFreqs);
    hasGeneratedCoding = true;
}

template <typename T>
HuffmanTreeWrapper<T>::~HuffmanTreeWrapper()
{
    clear();
}

template <typename T>
void HuffmanTreeWrapper<T>::traverseTreeAndExtractCoding(details::HuffNode<T> *currNode,
                                                         std::string &path)
{
    if (currNode == nullptr)
        return;

    if (currNode->isLeaf())
    {
        auto node = dynamic_cast<details::LeafNode<T> *>(currNode);
        this->dict[node->val()] = path;
    }
    else
    {
        auto node = dynamic_cast<details::InternalNode<T> *>(currNode);

        path.push_back('0');
        traverseTreeAndExtractCoding(node->left(), path);
        path.pop_back();

        path.push_back('1');
        traverseTreeAndExtractCoding(node->right(), path);
        path.pop_back();
    }
}

template <typename T>
void HuffmanTreeWrapper<T>::calculateCoding(
    const std::vector<std::pair<T, std::size_t>> &elemAndFreqs)
{
    clear();
    details::HuffTree<T> **treeArr = new details::HuffTree<T> *[elemAndFreqs.size()];

    for (std::size_t i = 0; i < elemAndFreqs.size(); i++)
        treeArr[i] = new details::HuffTree<T>(elemAndFreqs[i].first, elemAndFreqs[i].second);

    huffTree = details::buildHuff(treeArr, elemAndFreqs.size());

    std::string tempPath;
    traverseTreeAndExtractCoding(huffTree->root(), tempPath);

    hasGeneratedCoding = true;
    delete[] treeArr;
}

template <typename T>
std::string HuffmanTreeWrapper<T>::getCoding(const T &value)
{
    if (!hasGeneratedCoding)
        return "";

    auto foundAt = dict.find(value);
    if (foundAt != dict.end())
        return foundAt->second;

    return "";
}

template <typename T>
void HuffmanTreeWrapper<T>::clear()
{
    if (hasGeneratedCoding)
    {
        hasGeneratedCoding = false;
        dict.clear();
        delete huffTree;
    }
}

#endif  // !HUFFMANCODING_HUFFMANTREEWRAPPER_HPP
