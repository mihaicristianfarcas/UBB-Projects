#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using namespace std;

// Input vectors
vector<int> v1 = {1, 2, 3, 4, 5};
vector<int> v2 = {6, 7, 8, 9, 10};

// Shared resources
queue<int> q;
const int MAX_QUEUE_SIZE = 2;
int scalar_product = 0;

// Synchronization
mutex mtx;
condition_variable cv_producer;
condition_variable cv_consumer;

bool done_producing = false;


void producer_thread(int thread_id, int num_producers) {
    cout << "Producer " << thread_id << " started.\n";

    // Each producer processes a slice of the vectors
    for (size_t i = thread_id; i < v1.size(); i += num_producers) {
        int item = v1[i] * v2[i];

        unique_lock<mutex> lock(mtx);
        cv_producer.wait(lock, [] { return q.size() < MAX_QUEUE_SIZE; });

        q.push(item);
        cout << "Producer " << thread_id << " produced: " << item << "\n";

        cv_consumer.notify_one();
    }

    cout << "Producer " << thread_id << " finished.\n";
}


void consumer_thread(int thread_id) {
    cout << "Consumer " << thread_id << " started.\n";

    while (true) {
        unique_lock<mutex> lock(mtx);

        // Wait until there is work or production is done
        cv_consumer.wait(lock, [] { return !q.empty() || done_producing; });

        if (q.empty() && done_producing) {
            // No more work left
            break;
        }

        int item = q.front();
        q.pop();
        cout << "Consumer " << thread_id << " consumed: " << item << "\n";

        // Notify a producer that there's space in queue
        cv_producer.notify_one();

        scalar_product += item;
        cout << "Intermediate scalar_product = " << scalar_product << "\n";
    }

    cout << "Consumer " << thread_id << " finished.\n";
}


int main() {
    int NR_PRODUCERS = 1;
    int NR_CONSUMERS = 1;

    vector<thread> producers, consumers;

    // Start producers
    for (int i = 0; i < NR_PRODUCERS; ++i) {
        producers.emplace_back(producer_thread, i, NR_PRODUCERS);
    }

    // Start consumers
    for (int i = 0; i < NR_CONSUMERS; ++i) {
        consumers.emplace_back(consumer_thread, i);
    }

    // Wait for all producers to finish
    for (auto &p : producers) {
        p.join();
    }

    // Signal consumers that production is complete
    {
        unique_lock<mutex> lock(mtx);
        done_producing = true;
    }
    cv_consumer.notify_all();

    // Wait for all consumers to finish
    for (auto &c : consumers) {
        c.join();
    }

    cout << "\nFinal scalar product = " << scalar_product << "\n";
    cout << "Grand finale!\n";

    return 0;
}
