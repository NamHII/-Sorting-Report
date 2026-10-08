#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Hàm hỗ trợ trộn mảng cho MergeSort
void merge(vector<double>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<double> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) { arr[k] = L[i]; i++; }
        else { arr[k] = R[j]; j++; }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

// 1. Thuật toán MergeSort
void mergeSort(vector<double>& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Hàm hỗ trợ vun đống cho HeapSort
void heapify(vector<double>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// 2. Thuật toán HeapSort
void heapSort(vector<double>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// 3. Thuật toán QuickSort (Pivot phần tử giữa tránh Stack Overflow ở mảng có thứ tự)
void quickSort(vector<double>& arr, int low, int high) {
    if (low < high) {
        double pivot = arr[low + (high - low) / 2];
        int i = low - 1;
        int j = high + 1;
        while (true) {
            do { i++; } while (arr[i] < pivot);
            do { j--; } while (arr[j] > pivot);
            if (i >= j) break;
            swap(arr[i], arr[j]);
        }
        quickSort(arr, low, j);
        quickSort(arr, j + 1, high);
    }
}

// Hàm đo thời gian thực thi
template <typename Func>
double measureTime(Func sortFunc, vector<double> arr) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto stop = high_resolution_clock::now();
    return duration<double, milli>(stop - start).count();
}

int main() {
    const int NUM_ARRAYS = 10;
    const int SIZE = 1000000;
    vector<vector<double>> datasets(NUM_ARRAYS, vector<double>(SIZE));
    mt19937 rng(42);
    uniform_real_distribution<double> dist(0.0, 1000000.0);

    // Tạo bộ dữ liệu
    cout << "Khoi tao du lieu..." << endl;
    for (int i = 0; i < NUM_ARRAYS; i++) {
        for (int j = 0; j < SIZE; j++) datasets[i][j] = dist(rng);
    }
    sort(datasets[0].begin(), datasets[0].end()); // Dãy 1: Tăng dần
    sort(datasets[1].begin(), datasets[1].end(), greater<double>()); // Dãy 2: Giảm dần

    ofstream outFile("ket_qua_sap_xep.csv");
    outFile << "Day,std::sort,QuickSort,HeapSort,MergeSort\n";

    for (int i = 0; i < NUM_ARRAYS; i++) {
        double timeStdSort = measureTime([](vector<double>& a) { sort(a.begin(), a.end()); }, datasets[i]);
        double timeQuickSort = measureTime([](vector<double>& a) { quickSort(a, 0, a.size() - 1); }, datasets[i]);
        double timeHeapSort = measureTime([](vector<double>& a) { heapSort(a); }, datasets[i]);
        double timeMergeSort = measureTime([](vector<double>& a) { mergeSort(a, 0, a.size() - 1); }, datasets[i]);

        outFile << "Day " << i + 1 << "," << timeStdSort << "," << timeQuickSort << "," << timeHeapSort << "," << timeMergeSort << "\n";
        cout << "Hoan thanh xu ly day " << i + 1 << endl;
    }

    outFile.close();
    cout << "Du lieu luu tai ket_qua_sap_xep.csv" << endl;
    return 0;
}
