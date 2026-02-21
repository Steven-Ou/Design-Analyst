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
            }else break;
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
int partition(vector<int> &arr, int low, int high, Metrics& m)
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
//randomized to prevent worst case O(n^2)
int Protectedpartition(vector<int>& arr, int low, int high , Metrics& m ){
    int randomIdx = low + rand() %(high-low+1);
    swap(arr[randomIdx], arr[high]);
    m.swaps++;
    return Protectedpartition(arr, low, high,m); 
}
int main()
{
}