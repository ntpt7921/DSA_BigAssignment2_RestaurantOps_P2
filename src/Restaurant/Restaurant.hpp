#ifndef RESTAURANT_HPP
#define RESTAURANT_HPP

#include "AVL/AVLTree.hpp"
#include "CommandParser/Command.hpp"
#include "HashTable/StaticHashTable.hpp"
#include "Queue/QueueFIFO.hpp"
#include "Queue/QueueLFCO.hpp"
#include "Queue/QueueLRCO.hpp"
#include "Restaurant/CustomerRecord.hpp"
#include "main.h"
#include <array>
#include <unordered_map>
#include <utility>

struct CompareByOrderCount
{
    bool operator()(CustomerRecord const *c1, CustomerRecord const *c2) const;
};

class Restaurant
{
protected:
    // store info of guess at each table, each table's guest can be indexed with <tableID>
    // info about table is stored as a pair, first elem representing if the table is empty
    // second elem is the customer info
    // the first index (0) will be unused because tableID start from 1
    std::array<std::pair<bool, CustomerRecord>, MAXSIZE + 1> guessAtTable;

    // queue for various policy
    QueueFIFO<CustomerRecord *> queueFIFO;
    QueueLRCO<CustomerRecord *> queueLRCO;
    QueueLFCO<CustomerRecord *, CompareByOrderCount> queueLFCO;

    // data struct for each section
    StaticHashTable<CustomerRecord *, MAXSIZE / 2> section1;
    AVLTree<CustomerRecord::CustomerID, CustomerRecord *> section2;

    // store mapping of customer name to CustomerRecord::CustomerID
    // we use this because generating Huffman tree and extract coding is very resource intensive,
    // while using this will only cause us constant time lookup (on average) for already encountered
    // customer name
    std::unordered_map<std::string, CustomerRecord::CustomerID> prevCustomerID;

    // store mapping from name of customer currently present to their tableID,
    // this make the check faster (constant time on average) instead of O(n) - check every table
    std::unordered_map<std::string, CustomerRecord::TableID> presentCustomerTableID;

    // helper function
    CustomerRecord::CustomerID getCustomerID(CustomerRecord &c);
    CustomerRecord::TableArea getTableArea(CustomerRecord &c);
    CustomerRecord::TableID getTableID(CustomerRecord &c);
    void seatingCustomer(CustomerRecord &c);
    void removeCustomer(CustomerRecord *c);
    void serveNewOrderForTable(CustomerRecord::TableID tid);
    void replaceCustomer(CustomerRecord &c);

    // function for each command type
    void doREG(const Command &reg);
    void doCLE(const Command &cle);
    void printHT();
    void printAVL();
    void printMH();

public:
    Restaurant() = default;
    ~Restaurant() = default;

    bool processCommand(const Command &cmd);
};

#endif  // !RESTAURANT_HPP
