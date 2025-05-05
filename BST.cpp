#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <iomanip>
#include <map>
#include <fstream>
#include <string>

class TreeNode {
public:
    int key;
    TreeNode* left;
    TreeNode* right;
    double priority;
    
    TreeNode(int k) : key(k), left(nullptr), right(nullptr) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        priority = dis(gen);
    }
};

class RandomizedBST {
private:
    TreeNode* root;
    int nodes_visited;
    
    TreeNode* _insert(TreeNode* node, int key) {
        if (node == nullptr) {
            return new TreeNode(key);
        }
        
        if (key == node->key) {
            return node;
        }
        
        if (key < node->key) {
            node->left = _insert(node->left, key);
            
            if (node->left && node->left->priority > node->priority) {
                node = _rotate_right(node);
            }
        } else {
            node->right = _insert(node->right, key);
            
            if (node->right && node->right->priority > node->priority) {
                node = _rotate_left(node);
            }
        }
        
        return node;
    }
    
    TreeNode* _rotate_right(TreeNode* y) {
        TreeNode* x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }
    
    TreeNode* _rotate_left(TreeNode* x) {
        TreeNode* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }
    
    bool _search(TreeNode* node, int key) {
        nodes_visited++;
        
        if (node == nullptr) {
            return false;
        }
        
        if (key == node->key) {
            return true;
        }
        
        if (key < node->key) {
            return _search(node->left, key);
        } else {
            return _search(node->right, key);
        }
    }
    
    void _cleanup(TreeNode* node) {
        if (node == nullptr) return;
        _cleanup(node->left);
        _cleanup(node->right);
        delete node;
    }
    
public:
    RandomizedBST() : root(nullptr), nodes_visited(0) {}
    
    ~RandomizedBST() {
        _cleanup(root);
    }
    
    void reset_counter() {
        nodes_visited = 0;
    }
    
    void insert(int key) {
        root = _insert(root, key);
    }
    
    bool search(int key) {
        reset_counter();
        return _search(root, key);
    }
    
    std::pair<bool, int> search_count_nodes(int key) {
        reset_counter();
        bool result = _search(root, key);
        return {result, nodes_visited};
    }
};

void plot_results(const std::map<int, double>& results, const std::string& filename) {
    std::ofstream outfile(filename);
    
    if (!outfile.is_open()) {
        std::cerr << "Error" << filename<< std::endl;
        return;
    }
    
    outfile << "modnii hemjee, zangilaa" << std::endl;
    
    for (const auto& [size, avg_nodes] : results) {
        outfile << size << "," << std::fixed << std::setprecision(2) << avg_nodes << std::endl;
    }
    
    outfile.close();
    
}

std::map<int, double> test_randomized_bst_performance(const std::vector<int>& sizes, int searches) {
    std::map<int, double> results;
    
    for (int size : sizes) {
        std::cout << "Testing Randomized BST with " << size << " nodes..." << std::endl;
        
        RandomizedBST bst;
        
        std::cout << "Generating " << size << " random keys..." << std::endl;
        std::vector<int> keys;
        keys.reserve(size);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, size * 10);
        
        for (int i = 0; i < size; ++i) {
            keys.push_back(dis(gen));
        }
        
        std::cout << "Inserting " << size << " keys into the BST..." << std::endl;
        auto start_time = std::chrono::high_resolution_clock::now();
        
        for (int key : keys) {
            bst.insert(key);
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> insertion_time = end_time - start_time;
        
        std::cout << "Insertion took " << insertion_time.count() << " seconds" << std::endl;
        
        std::cout << "Preparing " << searches << " search keys..." << std::endl;
        std::vector<int> existing_keys;
        std::sample(keys.begin(), keys.end(), std::back_inserter(existing_keys), 
                   searches / 2, gen);
        
        std::vector<int> non_existing_keys;
        int max_key = *std::max_element(keys.begin(), keys.end());
        for (int i = 0; i < searches / 2; ++i) {
            non_existing_keys.push_back(max_key + i + 1);
        }
        
        std::vector<int> search_keys;
        search_keys.reserve(searches);
        search_keys.insert(search_keys.end(), existing_keys.begin(), existing_keys.end());
        search_keys.insert(search_keys.end(), non_existing_keys.begin(), non_existing_keys.end());
        std::shuffle(search_keys.begin(), search_keys.end(), gen);
        
        std::cout << "Performing " << searches << " searches..." << std::endl;
        int total_nodes_visited = 0;
        
        start_time = std::chrono::high_resolution_clock::now();
        
        for (int key : search_keys) {
            auto [_, nodes_visited] = bst.search_count_nodes(key);
            total_nodes_visited += nodes_visited;
        }
        
        end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> search_time = end_time - start_time;
        
        double avg_nodes_visited = static_cast<double>(total_nodes_visited) / searches;
        
        std::cout << "Search took " << search_time.count() << " seconds" << std::endl;
        std::cout << "Average nodes visited: " << avg_nodes_visited << std::endl;
        
        results[size] = avg_nodes_visited;
    }
    
    return results;
}

int main() {
    std::vector<int> test_sizes = {5000000, 10000000, 20000000};
    int searches = 1000000;
    
    auto results = test_randomized_bst_performance(test_sizes, searches);
    
    std::cout << "\nResults:" << std::endl;
    for (const auto& [size, avg_nodes] : results) {
        std::cout << "Tree size " << size << ": Average nodes visited = " 
                  << std::fixed << std::setprecision(2) << avg_nodes << std::endl;
    }
    
    plot_results(results, "randomized_bst_results.csv");
    
    return 0;
}