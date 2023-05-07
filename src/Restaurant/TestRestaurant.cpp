#include "Restaurant/TestRestaurant.hpp"
#include "Command.hpp"
#include "Restaurant/CustomerRecord.hpp"
#include "main.h"

void TestRestaurant::WrappedRestaurantForTest::testGetCustomerID()
{
    CustomerRecord testRec;
    testRec.name = "z";
    assert(getCustomerID(testRec) == 1);

    testRec.name = "ba";
    assert(getCustomerID(testRec) == 2);
    assert(getCustomerID(testRec) == 2);
    assert(getCustomerID(testRec) == 2);

    testRec.name = "Johnuigfifbahjasbdfhjbasdhjf";
    assert(getCustomerID(testRec) == 12116);
    assert(getCustomerID(testRec) == 12116);
    assert(getCustomerID(testRec) == 12116);
}

void TestRestaurant::WrappedRestaurantForTest::testGetTableArea()
{
    CustomerRecord testRec;
    testRec.name = "test";

    testRec.customerID = 1;
    assert(getTableArea(testRec) == CustomerRecord::TableArea::SECTION_1);

    testRec.customerID = 2;
    assert(getTableArea(testRec) == CustomerRecord::TableArea::SECTION_2);
}

void TestRestaurant::WrappedRestaurantForTest::testGetTableArea2()
{
    CustomerRecord testRec;
    testRec.name = "test";
    testRec.customerID = 1;

    Command testCmd;
    testCmd.type = Command::CommandType::REG;
    testCmd.name = "a";
    doREG(testCmd);  // should put guess into area 1, tableID = 2

    assert(getTableArea(testRec) == CustomerRecord::TableArea::SECTION_2);

    testCmd.name = "ba";
    doREG(testCmd);  // should put guess into area 2, tableId = 1
    try
    {
        getTableArea(testRec);
        assert(false);  // suppose to throw before
    }
    catch (...)
    {
    }
}

void TestRestaurant::WrappedRestaurantForTest::testGetTableArea3()
{
    CustomerRecord testRec;
    testRec.name = "test";
    testRec.customerID = 2;

    Command testCmd;
    testCmd.type = Command::CommandType::REG;
    testCmd.name = "ba";
    doREG(testCmd);  // should put guess into area 2, tableID = 1

    assert(getTableArea(testRec) == CustomerRecord::TableArea::SECTION_1);

    testCmd.name = "a";
    doREG(testCmd);  // should put guess into area 1, tableId = 2
    try
    {
        getTableArea(testRec);
        assert(false);  // suppose to throw before
    }
    catch (...)
    {
    }
}

void TestRestaurant::WrappedRestaurantForTest::testGetTableID()
{
    CustomerRecord testRecord;
    testRecord.name = "test";
    testRecord.customerID = 1;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_1;

    assert(guessAtTable[0].first == false);
    assert(guessAtTable[1].first == false);
    assert(guessAtTable[2].first == false);
    assert(getTableID(testRecord) == 2);

    seatingCustomer(testRecord);  // should put into table 2, SECTION_1
    assert(getTableID(testRecord) == 1);
}

void TestRestaurant::WrappedRestaurantForTest::testRemoveCustomer()
{
    Command testCommand;
    testCommand.name = "a";
    doREG(testCommand);  // add to table 1
    testCommand.name = "ba";
    doREG(testCommand);  // add to table 2

    assert(section1.size() == 1);
    assert(section2.size() == 1);
    assert(guessAtTable[0].first == false);
    assert(guessAtTable[1].first == true);
    assert(guessAtTable[2].first == true);
    assert(queueFIFO.size() == 2);
    assert(queueLRCO.size() == 2);
    assert(queueLFCO.size() == 2);
    assert(presentCustomerTableID.find("a") != presentCustomerTableID.end());
    assert(presentCustomerTableID.find("ba") != presentCustomerTableID.end());

    removeCustomer(&guessAtTable[1].second);
    removeCustomer(&guessAtTable[2].second);

    assert(section1.size() == 0);
    assert(section2.size() == 0);
    assert(guessAtTable[0].first == false);
    assert(guessAtTable[1].first == false);
    assert(guessAtTable[2].first == false);
    assert(queueFIFO.size() == 0);
    assert(queueLRCO.size() == 0);
    assert(queueLFCO.size() == 0);
    assert(presentCustomerTableID.find("a") == presentCustomerTableID.end());
    assert(presentCustomerTableID.find("ba") == presentCustomerTableID.end());
}

void TestRestaurant::WrappedRestaurantForTest::testDoCLE()
{
    Command reg;
    reg.name = "a";
    doREG(reg);  // add to table 2
    reg.name = "ba";
    doREG(reg);  // add to table 1

    assert(guessAtTable[1].first == true);
    assert(guessAtTable[2].first == true);

    Command cle;
    cle.num = 1;
    doCLE(cle);
    assert(guessAtTable[1].first == false);
    assert(guessAtTable[2].first == true);

    cle.num = 2;
    doCLE(cle);
    assert(guessAtTable[1].first == false);
    assert(guessAtTable[2].first == false);
}

void TestRestaurant::WrappedRestaurantForTest::testDoCLE2()
{
    Command reg;
    reg.name = "a";
    doREG(reg);  // add to table 2, section 1
    reg.name = "ba";
    doREG(reg);  // add to table 1, section 2

    assert(guessAtTable[1].first == true);
    assert(guessAtTable[2].first == true);
    assert(section1.size() == 1);
    assert(section2.size() == 1);

    Command cle;
    cle.num = 0;
    doCLE(cle);  // remove all from section 1
    assert(guessAtTable[1].first == true);
    assert(guessAtTable[2].first == false);
    assert(section1.size() == 0);

    cle.num = 10;
    doCLE(cle);  // remove all from section 2
    assert(guessAtTable[1].first == false);
    assert(guessAtTable[2].first == false);
    assert(section2.size() == 0);
}

void TestRestaurant::WrappedRestaurantForTest::testServeNewOrderForTable()
{
    Command reg;
    reg.name = "a";
    doREG(reg);  // add to table 2, section 1
    reg.name = "ba";
    doREG(reg);  // add to table 1, section 2

    assert(guessAtTable[1].first == true);
    assert(guessAtTable[2].first == true);
    assert(section1.size() == 1);
    assert(section2.size() == 1);
    assert(queueFIFO.size() == 2);
    assert(queueLRCO.size() == 2);
    assert(queueLFCO.size() == 2);
    assert(queueLRCO.front()->orderCount == 1);
    assert(queueLRCO.back()->orderCount == 1);

    serveNewOrderForTable(1);
    assert(queueLRCO.back()->orderCount == 2);
    assert(queueLRCO.back() == &guessAtTable[1].second);
    assert(queueLFCO.peek() == &guessAtTable[2].second);

    serveNewOrderForTable(2);
    serveNewOrderForTable(2);
    assert(queueLRCO.back()->orderCount == 3);
    assert(queueLRCO.back() == &guessAtTable[2].second);
    assert(queueLFCO.peek() == &guessAtTable[1].second);
}

void TestRestaurant::WrappedRestaurantForTest::testSeatingCustomer()
{
    CustomerRecord testRecord;
    testRecord.name = "test";
    testRecord.customerID = 1;
    testRecord.tableID = 1;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_1;

    assert(section1.size() == 0);
    assert(guessAtTable[1].first == false);

    seatingCustomer(testRecord);  // should put into table 1, SECTION_1
    assert(section1.size() == 1);
    assert(guessAtTable[1].first == true);
    assert(queueFIFO.front() == &guessAtTable[1].second);
    assert(queueLRCO.front() == &guessAtTable[1].second);
    assert(queueLFCO.peek() == &guessAtTable[1].second);
    assert(presentCustomerTableID.find(testRecord.name)->second == guessAtTable[1].second.tableID);
}

void TestRestaurant::WrappedRestaurantForTest::testReplaceCustomer()
{
    CustomerRecord testRecord;
    testRecord.name = "test";
    testRecord.customerID = 1;
    testRecord.tableID = 1;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_1;

    seatingCustomer(testRecord);  // should put into table 1, SECTION_1
    testRecord.customerID = 2;
    testRecord.tableID = 2;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_2;
    seatingCustomer(testRecord);

    assert(section1.size() == 1);
    assert(section2.size() == 1);
    assert(queueFIFO.size() == 2);
    assert(queueLRCO.size() == 2);
    assert(queueLFCO.size() == 2);

    testRecord.customerID = 3;  // FIFO replace
    testRecord.tableID = 2;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_2;

    replaceCustomer(testRecord);
    assert(testRecord.tableID == 1);
    assert(testRecord.tableArea == CustomerRecord::TableArea::SECTION_1);
    assert(queueFIFO.size() == 1);
    assert(queueLRCO.size() == 1);
    assert(queueLFCO.size() == 1);
}

void TestRestaurant::WrappedRestaurantForTest::testReplaceCustomer2()
{
    CustomerRecord testRecord;
    testRecord.name = "test";
    testRecord.customerID = 1;
    testRecord.tableID = 1;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_1;

    seatingCustomer(testRecord);  // should put into table 1, SECTION_1
    testRecord.customerID = 2;
    testRecord.tableID = 2;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_2;
    seatingCustomer(testRecord);

    assert(section1.size() == 1);
    assert(section2.size() == 1);
    assert(queueFIFO.size() == 2);
    assert(queueLRCO.size() == 2);
    assert(queueLFCO.size() == 2);

    testRecord.customerID = 4;  // LRCO replace
    testRecord.tableID = 1;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_1;

    queueLRCO.renew(guessAtTable[1].second.placeInQueueLRCO);
    replaceCustomer(testRecord);

    assert(testRecord.tableID == 2);
    assert(testRecord.tableArea == CustomerRecord::TableArea::SECTION_2);
    assert(queueFIFO.size() == 1);
    assert(queueLRCO.size() == 1);
    assert(queueLFCO.size() == 1);
}

void TestRestaurant::WrappedRestaurantForTest::testReplaceCustomer3()
{
    CustomerRecord testRecord;
    testRecord.name = "test";
    testRecord.customerID = 1;
    testRecord.tableID = 1;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_1;

    seatingCustomer(testRecord);  // should put into table 1, SECTION_1
    testRecord.customerID = 2;
    testRecord.tableID = 2;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_2;
    seatingCustomer(testRecord);

    assert(section1.size() == 1);
    assert(section2.size() == 1);
    assert(queueFIFO.size() == 2);
    assert(queueLRCO.size() == 2);
    assert(queueLFCO.size() == 2);

    testRecord.customerID = 5;  // LFCO replace
    testRecord.tableID = 1;
    testRecord.tableArea = CustomerRecord::TableArea::SECTION_1;

    serveNewOrderForTable(1);
    serveNewOrderForTable(1);
    replaceCustomer(testRecord);

    assert(testRecord.tableID == 2);
    assert(testRecord.tableArea == CustomerRecord::TableArea::SECTION_2);
    assert(queueFIFO.size() == 1);
    assert(queueLRCO.size() == 1);
    assert(queueLFCO.size() == 1);
}

void TestRestaurant::test()
{
    assert(MAXSIZE == 2 && "MAXSIZE is supposed to be set to 2 for these test");

    {
        WrappedRestaurantForTest test;
        test.testGetCustomerID();
    }
    {
        WrappedRestaurantForTest test;
        test.testGetTableArea();
    }
    {
        WrappedRestaurantForTest test;
        test.testGetTableArea2();
    }
    {
        WrappedRestaurantForTest test;
        test.testGetTableArea3();
    }
    {
        WrappedRestaurantForTest test;
        test.testGetTableID();
    }
    {
        WrappedRestaurantForTest test;
        test.testRemoveCustomer();
    }
    {
        WrappedRestaurantForTest test;
        test.testDoCLE();
    }
    {
        WrappedRestaurantForTest test;
        test.testDoCLE2();
    }
    {
        WrappedRestaurantForTest test;
        test.testServeNewOrderForTable();
    }
    {
        WrappedRestaurantForTest test;
        test.testSeatingCustomer();
    }
    {
        WrappedRestaurantForTest test;
        test.testReplaceCustomer();
    }
    {
        WrappedRestaurantForTest test;
        test.testReplaceCustomer2();
    }
    {
        WrappedRestaurantForTest test;
        test.testReplaceCustomer3();
    }
}
