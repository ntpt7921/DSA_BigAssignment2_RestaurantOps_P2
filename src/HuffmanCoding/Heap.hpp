#ifndef HUFFMANCODING_HEAP_HPP
#define HUFFMANCODING_HEAP_HPP

#include <queue>

namespace details
{

template <typename E, typename Comp>
class heap
{
private:
    E *Heap;
    int n;
    int maxsize;

    void siftdown(int pos)
    {
        while (!isLeaf(pos))
        {  // Stop if pos is a leaf
            int j = leftchild(pos);
            int rc = rightchild(pos);
            if ((rc < n) && Comp::prior(Heap[rc], Heap[j]))
                j = rc;
            if (Comp::prior(Heap[pos], Heap[j]))
                return;
            swap(Heap, pos, j);
            pos = j;
        }
    }

    void swap(E *arr, int i1, int i2) { std::swap(arr[i1], arr[i2]); }

public:
    heap(E *h, int num, int max)
    {
        Heap = h;
        n = num;
        maxsize = max;
        buildHeap();
    }

    int size() const { return n; }

    bool isLeaf(int pos) const { return (pos >= n / 2) && (pos < n); }

    int leftchild(int pos) const { return 2 * pos + 1; }

    int rightchild(int pos) const { return 2 * pos + 2; }

    int parent(int pos) const { return (pos - 1) / 2; }

    void buildHeap()
    {
        for (int i = n / 2 - 1; i >= 0; i--)
            siftdown(i);
    }

    void insert(const E &it)
    {
        int curr = n++;
        Heap[curr] = it;
        while ((curr != 0) && (Comp::prior(Heap[curr], Heap[parent(curr)])))
        {
            swap(Heap, curr, parent(curr));
            curr = parent(curr);
        }
    }

    E removefirst()
    {
        swap(Heap, 0, --n);
        if (n != 0)
            siftdown(0);
        return Heap[n];
    }

    E remove(int pos)
    {
        if (pos == (n - 1))
            n--;  // Last element, no work to do
        else
        {
            swap(Heap, pos, --n);
            // Swap with last value
            while ((pos != 0) && (Comp::prior(Heap[pos], Heap[parent(pos)])))
            {
                swap(Heap, pos, parent(pos));  // Push up large key
                pos = parent(pos);
            }
            if (n != 0)
                siftdown(pos);  // Push down small key
        }
        return Heap[n];
    }
};

};      // namespace details
#endif  // !HUFFMANCODING_HEAP_HPP
