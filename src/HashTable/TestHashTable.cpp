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

    assert(testObject.insert(0, 0));
    assert(!testObject.emptyAt(0));
    assert(testObject[0] == 0);

    assert(testObject.insert(3, 3));
    assert(!testObject.emptyAt(3));
    assert(testObject[3] == 3);
}

void TestHashTable::StaticHashTable::testInsertWithCollision()
{
    ::StaticHashTable<int, 4> testObject;

    assert(testObject.insert(3, 3));

    assert(!testObject.emptyAt(3));
    assert(testObject[3] == 3);

    assert(testObject.emptyAt(0));
    assert(testObject.emptyAt(1));
    assert(testObject.emptyAt(2));

    assert(testObject.insert(3, 0));
    assert(!testObject.emptyAt(0));
    assert(testObject[0] == 0);

    assert(testObject.emptyAt(1));
    assert(testObject.emptyAt(2));
}

void TestHashTable::StaticHashTable::testInsertAfterFull()
{
    ::StaticHashTable<int, 2> testObject;

    assert(testObject.insert(0, 0));
    assert(testObject.insert(0, 1));
    assert(!testObject.insert(0, 2));
}

void TestHashTable::StaticHashTable::testRemove()
{
    ::StaticHashTable<int, 2> testObject;

    assert(testObject.insert(0, 0));
    assert(testObject.insert(0, 1));
    assert(!testObject.emptyAt(0));
    assert(!testObject.emptyAt(1));
    assert(testObject.size() == 2);

    testObject.remove(0);
    testObject.remove(1);
    assert(testObject.emptyAt(0));
    assert(testObject.emptyAt(1));
    assert(testObject.size() == 0);
}

void TestHashTable::StaticHashTable::testPrint()
{
    ::StaticHashTable<int, 4> testObject;

    testObject.insert(0, 0);
    testObject.insert(3, 3);
    testObject.insert(3, 1);
    testObject.forEach([](const int &elem) { std::cout << elem << ' '; });
    std::cout << '\n';
}

void TestHashTable::StaticHashTable::testAll()
{
    testInsert();
    testInsertWithCollision();
    testInsertAfterFull();
    testRemove();
}
