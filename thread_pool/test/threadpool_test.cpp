#include <iostream>

#include "threadpool.hpp"
#include "task.hpp"

using namespace ilrd;

std::mutex lock_t;

class PrintTaskLow : public Task
{
public:
    PrintTaskLow(Priority priority = LOW) : Task(priority){}
    void Execute()
    {
        std::lock_guard<std::mutex> lock(lock_t);
        std::cout << "Print low task " << std::endl;
    }
};

class PrintTaskMid : public Task
{
public:
    PrintTaskMid(Priority priority = MID) : Task(priority){}
    void Execute()
    {
        std::lock_guard<std::mutex> lock(lock_t);
        std::cout << "Print MID task " << std::endl;
    }
};


int APlusB(int a, int b)
{
    return a + b;
}

float APlusBPlusC(float a, float b, float c)
{
    return a + b + c;
}

void PrintRes(int res)
{
    std::lock_guard<std::mutex> lock(lock_t);
    std::cout << "the res is: " << res << std::endl;
}

void PrintRes(float res)
{
    std::lock_guard<std::mutex> lock(lock_t);
    std::cout << "the res is: " << res << std::endl;
}


int main()
{
    ThreadPool thread_pool;
    std::shared_ptr<Task> task_low = std::make_shared<PrintTaskLow>();
    std::shared_ptr<Task> task_mid = std::make_shared<PrintTaskMid>();
    for (int i = 0; i < 10; i++)
    {
        thread_pool.AddTask(task_mid);
        thread_pool.AddTask(task_low);
    }

    std::shared_ptr<FutureTask<int, int, int>> task_plus = std::make_shared<FutureTask<int, int, int>>(APlusB, Task::HIGH, 3, 4);
    std::shared_ptr<FutureTask<int, int, int>> task_plus2 = std::make_shared<FutureTask<int, int, int>>(APlusB, Task::HIGH, -2, 3);
    std::shared_ptr<FutureTask<int, int, int>> task_plus3 = std::make_shared<FutureTask<int, int, int>>(APlusB, Task::HIGH, -3, -44);
    std::shared_ptr<FutureTask<int, int, int>> task_plus4 = std::make_shared<FutureTask<int, int, int>>(APlusB, Task::LOW, 0, 0);
    thread_pool.AddTask(task_plus);
    thread_pool.AddTask(task_plus2);
    thread_pool.AddTask(task_plus3);
    thread_pool.AddTask(task_plus4);
    int res = task_plus->GetReturnValue();
    PrintRes(res);
    res = task_plus2->GetReturnValue();
    PrintRes(res);
    res = task_plus3->GetReturnValue();
    PrintRes(res);
    res = task_plus4->GetReturnValue();
    PrintRes(res);

    thread_pool.Pause();
    std::cout << "pause!!!!!" << std::endl;

    res = 5;

    std::shared_ptr<FutureTask<float, float, float, float>> task_float = std::make_shared<FutureTask<float, float, float, float>>(APlusBPlusC, Task::HIGH, 3.5, 4.25, 2.25);
    thread_pool.AddTask(task_float);
    thread_pool.AddTask(task_plus);
    sleep(2);
    thread_pool.Continue();
    res = task_plus->GetReturnValue();
    float res_f = task_float->GetReturnValue();
    PrintRes(res);
    PrintRes(res_f);

    sleep(3);

    for (int i = 0; i < 10; i++)
    {
        thread_pool.AddTask(task_mid);
        thread_pool.AddTask(task_low);
    }

    thread_pool.SetNumOfThreads(4);

    thread_pool.Pause();
    std::cout << "pause!!!!!" << std::endl;
    thread_pool.Continue();
    std::cout << "Continue!!!!!" << std::endl;




    

    
    




    









    return 0;
}




