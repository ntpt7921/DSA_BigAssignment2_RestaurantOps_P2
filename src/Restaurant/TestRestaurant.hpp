#ifndef RESTAURANT_TESTRESTAURANT_HPP
#define RESTAURANT_TESTRESTAURANT_HPP

#include "Restaurant/CustomerRecord.hpp"
#include "Restaurant/Restaurant.hpp"

namespace TestRestaurant
{

class WrappedRestaurantForTest : public Restaurant
{
public:
    WrappedRestaurantForTest() = default;
    ~WrappedRestaurantForTest() = default;

    void testGetCustomerID();

    void testGetTableArea();
    void testGetTableArea2();
    void testGetTableArea3();

    void testGetTableID();

    void testRemoveCustomer();
    void testDoCLE();
    void testDoCLE2();

    void testServeNewOrderForTable();

    void testSeatingCustomer();

    void testReplaceCustomer();
    void testReplaceCustomer2();
    void testReplaceCustomer3();
};

void test();

};  // namespace TestRestaurant

#endif  // !RESTAURANT_TESTRESTAURANT_HPPRESTAURANT_TESTRESTAURANT_HPP
