#ifndef CUSTOMER_RECORD_HPP
#define CUSTOMER_RECORD_HPP

#include "Queue/QueueFIFO.hpp"
#include "Queue/QueueLRCO.hpp"
#include <cinttypes>
#include <cstdint>
#include <string>

struct CustomerRecord
{
    enum class TableArea
    {
        OCEAN_SIDE = 1,
        MOUNTAIN_SIDE = 2,
    };
    using CustomerID = uint_fast16_t;
    using TableID = uint_fast16_t;

    std::string name;
    CustomerID customerID;  // derived from huffman coding of the customer's name
    TableArea tableArea;
    TableID tableID;
    uint_fast32_t orderCount;  // i'll be surprised if anyone order mroe than 4 billion times

    QueueFIFO<CustomerRecord *>::iterator placeInQueueFIFO;
    QueueLRCO<CustomerRecord *>::iterator placeInQueueLRCO;
    // LFCO implementation (heap) don't allow for constant time deletion, so no pointer is store for
    // this structure
};

#endif  // !CUSTOMER_RECORD_HPP
