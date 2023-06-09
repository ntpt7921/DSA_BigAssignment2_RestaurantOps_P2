#ifndef HUFFMANCODING_TESTHUFFMANCODING_HPP
#define HUFFMANCODING_TESTHUFFMANCODING_HPP

namespace TestHuffmanCoding
{

namespace Heap
{

void testInsert();
void test();

};  // namespace Heap

namespace HuffTree
{

void testMemoryLeakageOneLeafNode();
void testMemoryLeakage();
void test();

};  // namespace HuffTree

namespace HuffmanTreeWrapper
{

void testSamplePattern();
void testSamplePatternProvided();
void testCalculateMultipleTree();
void test();

};  // namespace HuffmanTreeWrapper

};  // namespace TestHuffmanCoding

#endif  // !HUFFMANCODING_TESTHUFFMANCODING_HPP
