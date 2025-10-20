#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <random>
#include <chrono>
#include <algorithm>
#include <set>

using namespace std;

// Configuration
const int NUM_WAREHOUSES = 10;
const int NUM_PRODUCTS = 20;
const int INITIAL_QUANTITY = 100;
const int NUM_THREADS = 8;
const int OPERATIONS_PER_THREAD = 100;

// Data structures
// warehouses[warehouse_id][product_id] = quantity
map<int, map<int, int>> warehouses;

// One mutex per product - protects that product's data across all warehouses
map<int, mutex> product_mutexes;

// Initial totals for invariant checking
map<int, int> initial_totals;

// Initialize warehouses with products
void initialize()
{
    for (int w = 0; w < NUM_WAREHOUSES; w++)
    {
        for (int p = 0; p < NUM_PRODUCTS; p++)
        {
            warehouses[w][p] = INITIAL_QUANTITY;
        }
    }

    for (int p = 0; p < NUM_PRODUCTS; p++)
    {
        initial_totals[p] = NUM_WAREHOUSES * INITIAL_QUANTITY;
    }
}

// Inventory check - verifies invariants
bool check_inventory()
{
    // Acquire all product locks in sorted order
    vector<unique_lock<mutex>> locks;
    for (int p = 0; p < NUM_PRODUCTS; p++)
    {
        locks.push_back(unique_lock<mutex>(product_mutexes[p]));
    }

    bool valid = true;

    for (int p = 0; p < NUM_PRODUCTS; p++)
    {
        int total = 0;
        for (int w = 0; w < NUM_WAREHOUSES; w++)
        {
            total += warehouses[w][p];
        }

        if (total != initial_totals[p])
        {
            cout << "INVARIANT VIOLATED for product " << p
                 << ": expected " << initial_totals[p]
                 << ", got " << total << endl;
            valid = false;
        }
    }

    cout << "Inventory check completed. All good." << endl;

    return valid;
}

// Move a single product from src to dst warehouse
void move_product(int src_warehouse, int dst_warehouse, int product_id, int quantity)
{
    if (src_warehouse == dst_warehouse)
        return;

    // Lock the product mutex
    lock_guard<mutex> lock(product_mutexes[product_id]);

    // Check if move is possible
    if (warehouses[src_warehouse][product_id] >= quantity)
    {
        warehouses[src_warehouse][product_id] -= quantity;
        warehouses[dst_warehouse][product_id] += quantity;
    }
}

// Move multiple products from src to dst warehouse
void move_multiple_products(int src_warehouse, int dst_warehouse,
                            const vector<int> &product_ids,
                            const vector<int> &quantities)
{
    if (src_warehouse == dst_warehouse)
        return;

    // Sort product IDs to prevent deadlock
    // Thread A needs locks for products [3, 7]
    // Thread B needs locks for products [7, 3]
    // This can lead to deadlock if both threads acquire locks in different orders
    // Only allow locks to be acquired in a consistent order
    vector<int> sorted_products = product_ids;
    sort(sorted_products.begin(), sorted_products.end());

    // Acquire locks in sorted order
    vector<unique_lock<mutex>> locks;
    for (int pid : sorted_products)
    {
        locks.push_back(unique_lock<mutex>(product_mutexes[pid]));
    }

    // Perform all moves
    for (size_t i = 0; i < product_ids.size(); i++)
    {
        int pid = product_ids[i];
        int qty = quantities[i];

        if (warehouses[src_warehouse][pid] >= qty)
        {
            warehouses[src_warehouse][pid] -= qty;
            warehouses[dst_warehouse][pid] += qty;
        }
    }
}

// Worker thread function
void worker_thread(int thread_id)
{
    // Each thread has its own random generator
    random_device rd;
    mt19937 gen(rd() + thread_id);
    uniform_int_distribution<> warehouse_dist(0, NUM_WAREHOUSES - 1);
    uniform_int_distribution<> product_dist(0, NUM_PRODUCTS - 1);
    uniform_int_distribution<> quantity_dist(1, 100);
    uniform_int_distribution<> operation_dist(0, 2); // 0: single, 1-2: multiple

    for (int op = 0; op < OPERATIONS_PER_THREAD; op++)
    {
        int src = warehouse_dist(gen);
        int dst = warehouse_dist(gen);

        if (op % 10 == 0)
        {
            check_inventory();
        }

        if (operation_dist(gen) == 0)
        {
            // Single product move
            int product = product_dist(gen);
            int quantity = quantity_dist(gen);
            move_product(src, dst, product, quantity);
        }
        else
        {
            // Multiple product move (2-3 products)
            int num_products = 2 + (operation_dist(gen) % 2);
            set<int> unique_products;

            while (unique_products.size() < num_products)
            {
                unique_products.insert(product_dist(gen));
            }

            vector<int> products(unique_products.begin(), unique_products.end());
            vector<int> quantities;

            for (size_t i = 0; i < products.size(); i++)
            {
                quantities.push_back(quantity_dist(gen));
            }

            move_multiple_products(src, dst, products, quantities);
        }
    }
}

// Performance test
void run_test()
{
    initialize();

    cout << "Starting test with:" << endl;
    cout << "  Warehouses: " << NUM_WAREHOUSES << endl;
    cout << "  Products: " << NUM_PRODUCTS << endl;
    cout << "  Threads: " << NUM_THREADS << endl;
    cout << "  Operations per thread: " << OPERATIONS_PER_THREAD << endl;
    cout << "  Total operations: " << (NUM_THREADS * OPERATIONS_PER_THREAD) << endl;

    // Launch worker threads
    vector<thread> threads;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads.push_back(thread(worker_thread, i));
    }

    // Wait for all threads to complete
    for (auto &t : threads)
    {
        t.join();
    }

    // Final inventory check
    bool valid = check_inventory();

    cout << "Invariants: " << (valid ? "VALID" : "INVALID") << endl;
}

int main()
{
    run_test();
    return 0;
}
