#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <map>

int quickselect(std::vector<int>& arr, int k);
int _quickselect(std::vector<int>& arr, int left, int right, int k);
int _partition(std::vector<int>& arr, int left, int right, int pivot_idx);
void quicksort(std::vector<int>& arr);
void _quicksort(std::vector<int>& arr, int left, int right);
std::pair<std::map<int, double>, std::map<int, double>> benchmark_quickselect_vs_quicksort(
    const std::vector<int>& sizes = {10000000, 20000000, 40000000, 80000000, 160000000},
    int trials = 100);
void plot_results(const std::map<int, double>& quickselect_times, const std::map<int, double>& quicksort_times);

std::mt19937 get_rng() {
    std::random_device rd;
    return std::mt19937(rd());
}

void insertion_sort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        
        arr[j + 1] = key;
    }
}

int median_of_three(std::vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    
    if (arr[left] > arr[mid])
        std::swap(arr[left], arr[mid]);
    if (arr[left] > arr[right])
        std::swap(arr[left], arr[right]);
    if (arr[mid] > arr[right])
        std::swap(arr[mid], arr[right]);
    
    std::swap(arr[mid], arr[right-1]);
    return right-1;
}

int _partition_optimized(std::vector<int>& arr, int left, int right) {
    int pivot_idx = median_of_three(arr, left, right);
    int pivot_value = arr[pivot_idx];

    int i = left;
    int j = right - 1;
    
    while (true) {
        while (arr[++i] < pivot_value) {}
        while (arr[--j] > pivot_value) {}
        
        if (i >= j)
            break;
        std::swap(arr[i], arr[j]);
    }
    
    std::swap(arr[i], arr[right-1]);
    return i;
}

int quickselect(std::vector<int>& arr, int k) {
    if (k < 0 || k >= arr.size()) {
        throw std::invalid_argument("k must be in range [0, arr.size()-1]");
    }
    
    return _quickselect(arr, 0, arr.size() - 1, k);
}

int _quickselect(std::vector<int>& arr, int left, int right, int k) {
    if (left == right) {
        return arr[left];
    }
    
    std::mt19937 rng = get_rng();
    std::uniform_int_distribution<int> uni(left, right);
    int pivot_idx = uni(rng);
    
    pivot_idx = _partition(arr, left, right, pivot_idx);
    
    if (k == pivot_idx) {
        return arr[k];
    } else if (k < pivot_idx) {
        return _quickselect(arr, left, pivot_idx - 1, k);
    } else {
        return _quickselect(arr, pivot_idx + 1, right, k);
    }
}

int _partition(std::vector<int>& arr, int left, int right, int pivot_idx) {
    int pivot_value = arr[pivot_idx];
    
    std::swap(arr[pivot_idx], arr[right]);
    
    int store_idx = left;
    
    for (int i = left; i < right; i++) {
        if (arr[i] < pivot_value) {
            std::swap(arr[store_idx], arr[i]);
            store_idx++;
        }
    }
    
    std::swap(arr[store_idx], arr[right]);
    
    return store_idx;
}

void quicksort(std::vector<int>& arr) {
    if (arr.size() <= 1) {
        return;
    }
    
    _quicksort(arr, 0, arr.size() - 1);
}

void _quicksort(std::vector<int>& arr, int left, int right) {
    std::vector<std::pair<int, int>> stack;
    stack.push_back({left, right});
    
    while (!stack.empty()) {
        std::tie(left, right) = stack.back();
        stack.pop_back();
        
        if (right - left <= 16) {
            insertion_sort(arr, left, right);
            continue;
        }
        
        bool is_sorted = true;
        for (int i = left; i < right; i++) {
            if (arr[i] > arr[i + 1]) {
                is_sorted = false;
                break;
            }
        }
        if (is_sorted) continue;
        
        int pivot_idx = _partition_optimized(arr, left, right);
        
        if (pivot_idx - left < right - pivot_idx) {
            stack.push_back({pivot_idx + 1, right});
            stack.push_back({left, pivot_idx - 1});
        } else {
            stack.push_back({left, pivot_idx - 1});
            stack.push_back({pivot_idx + 1, right});
        }
    }
}

std::pair<std::map<int, double>, std::map<int, double>> benchmark_quickselect_vs_quicksort(
    const std::vector<int>& sizes, int trials) {
    
    std::map<int, double> quickselect_times;
    std::map<int, double> quicksort_times;
    
    std::mt19937 rng = get_rng();
    
    for (int size : sizes) {
        std::cout << "oroltiin hemjee " << size << "..." << std::endl;
        
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }
        std::shuffle(arr.begin(), arr.end(), rng);
        

        double qs_total_time = 0.0;
        for (int t = 0; t < trials; t++) {
            std::uniform_int_distribution<int> uni(0, size - 1);
            int k = uni(rng);
            
            std::vector<int> arr_copy = arr;
            
            auto start_time = std::chrono::high_resolution_clock::now();
            quickselect(arr_copy, k);
            auto end_time = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double> duration = end_time - start_time;
            qs_total_time += duration.count();
        }
        
        quickselect_times[size] = qs_total_time / trials;
        
        std::vector<int> arr_copy = arr;
        auto start_time = std::chrono::high_resolution_clock::now();
        quicksort(arr_copy);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double> duration = end_time - start_time;
        quicksort_times[size] = duration.count();
        
        std::cout << "QuickSelect hugatsaa: " << std::fixed << std::setprecision(6) 
                  << quickselect_times[size] << " seconds" << std::endl;
        std::cout << "QuickSort hugatsaa: " << std::fixed << std::setprecision(6)
                  << quicksort_times[size] << " seconds" << std::endl;
    }
    
    return {quickselect_times, quicksort_times};
}

void plot_results(const std::map<int, double>& quickselect_times, const std::map<int, double>& quicksort_times) {

    std::ofstream outfile("quickselect_vs_quicksort_data.csv");
    outfile << "Size,QuickSelect,QuickSort\n";
    
    for (const auto& [size, time] : quickselect_times) {
        outfile << size << "," << time << "," << quicksort_times.at(size) << "\n";
    }
    outfile.close();
    
    std::cout << "\nResults summary:" << std::endl;
    std::cout << "Hemjee, QuickSelect Time (s), QuickSort Time (s)" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    
    for (const auto& [size, time] : quickselect_times) {
        std::stringstream ss;
        ss.imbue(std::locale(""));
        ss << std::fixed << size;
        
        std::cout << std::setw(12) << ss.str() << ", "
                  << std::fixed << std::setprecision(6) << std::setw(20) << time << ", "
                  << std::fixed << std::setprecision(6) << std::setw(18) << quicksort_times.at(size) << std::endl;
    }
}

int main() {
    std::vector<int> test_sizes = {10000000, 20000000, 40000000, 80000000, 160000000};
    int trials = 100;
    
    try {        
        auto [qs_times, qsort_times] = benchmark_quickselect_vs_quicksort(test_sizes, trials);
        plot_results(qs_times, qsort_times);
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}