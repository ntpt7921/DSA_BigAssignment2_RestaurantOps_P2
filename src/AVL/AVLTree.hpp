#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <queue>

template <typename K, typename T>
class AVLTree
{
public:
    enum class BalanceValue
    {
        LH = -1,
        EH = 0,
        RH = 1
    };

    class Node
    {
    private:
        K key;
        T data;
        Node *pLeft, *pRight;
        BalanceValue balance;
        friend class AVLTree<K, T>;

    public:
        Node(const K &key, const T &value)
            : key(key), data(value), pLeft(nullptr), pRight(nullptr), balance(BalanceValue::EH)
        {
        }

        ~Node() { }
    };

    void printTreeStructure()
    {
        int height = this->getHeight();
        if (this->root == nullptr)
        {
            std::cout << "NULL\n";
            return;
        }
        std::queue<Node *> q;
        q.push(root);
        Node *temp;
        int count = 0;
        int maxNode = 1;
        int level = 0;
        int space = pow(2, height);
        printNSpace(space / 2);
        while (!q.empty())
        {
            temp = q.front();
            q.pop();
            if (temp == NULL)
            {
                std::cout << " ";
                q.push(NULL);
                q.push(NULL);
            }
            else
            {
                std::cout << temp->data;
                q.push(temp->pLeft);
                q.push(temp->pRight);
            }
            printNSpace(space);
            count++;
            if (count == maxNode)
            {
                std::cout << std::endl;
                count = 0;
                maxNode *= 2;
                level++;
                space /= 2;
                printNSpace(space / 2);
            }
            if (level == height)
                return;
        }
    }

private:
    Node *root;
    std::size_t count;

protected:
    int getHeightRec(Node *node)
    {
        if (node == nullptr)
            return 0;
        int lh = this->getHeightRec(node->pLeft);
        int rh = this->getHeightRec(node->pRight);
        return (lh > rh ? lh : rh) + 1;
    }

    void printNSpace(int n)
    {
        for (int i = 0; i < n - 1; i++)
            std::cout << " ";
    }

    bool checkTreeStructureRecursive(Node *curr, int &height);

    // ultility function
    void rotateLeftToBalance(Node **curr);
    void rotateRightToBalance(Node **curr);
    void fixImbalanceLeftFromInsert(Node **curr, bool &hasBecomesHigher);
    void fixImbalanceRightFromInsert(Node **curr, bool &hasBecomesHigher);
    void insertRecursive(Node **curr, const K &key, const T &value, bool &hasBecomesHigher);

    void fixImbalanceRightFromRemove(Node **curr, bool &hasBecomesShorter);
    void fixImbalanceLeftFromRemove(Node **curr, bool &hasBecomesShorter);
    Node *removeRecursive(Node **curr, const K &key, bool &hasBecomesShorter, bool &successDelete);

    void clearRecursive(Node *curr);

public:
    AVLTree() : root(nullptr), count(0) { }

    ~AVLTree() { clear(); }

    int getHeight() { return this->getHeightRec(this->root); }

    bool checkTreeStructure();

    void insert(const K &key, const T &value);
    void remove(const K &key);
    void clear();
    void forEach(std::function<void(const T &)> printFunction);

    std::size_t size() const { return count; }
};

template <typename K, typename T>
bool AVLTree<K, T>::checkTreeStructureRecursive(Node *curr, int &height)
{
    if (curr == nullptr)
        return true;

    int leftHeight = 0;
    int rightHeight = 0;

    if (!checkTreeStructureRecursive(curr->pLeft, leftHeight))
        return false;

    // check for key ordering
    if (curr->pLeft != nullptr)
    {
        if (curr->pLeft->key > curr->key)
        {
            std::cerr << "Wrong key ordering on left subtree of node" << std::endl;
            return false;
        }
    }
    if (curr->pRight != nullptr)
    {
        if (curr->pRight->key < curr->key)
        {
            std::cerr << "Wrong key ordering on right subtree of node" << std::endl;
            return false;
        }
    }

    if (!checkTreeStructureRecursive(curr->pRight, rightHeight))
        return false;

    // now have left and right height, check for correct height and balance value
    if (std::abs(leftHeight - rightHeight) > 1)
    {
        std::cerr << "Height imbalance at node" << std::endl;
        return false;
    }
    else
    {
        int diffHeight = rightHeight - leftHeight;
        switch (diffHeight)
        {
            case -1:
                if (curr->balance != BalanceValue::LH)
                {
                    std::cerr << "Wrong balance value for current node" << std::endl;
                    return false;
                }
                break;
            case 0:
                if (curr->balance != BalanceValue::EH)
                {
                    std::cerr << "Wrong balance value for current node" << std::endl;
                    return false;
                }
                break;
            case 1:
                if (curr->balance != BalanceValue::RH)
                {
                    std::cerr << "Wrong balance value for current node" << std::endl;
                    return false;
                }
                break;
            default:
                break;
        }
    }

    height = std::max(leftHeight, rightHeight) + 1;
    return true;
}

template <typename K, typename T>
bool AVLTree<K, T>::checkTreeStructure()
{
    if (this->root == nullptr)
        return true;

    int height = 0;
    return checkTreeStructureRecursive(this->root, height);
}

template <typename K, typename T>
void AVLTree<K, T>::rotateLeftToBalance(Node **curr)
{
    Node *tempRightSub = (*curr)->pRight;
    (*curr)->pRight = tempRightSub->pLeft;
    tempRightSub->pLeft = *curr;
    *curr = tempRightSub;
}

template <typename K, typename T>
void AVLTree<K, T>::rotateRightToBalance(Node **curr)
{
    Node *tempLeftSub = (*curr)->pLeft;
    (*curr)->pLeft = tempLeftSub->pRight;
    tempLeftSub->pRight = *curr;
    *curr = tempLeftSub;
}

template <typename K, typename T>
void AVLTree<K, T>::fixImbalanceLeftFromInsert(Node **curr, bool &hasBecomesHigher)
{
    // know that there is a imbalance on the left subtree
    // and that left subtree is either LH or RH (no EH), because hasBecomesHigher is true
    Node *rootSubLeft = (*curr)->pLeft;

    if (rootSubLeft->balance == BalanceValue::EH)
    {
        std::cerr << "fixImbalanceLeft - left subtree is EH" << std::endl;
        return;
    }

    if (rootSubLeft->balance == BalanceValue::LH)
    {
        // left of left imbalance
        (*curr)->balance = BalanceValue::EH;
        rootSubLeft->balance = BalanceValue::EH;
        rotateRightToBalance(curr);
    }
    else  // rootSubLeft->balance == RH
    {
        // right of left imbalance
        Node *nodeA = *curr;
        Node *nodeB = (*curr)->pLeft;
        Node *nodeE = nodeB->pRight;

        if (nodeE->balance == BalanceValue::LH)
            nodeA->balance = BalanceValue::RH;
        else
            nodeA->balance = BalanceValue::EH;

        if (nodeE->balance == BalanceValue::RH)
            nodeB->balance = BalanceValue::LH;
        else
            nodeB->balance = BalanceValue::EH;

        nodeE->balance = BalanceValue::EH;

        rotateLeftToBalance(&(*curr)->pLeft);
        rotateRightToBalance(curr);
    }

    hasBecomesHigher = false;
}

template <typename K, typename T>
void AVLTree<K, T>::fixImbalanceRightFromInsert(Node **curr, bool &hasBecomesHigher)
{
    // know that there is a imbalance on the right subtree
    // and that right subtree is either LH or RH (no EH), because hasBecomesHigher is true
    Node *rootSubRight = (*curr)->pRight;

    if (rootSubRight->balance == BalanceValue::EH)
    {
        std::cerr << "fixImbalanceRight - right subtree is EH" << std::endl;
        return;
    }

    if (rootSubRight->balance == BalanceValue::LH)
    {
        // left of right imbalance
        Node *nodeA = *curr;
        Node *nodeB = (*curr)->pRight;
        Node *nodeE = nodeB->pLeft;

        if (nodeE->balance == BalanceValue::RH)
            nodeA->balance = BalanceValue::LH;
        else
            nodeA->balance = BalanceValue::EH;

        if (nodeE->balance == BalanceValue::LH)
            nodeB->balance = BalanceValue::RH;
        else
            nodeB->balance = BalanceValue::EH;

        nodeE->balance = BalanceValue::EH;

        rotateRightToBalance(&(*curr)->pRight);
        rotateLeftToBalance(curr);
    }
    else  // rootSubRight->balance == RH
    {
        // right of right imbalance
        (*curr)->balance = BalanceValue::EH;
        rootSubRight->balance = BalanceValue::EH;
        rotateLeftToBalance(curr);
    }

    hasBecomesHigher = false;
}

template <typename K, typename T>
void AVLTree<K, T>::insertRecursive(Node **curr, const K &key, const T &value,
                                    bool &hasBecomesHigher)
{
    if (*curr == nullptr)
    {
        *curr = new Node(key, value);
        hasBecomesHigher = true;
        return;
    }

    if (key < (*curr)->key)
    {
        insertRecursive(&((*curr)->pLeft), key, value, hasBecomesHigher);
        if (hasBecomesHigher)
        {
            if ((*curr)->balance == BalanceValue::EH)
            {
                (*curr)->balance = BalanceValue::LH;
                hasBecomesHigher = true;
            }
            else if ((*curr)->balance == BalanceValue::RH)
            {
                (*curr)->balance = BalanceValue::EH;
                hasBecomesHigher = false;
            }
            else  // ((*curr)->balance == LH)
            {
                // left imbalance
                fixImbalanceLeftFromInsert(curr, hasBecomesHigher);
            }
        }
    }
    else
    {
        insertRecursive(&((*curr)->pRight), key, value, hasBecomesHigher);
        if (hasBecomesHigher)
        {
            if ((*curr)->balance == BalanceValue::EH)
            {
                (*curr)->balance = BalanceValue::RH;
                hasBecomesHigher = true;
            }
            else if ((*curr)->balance == BalanceValue::LH)
            {
                (*curr)->balance = BalanceValue::EH;
                hasBecomesHigher = false;
            }
            else  // ((*curr)->balance == BalanceValue::RH)
            {
                // right imbalance
                fixImbalanceRightFromInsert(curr, hasBecomesHigher);
            }
        }
    }
}

template <typename K, typename T>
void AVLTree<K, T>::insert(const K &key, const T &value)
{
    if (this->root == nullptr)
    {
        this->root = new Node(key, value);
        count++;
        return;
    }

    bool hasBecomesHigher = false;
    insertRecursive(&root, key, value, hasBecomesHigher);
    count++;
}

template <typename K, typename T>
void AVLTree<K, T>::fixImbalanceLeftFromRemove(Node **curr, bool &hasBecomesShorter)
{
    Node *nodeA = *curr;
    Node *nodeC = (*curr)->pRight;
    hasBecomesShorter = true;  // always true except for nodeC->balance == BalanceValue::EH
    if (nodeC->balance == BalanceValue::LH)
    {
        // left of right imbalance
        Node *nodeD = nodeC->pLeft;

        if (nodeD->balance == BalanceValue::EH)
        {
            nodeA->balance = BalanceValue::EH;
            nodeC->balance = BalanceValue::EH;
        }
        else if (nodeD->balance == BalanceValue::RH)
        {
            nodeA->balance = BalanceValue::LH;
            nodeC->balance = BalanceValue::EH;
        }
        else  // nodeD->balance == LH
        {
            nodeA->balance = BalanceValue::EH;
            nodeC->balance = BalanceValue::RH;
        }

        rotateRightToBalance(&((*curr)->pRight));
        rotateLeftToBalance(curr);
        nodeD->balance = BalanceValue::EH;
    }
    else
    {
        // right of right imbalance
        if (nodeC->balance == BalanceValue::EH)
        {
            nodeA->balance = BalanceValue::RH;
            nodeC->balance = BalanceValue::LH;
            hasBecomesShorter = false;
        }
        else  // nodeC->balance == RH
        {
            nodeA->balance = BalanceValue::EH;
            nodeC->balance = BalanceValue::EH;
        }
        rotateLeftToBalance(curr);
    }
}

template <typename K, typename T>
void AVLTree<K, T>::fixImbalanceRightFromRemove(Node **curr, bool &hasBecomesShorter)
{
    Node *nodeA = *curr;
    Node *nodeC = (*curr)->pLeft;
    hasBecomesShorter = true;  // always true except for nodeC->balance == EH
    if (nodeC->balance == BalanceValue::RH)
    {
        // right of left imbalance
        Node *nodeD = nodeC->pRight;

        if (nodeD->balance == BalanceValue::EH)
        {
            nodeA->balance = BalanceValue::EH;
            nodeC->balance = BalanceValue::EH;
        }
        else if (nodeD->balance == BalanceValue::LH)
        {
            nodeA->balance = BalanceValue::RH;
            nodeC->balance = BalanceValue::EH;
        }
        else  // nodeD->balance == BalanceValue::RH
        {
            nodeA->balance = BalanceValue::EH;
            nodeC->balance = BalanceValue::LH;
        }

        rotateLeftToBalance(&((*curr)->pLeft));
        rotateRightToBalance(curr);
        nodeD->balance = BalanceValue::EH;
    }
    else
    {
        // left of left imbalance
        if (nodeC->balance == BalanceValue::EH)
        {
            nodeA->balance = BalanceValue::LH;
            nodeC->balance = BalanceValue::RH;
            hasBecomesShorter = false;
        }
        else  // nodeC->balance == BalanceValue::LH
        {
            nodeA->balance = BalanceValue::EH;
            nodeC->balance = BalanceValue::EH;
        }
        rotateRightToBalance(curr);
    }
}

template <typename K, typename T>
typename AVLTree<K, T>::Node *AVLTree<K, T>::removeRecursive(Node **curr, const K &key,
                                                             bool &hasBecomesShorter,
                                                             bool &successDelete)
{
    if (*curr == nullptr)
    {
        hasBecomesShorter = false;
        successDelete = false;
        return nullptr;
    }

    if (key < (*curr)->key)
    {
        (*curr)->pLeft = removeRecursive(&((*curr)->pLeft), key, hasBecomesShorter, successDelete);

        if (!successDelete)
            return *curr;

        if (hasBecomesShorter)
        {
            // delete on the left, may have caused imbalance
            if ((*curr)->balance == BalanceValue::LH)
                (*curr)->balance = BalanceValue::EH;
            else if ((*curr)->balance == BalanceValue::EH)
            {
                (*curr)->balance = BalanceValue::RH;
                hasBecomesShorter = false;
            }
            else
            {
                // imbalance - left height smaller than right height by 2, fix that
                fixImbalanceLeftFromRemove(curr, hasBecomesShorter);
            }
        }
    }
    else if ((*curr)->key < key)
    {
        (*curr)->pRight =
            removeRecursive(&((*curr)->pRight), key, hasBecomesShorter, successDelete);

        if (!successDelete)
            return *curr;

        if (hasBecomesShorter)
        {
            // delete on the right, may have caused imbalance
            if ((*curr)->balance == BalanceValue::RH)
                (*curr)->balance = BalanceValue::EH;
            else if ((*curr)->balance == BalanceValue::EH)
            {
                (*curr)->balance = BalanceValue::LH;
                hasBecomesShorter = false;
            }
            else
            {
                // imbalance - right height smaller than left height by 2, fix that
                fixImbalanceRightFromRemove(curr, hasBecomesShorter);
            }
        }
    }
    else  // (*curr)->data == value
    {

        if ((*curr)->pLeft == nullptr)  // no right subtree
        {
            Node *result = (*curr)->pRight;
            delete *curr;
            hasBecomesShorter = true;
            successDelete = true;
            return result;
        }
        else if ((*curr)->pRight == nullptr)  // no left subtree
        {
            Node *result = (*curr)->pLeft;
            delete *curr;
            hasBecomesShorter = true;
            successDelete = true;
            return result;
        }
        else
        {
            // curr node have both subtree, replace with smallest elem on right subtree
            Node *replaceWith = (*curr)->pRight;
            while (replaceWith->pLeft != nullptr)
                replaceWith = replaceWith->pLeft;
            (*curr)->data = replaceWith->data;

            // remove replaceWith from the tree, will always succeed
            (*curr)->pRight = removeRecursive(&((*curr)->pRight), replaceWith->key,
                                              hasBecomesShorter, successDelete);
            if (hasBecomesShorter)
            {
                // delete on the right, may have caused imbalance
                if ((*curr)->balance == BalanceValue::RH)
                    (*curr)->balance = BalanceValue::EH;
                else if ((*curr)->balance == BalanceValue::EH)
                {
                    (*curr)->balance = BalanceValue::LH;
                    hasBecomesShorter = false;
                }
                else
                {
                    // imbalance - right height smaller than left height by 2, fix that
                    fixImbalanceRightFromRemove(curr, hasBecomesShorter);
                }
            }
        }
    }

    return *curr;
}

template <typename K, typename T>
void AVLTree<K, T>::remove(const K &key)
{
    if (this->root == nullptr)
        return;

    bool hasBecomesShorter = false, successDelete = false;
    this->root = removeRecursive(&(this->root), key, hasBecomesShorter, successDelete);

    if (successDelete)
        count--;
}

template <typename K, typename T>
void AVLTree<K, T>::clearRecursive(Node *curr)
{
    if (curr == nullptr)
        return;

    clearRecursive(curr->pLeft);
    clearRecursive(curr->pRight);
    delete curr;
}

template <typename K, typename T>
void AVLTree<K, T>::clear()
{
    if (this->root == nullptr)
        return;

    clearRecursive(this->root);
    this->root = nullptr;
    count = 0;
}

template <typename K, typename T>
void AVLTree<K, T>::forEach(std::function<void(const T &)> func)
{
    if (root == nullptr)
        return;

    // traverse breadth first through the tree, storing visited node into a queue
    // then go through that queue one by one
    std::queue<Node *> bfQueue;
    std::queue<Node *> elemQueue;
    bfQueue.push(root);

    while (!bfQueue.empty())
    {
        Node *curr = bfQueue.front();
        bfQueue.pop();
        if (curr->pLeft != nullptr)
            bfQueue.push(curr->pLeft);
        if (curr->pRight != nullptr)
            bfQueue.push(curr->pRight);

        elemQueue.push(curr);
    }

    while (!elemQueue.empty())
    {
        Node *curr = elemQueue.front();
        elemQueue.pop();
        func(curr->data);
    }
}

#endif  // AVLTREE_HPP
