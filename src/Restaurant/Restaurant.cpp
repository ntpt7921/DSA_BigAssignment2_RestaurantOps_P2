#include "Restaurant/Restaurant.hpp"
#include "CommandParser/Command.hpp"
#include "HuffmanTreeWrapper.hpp"
#include "Restaurant/CustomerRecord.hpp"
#include "main.h"
#include <string>
#include <utility>

bool CompareByOrderCount::operator()(CustomerRecord const *c1, CustomerRecord const *c2) const
{
    if (c1->orderCount != c2->orderCount)
        return (c1->orderCount < c2->orderCount);
    else
        // both customer is equiv, fallback to insert order within heap
        return false;
}

CustomerRecord::CustomerID Restaurant::getCustomerID(CustomerRecord &c)
{
    std::string &name = c.name;
    if (name.empty())
        throw "Name is empty";

    auto foundAt = prevCustomerID.find(name);
    if (foundAt != prevCustomerID.end())  // if is a previous customer
        return foundAt->second;

    if (name.size() == 1)
    {
        prevCustomerID[name] = 1;
        c.customerID = 1;
        return 1;
    }

    std::unordered_map<char, std::size_t> charCount;
    for (char c : name)
        ++charCount[c];

    HuffmanTreeWrapper<char> huffmanTree;
    huffmanTree.calculateCoding(
        std::vector<std::pair<char, std::size_t>>(charCount.begin(), charCount.end()));

    std::string encodedName;
    for (auto i = name.size(); encodedName.size() < 15 && i > 0; i--)
        encodedName = huffmanTree.getCoding(name[i - 1]) + encodedName;

    if (encodedName.size() >= 15)
        c.customerID = std::stoul(encodedName.substr(encodedName.size() - 15), nullptr, 2);
    else
        c.customerID = std::stoul(encodedName, nullptr, 2);
    return c.customerID;
}

CustomerRecord::TableArea Restaurant::getTableArea(CustomerRecord &c)
{
    bool sec1Full = (section1.size() == MAXSIZE / 2);
    bool sec2Full = (section2.size() == MAXSIZE / 2);
    CustomerRecord::TableArea &result = c.tableArea;

    if (sec1Full && sec2Full)
    {
        // the case where all table is full (need to kick someone out) should be dealt with BEFORE
        // calling this function
        throw "Not suppose to reach this";
    }
    else if (sec1Full)
    {
        result = CustomerRecord::TableArea::SECTION_2;
    }
    else if (sec2Full)
    {
        result = CustomerRecord::TableArea::SECTION_1;
    }
    else
    {
        if (c.customerID % 2 == 1)  // is odd, section 1
            result = CustomerRecord::TableArea::SECTION_1;
        else  // is even, section 2
            result = CustomerRecord::TableArea::SECTION_2;
    }

    return result;
}

CustomerRecord::TableID Restaurant::getTableID(CustomerRecord &c)
{
    CustomerRecord::TableID &tid = c.tableID;
    tid = c.customerID % MAXSIZE + 1;

    for (int i = 0; i < MAXSIZE; i++)
    {
        if (!guessAtTable[tid].first)  // table is empty
            return tid;

        ++tid;
        if (tid > MAXSIZE)
            tid = 1;
    }

    throw "Not suppose to reach this";
}

static inline CustomerRecord::CustomerID keyForArea1(CustomerRecord::CustomerID id)
{
    return id % (MAXSIZE / 2);
}

static inline CustomerRecord::CustomerID keyForArea2(CustomerRecord::CustomerID id) { return id; }

void Restaurant::seatingCustomer(CustomerRecord &c)
{
    c.orderCount = 1;  // new guess will order immediately
    // setting CustomerRecord for the restaurant
    CustomerRecord &currGuess = guessAtTable[c.tableID].second = c;
    guessAtTable[c.tableID].first = true;

    // add customer into different areas (sections)
    switch (c.tableArea)
    {
        case CustomerRecord::TableArea::SECTION_1:
            section1.insert(keyForArea1(currGuess.customerID), &currGuess);
            break;
        case CustomerRecord::TableArea::SECTION_2:
            section2.insert(keyForArea2(currGuess.customerID), &currGuess);
            break;
        default:
            throw "Unknown table area";
    }

    // add customer into queues
    currGuess.placeInQueueFIFO = queueFIFO.enqueue(&currGuess);
    currGuess.placeInQueueLRCO = queueLRCO.enqueue(&currGuess);
    queueLFCO.push(&currGuess);

    // add customer name to tableID mapping
    presentCustomerTableID[currGuess.name] = currGuess.tableID;
}

void Restaurant::removeCustomer(CustomerRecord *c)
{
    switch (c->tableArea)
    {
        case CustomerRecord::TableArea::SECTION_1:
            section1.remove(keyForArea1(c->customerID));
            break;
        case CustomerRecord::TableArea::SECTION_2:
            section2.remove(keyForArea2(c->customerID));
            break;
        default:
            throw "Unknown table area";
            break;
    }

    queueFIFO.erase(c->placeInQueueFIFO);
    queueLRCO.erase(c->placeInQueueLRCO);
    queueLFCO.remove(c);

    guessAtTable[c->tableID].first = false;  // table is now empty
    if (presentCustomerTableID.erase(c->name) != 1)
        throw "Unknown customer name encountered";
}

void Restaurant::serveNewOrderForTable(CustomerRecord::TableID tid)
{
    if (!guessAtTable[tid].first)
        throw "Trying to serve empty table";

    CustomerRecord &customer = guessAtTable[tid].second;
    customer.orderCount++;
    customer.placeInQueueLRCO = queueLRCO.renew(customer.placeInQueueLRCO);
    queueLFCO.renew(&customer);
}

void Restaurant::replaceCustomer(CustomerRecord &c)
{
    CustomerRecord *oldCustomer = nullptr;

    switch (c.customerID % 3)
    {
        case 0:
            // FIFO policy
            oldCustomer = queueFIFO.front();
            break;
        case 1:
            // LRCO policy
            oldCustomer = queueLRCO.front();
            break;
        case 2:
            // LFCO policy
            oldCustomer = queueLFCO.peek();
            break;
        default:
            throw "Unexpected opt value";
    }

    c.tableID = oldCustomer->tableID;
    c.tableArea = oldCustomer->tableArea;

    if (oldCustomer == nullptr)
        throw "Unexpected nullptr encountered";
    removeCustomer(oldCustomer);
}

void Restaurant::doREG(const Command &reg)
{
    CustomerRecord newCustomer;
    newCustomer.name = std::move(reg.name);

    // if that customer is currently present

    auto foundAt = presentCustomerTableID.find(newCustomer.name);
    if (foundAt != presentCustomerTableID.end())
    {
        serveNewOrderForTable(foundAt->second);
        return;
    }

    // if this is a new customer

    getCustomerID(newCustomer);
    // if all the table are full
    if (section1.size() == MAXSIZE / 2 && section2.size() == MAXSIZE / 2)
    {
        // newCustomer take the table ID and table area from old kicked out customer
        replaceCustomer(newCustomer);
    }
    else
    {
        // generate table ID and table area normally
        getTableArea(newCustomer);
        getTableID(newCustomer);
    }
    seatingCustomer(newCustomer);

    return;
}

void Restaurant::doCLE(const Command &cle)
{
    CustomerRecord::TableID tableID = cle.num;

    if (tableID < 1)
    {
        // dismiss all guess at section 1
        section1.forEach([this](CustomerRecord *const &c) { removeCustomer(c); });
    }
    else if (tableID > MAXSIZE)
    {
        // dismiss all guess at section 2
        section2.forEach([this](CustomerRecord *const &c) { removeCustomer(c); });
    }
    else
    {
        // if there no guest at table
        if (!guessAtTable[tableID].first)
            return;
        removeCustomer(&(guessAtTable[tableID].second));
    }
}

void Restaurant::printHT()
{
    section1.forEach(
        [](CustomerRecord *const &cr)
        { std::cout << cr->tableID << '-' << cr->customerID << '-' << cr->orderCount << '\n'; });
}

void Restaurant::printAVL()
{
    section2.forEach(
        [](CustomerRecord *const &cr)
        { std::cout << cr->tableID << '-' << cr->customerID << '-' << cr->orderCount << '\n'; });
}

void Restaurant::printMH()
{
    queueLFCO.forEach([](CustomerRecord *const &cr)
                      { std::cout << cr->tableID << '-' << cr->orderCount << '\n'; });
}

bool Restaurant::processCommand(const Command &cmd)
{
    switch (cmd.type)
    {
        case Command::CommandType::REG:
            doREG(cmd);
            break;
        case Command::CommandType::CLE:
            doCLE(cmd);
            break;
        case Command::CommandType::PrintHT:
            printHT();
            break;
        case Command::CommandType::PrintAVL:
            printAVL();
            break;
        case Command::CommandType::PrintMH:
            printMH();
            break;
        case Command::CommandType::INVALID:
            return false;
        default:
            throw "Unknown command type";
    }

    return true;
}
