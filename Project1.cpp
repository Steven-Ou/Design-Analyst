#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
using namespace std;

struct Metrics
{
    long long comparisons = 0;
    long long swaps = 0;
};
struct Stats
{
    double totalTime = 0, minTime = 999999.0, maxTime = 0;
    long long totalComp = 0, minComp = 9999999999, maxComp = 0;
    long long totalSwaps = 0, minSwaps = 9999999999, maxSwaps = 0;

    void update(double time, long long comp, long long swaps)
    {
        //accumulate totals for average
        totalTime += time;
        totalComp += comp;
        totalSwaps += swaps;

        //Check for minimums
        if (time < minTime) minTime = time;
        if (comp < minComp) minComp = comp;
        if (swaps < minSwaps) minSwaps = swaps;

        //Check for maximum
        if (time > maxTime) maxTime = time;
        if (comp > maxComp) maxComp = comp;
        if (swaps > maxSwaps) maxSwaps = swaps;
    }
};
void generateRandomArray(vector<int> &arr, int size)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000000);
    arr.clear();
    for (int i = 0; i < size; ++i)
    {
        arr.push_back(dis(gen));
    }
}
void insertionSort(vector<int> &arr, Metrics &m)
{
    int n = arr.size();
    for (int i = 1; i < n; ++i)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0)
        {
            m.comparisons++;
            if (arr[j] > key)
            {
                arr[j + 1] = arr[j];
                m.swaps++;
                j = j - 1;
            }
            else
                break;
        }
        arr[j + 1] = key;
    }
}
void merge(vector<int> &arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}
void mergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
// regular partition - using fixed pivot
int partition(vector<int> &arr, int low, int high, Metrics &m)
{
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++)
    {
        m.comparisons++;
        if (arr[j] < pivot)
        {
            i++;
            swap(arr[i], arr[j]);
            m.swaps++;
        }
    }
    swap(arr[i + 1], arr[high]);
    m.swaps++;
    return i + 1;
}
// randomized to prevent worst case O(n^2)
int Protectedpartition(vector<int> &arr, int low, int high, Metrics &m)
{
    int randomIdx = low + rand() % (high - low + 1);
    swap(arr[randomIdx], arr[high]);
    m.swaps++;
    return partition(arr, low, high, m);
}
void protectedQuickSort(vector<int> &arr, int low, int high, Metrics &m)
{
    if (low < high)
    {
        int pi = Protectedpartition(arr, low, high, m);
        protectedQuickSort(arr, low, pi - 1, m);
        protectedQuickSort(arr, pi + 1, high, m);
    }
}
void quickSort(vector<int> &arr, int low, int high, Metrics &m)
{
    if (low < high)
    {
        int pi = partition(arr, low, high, m);
        quickSort(arr, low, pi - 1, m);
        quickSort(arr, pi + 1, high, m);
    }
}

void heapify(vector<int> &arr, int n, int i, Metrics &m)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n)
    {
        m.comparisons++;
        if (arr[l] > arr[largest])
            largest = l;
    }
    if (r < n)
    {
        m.comparisons++;
        if (arr[r] > arr[largest])
            largest = r;
    }
    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        m.swaps++;
        heapify(arr, n, largest, m);
    }
}
void heapSort(vector<int> &arr, Metrics &m)
{
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, m);
    for (int i = n - 1; i > 0; i--)
    {
        swap(arr[0], arr[i]);
        m.swaps++;
        heapify(arr, i, 0, m);
    }
}
int main()
{
    const int numRuns = 100;
    const int arraySize = 10000;

    // Accumulators for average
    Metrics totalInsert, totalHeap, totalQSBasic, totalQSProt;
    double totalTimeInsert = 0, totalTimeHeap = 0, totalTimeQSBasic = 0, totalTimeQSProt = 0;

    for (int i = 0; i < numRuns; i++)
    {
        vector<int> baseArray;
        generateRandomArray(baseArray, arraySize);

        // Insertion sort
        vector<int> testArr = baseArray;
        Metrics runM;
        auto start = chrono::high_resolution_clock::now();
        insertionSort(testArr, runM);
        auto end = chrono::high_resolution_clock::now();
        totalTimeInsert += chrono::duration<double>(end - start).count();
        totalInsert.comparisons += runM.comparisons;
        totalInsert.swaps += runM.swaps;

        // Heap Sort
        testArr = baseArray;
        runM.comparisons = 0;
        runM.swaps = 0;
        start = chrono::high_resolution_clock::now();
        heapSort(testArr, runM);
        end = chrono::high_resolution_clock::now();
        totalTimeHeap += chrono::duration<double>(end - start).count();
        totalHeap.comparisons += runM.comparisons;
        totalHeap.swaps += runM.swaps;

        // Quick sort
        testArr = baseArray;
        Metrics qsBasicM;
        start = chrono::high_resolution_clock::now();
        quickSort(testArr, 0, testArr.size() - 1, qsBasicM);
        end = chrono::high_resolution_clock::now();
        totalTimeHeap += chrono::duration<double>(end - start).count();
        totalHeap.comparisons += runM.comparisons;
        totalHeap.swaps += runM.swaps;

        // Protected quicksort test
        testArr = baseArray;
        Metrics qsProtM; // Fresh struct
        start = chrono::high_resolution_clock::now();
        protectedQuickSort(testArr, 0, testArr.size() - 1, qsProtM);
        end = chrono::high_resolution_clock::now();
        totalTimeQSProt += chrono::duration<double>(end - start).count();
        totalQSProt.comparisons += qsProtM.comparisons;
        totalQSProt.swaps += qsProtM.swaps;

        cout << "\n=== FINAL AVERAGES OVER " << numRuns << " RUNS ===" << endl;

        cout << "\nInsertion Sort:" << endl;
        cout << "  Time:        " << totalTimeInsert / numRuns << " seconds" << endl;
        cout << "  Comparisons: " << totalInsert.comparisons / numRuns << endl;
        cout << "  Swaps:       " << totalInsert.swaps / numRuns << endl;

        cout << "\nHeap Sort:" << endl;
        cout << "  Time:        " << totalTimeHeap / numRuns << " seconds" << endl;
        cout << "  Comparisons: " << totalHeap.comparisons / numRuns << endl;
        cout << "  Swaps:       " << totalHeap.swaps / numRuns << endl;

        cout << "\nQuickSort (Basic):" << endl;
        cout << "  Time:        " << totalTimeQSBasic / numRuns << " seconds" << endl;
        cout << "  Comparisons: " << totalQSBasic.comparisons / numRuns << endl;
        cout << "  Swaps:       " << totalQSBasic.swaps / numRuns << endl;

        cout << "\nQuickSort (Protected):" << endl;
        cout << "  Time:        " << totalTimeQSProt / numRuns << " seconds" << endl;
        cout << "  Comparisons: " << totalQSProt.comparisons / numRuns << endl;
        cout << "  Swaps:       " << totalQSProt.swaps / numRuns << endl;

        return 0;
    }
}