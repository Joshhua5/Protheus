//
//  MCMPQueueTest.cpp
//  Protheus
//
//  Created by Joshua Waring on 10/07/2015.
//  Copyright (c) 2015 Joshua Waring. All rights reserved.
//
#include "stdafx.h"
#include "CppUnitTest.h"


using namespace Pro;
using namespace Util;

#define SAMPLE_SIZE 1000000
#define PRODUCER_MAX 5
#define CONSUMER_MAX 5
#define TEST_SIZE 1

void consumer(std::atomic<bool>* wait,
              mcmp_queue<int>* queue,
              std::atomic<unsigned long long>* ret,
              std::atomic<unsigned>* active_thread_count,
              int c, int p) {
    long long result = 0;
    int obj;
    active_thread_count->fetch_add(1);
    while (wait->load());
    for (unsigned x = 0; x < (SAMPLE_SIZE * p) / c; ++x) {
        if (queue->Pop(obj) == true) {
            if (obj < 0 || obj > SAMPLE_SIZE) {
                queue->DebugInfo();
                std::cout << obj;
            }
            result += obj;
        }
        else {
            // Failed popping is expected
            // Thread scheduling can leave the consumers waiting for a producer
            --x;
        }
    }
    // Last thread checks for any leftovers
    if(active_thread_count->fetch_sub(1) == 1){
        while(queue->size() != 0){
            // The division for the for loop may not be perfect
            // so this loop is to catch any variables that arn't popped.
            if (queue->Pop(obj) == true){
                if (obj < 0 || obj > SAMPLE_SIZE) {
                    queue->DebugInfo();
                    std::cout << obj;
                }
                result += obj;
            }
        }
    }
    ret->fetch_add(result);
}

void producer(std::atomic<bool>* wait, mcmp_queue<int>* queue, std::atomic<unsigned long long>* ret) {
    long long result = 0;
    while (wait->load());
    for (unsigned x = 0; x < SAMPLE_SIZE; ++x) {
        if (queue->Push(x) == true) {
            result += x;
        }
        else {
            // Failed pushing is not expected
            global_log.Report<LogCode::FAULT>("Failed push", __FUNCTION__, __LINE__);
            --x;
        }
    }
    ret->fetch_add(result);
}

void TestSingleThreaded(char& exit_status){
    // Single threaded
    {
        global_log.Report<LogCode::MESSAGE>("Testing Single Threaded use of a mcmp_queue", __FUNCTION__, __LINE__);
        
        mcmp_queue<int> queue(SAMPLE_SIZE, 7);
        for (int x = 0; x < SAMPLE_SIZE; ++x)
            if (!queue.Push(x)) {
                global_log.Report<LogCode::FAULT>("False returned with pushing the queue: " + std::to_string(x), __FUNCTION__, __LINE__);
                // print debugging info on stacks
                exit_status |= 1;
            }
        
        for (int x = 0; x < SAMPLE_SIZE; ++x) {
            int ret;
            if (queue.Pop(ret) == false) {
                global_log.Report<LogCode::FAULT>("False returned when popping the queue: " + std::to_string(x++), __FUNCTION__, __LINE__);
                exit_status |= 2;
            }
            if (ret != x) {
                global_log.Report<LogCode::FAULT>("Queue returned value out of order: " + std::to_string(ret) + " Expected " + std::to_string(x), __FUNCTION__, __LINE__);
                exit_status |= 4;
            }
        }
    }
}

int main() {
    char exit_status = 0;
    global_log.EchoOnConsole(true);
    
    TestSingleThreaded(exit_status);
    
    unsigned long long test_speed[TEST_SIZE][CONSUMER_MAX][PRODUCER_MAX];
    memset(test_speed, sizeof(test_speed), 0);
    
    // To increase the chance of detecting an error
    for (int x = 0; x < TEST_SIZE; ++x) {
        for(int c = 1; c <= CONSUMER_MAX; c++){
            for(int p = 1; p <= PRODUCER_MAX; p++)
            {
                global_log.Report<LogCode::MESSAGE>("Testing " + std::to_string(c) +
                                                    " consumer " + std::to_string(p) +
                                                    " producer use of a mcmp_queue", __FUNCTION__, __LINE__);
                std::atomic<unsigned long long> prod_result(0);
                std::atomic<unsigned long long> cons_result(0);
                std::atomic<unsigned> active_consumer_thread_count(0);
                std::atomic<bool> wait(true);
                
                mcmp_queue<int> queue(SAMPLE_SIZE * c * p, (c + p) * 2);
                
                std::vector<std::thread*> consumers;
                std::vector<std::thread*> producers;
                
                for(int x = 0; x < c; ++x)
                    consumers.push_back(new std::thread(&consumer, &wait, &queue, &cons_result, &active_consumer_thread_count, c, p));
                for(int x = 0; x < p; ++x)
                    producers.push_back(new std::thread(&producer, &wait, &queue, &prod_result));
                
                auto start = std::chrono::high_resolution_clock::now();
                
                wait = false;
                
                for(std::thread* thread : producers)
                    thread->join();
                for(std::thread* thread : consumers)
                    thread->join();
                
                auto end = std::chrono::high_resolution_clock::now();

                for(std::thread* thread : producers)
                    delete thread;
                
                for(std::thread* thread : consumers)
                    delete thread;
                
                test_speed[x][c-1][p-1] = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                
                if (prod_result != cons_result) {
                    queue.DebugInfo();
                    global_log.Report<LogCode::MESSAGE>("Values were lost", __FUNCTION__, __LINE__);
                    exit_status |= 8;
                } 
                global_log.Flush();
            }
        }
    }
    
    
    unsigned long long average[CONSUMER_MAX][PRODUCER_MAX];
    memset(average, sizeof(average), 0);
    
    for(int x = 0; x < CONSUMER_MAX; ++x){
        for(int y = 0; y < PRODUCER_MAX; ++y){
            unsigned long long total = 0;
            for(int z = 0; z < TEST_SIZE; ++z){
                total += test_speed[z][x][y];
            }
            average[x][y] = total / TEST_SIZE;
            // Scale the results to the sample size
            average[x][y] /= y + 1;
        }
    }
    
    printf("Profile Results SampleSize: %i, ms\n", SAMPLE_SIZE);
    printf("Consumer ->\n");
    printf("Producer \\/\n");
    for(int y = 0; y < PRODUCER_MAX; ++y){
        for(int x = 0; x < CONSUMER_MAX; ++x){
            printf("%llu ", average[x][y]);
        }
        printf("\n");
    }
    
    return exit_status;
}