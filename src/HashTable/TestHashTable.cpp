#include "HashTable/TestHashTable.hpp"
#include "HashTable/StaticHashTable.hpp"
#include <cassert>
#include <iostream>

void TestHashTable::StaticHashTable::testInsert()
{
    ::StaticHashTable<int, 4> testObject;

    assert(testObject.emptyAt(0));
    assert(testObject.emptyAt(1));
    assert(testObject.emptyAt(2));
    assert(testObject.emptyAt(3));

    assert(testObject.add(0, 0));
    assert(!testObject.emptyAt(0));
    assert(testObject[0] == 0);

    assert(testObject.add(3, 3));
    assert(!testObject.emptyAt(3));
    assert(testObject[3] == 3);
}

void TestHashTable::StaticHashTable::testInsertWithCollision()
{
    ::StaticHashTable<int, 4> testObject;

    assert(testObject.add(3, 3));

    assert(!testObject.emptyAt(3));
    assert(testObject[3] == 3);

    assert(testObject.emptyAt(0));
    assert(testObject.emptyAt(1));
    assert(testObject.emptyAt(2));

    assert(testObject.add(3, 0));
    assert(!testObject.emptyAt(0));
    assert(testObject[0] == 0);

    assert(testObject.emptyAt(1));
    assert(testObject.emptyAt(2));
}

void TestHashTable::StaticHashTable::testInsertAfterFull()
{
    ::StaticHashTable<int, 2> testObject;

    assert(testObject.add(0, 0));
    assert(testObject.add(0, 1));
    assert(!testObject.add(0, 2));
}

void TestHashTable::StaticHashTable::testPrint()
{
    ::StaticHashTable<int, 4> testObject;

    testObject.add(0, 0);
    testObject.add(3, 3);
    testObject.add(3, 1);
    testObject.print([](const int &elem) { std::cout << elem << ' '; });
    std::cout << '\n';
}

void TestHashTable::StaticHashTable::testAll()
{
    testInsert();
    testInsertWithCollision();
    testInsertAfterFull();
}
