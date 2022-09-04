#include <iostream> //cout
#include <queue> //queue
#include <thread> //thread
#include <chrono> // for milliseconds
#include <unistd.h> //sleep

#include "waitable_queue.hpp"

using namespace ilrd;


void WaitTillGetValue(WaitableQueue<std::queue<int>> &queue)
{
    int res = 0;
    std::cout <<"WaitTillGetValue thread: waiting" << std::endl;
    queue.Peek(res);
    std::cout<< "WaitTillGetValue thread: got value - " << res << std::endl;
}

void Wait3Sec(WaitableQueue<std::queue<int>> &queue)
{
    int res = 0;
    std::cout <<"Wait3Sec thread: waiting" << std::endl;
    if (queue.Pop(res, (std::chrono::milliseconds)3000) == true)
    {
         std::cout<< "Wait3Sec thread: got value - " << res << std::endl;
    }
    else
    {
         std::cout<< "Wait3Sec thread: time's up - "<< std::endl;
    }
}

void PushValue(WaitableQueue<std::queue<int>> &queue)
{
    int push = 5;
    std::cout <<"PushValue thread: sleep for 2 sec" << std::endl;
    sleep(5);
    std::cout <<"PushValue thread: pushing now" << std::endl;
    queue.Push(push);
}

void WaitForever(WaitableQueue<std::queue<int>> &queue)
{
    int res = 0;
    std::cout <<"WaitForever thread: waiting" << std::endl;
    queue.Pop(res);
    std::cout<< "WaitForever thread: got value - " << res << std::endl;

}


void NoThreadTests()
{
    WaitableQueue<std::queue<int>> queue;
    if (queue.IsEmpty() != true)
    {
        std::cout << "error tes[&]{return (IsEmpty() == false);}t 1!" << std::endl;
    }
    int a = 5;
    queue.Push(a);
    if (queue.IsEmpty() != false)
    {
        std::cout << "error test 2!" << std::endl;
    }
    int res = 0;
    queue.Peek(res);
    if (res != 5)
    {
        std::cout << "error test 3!" << std::endl;
    }
    res = 0;
    queue.Pop(res);
    if (queue.IsEmpty() != true)
    {
        std::cout << "error test 4!" << std::endl;
    }
    if (res != 5)
    {
        std::cout << "error test 5!" << std::endl;
    }
    if (queue.Pop(res, (std::chrono::milliseconds)2000) != false)
    {
        std::cout << "error test 6!" << std::endl;
    }
    

}

int main()
{ 
    NoThreadTests();
    WaitableQueue<std::queue<int>> queue;
    std::thread t1(WaitTillGetValue, std::ref(queue));
    std::thread t2(PushValue, std::ref(queue));
    std::thread t3(Wait3Sec, std::ref(queue));
    std::thread t4(WaitTillGetValue, std::ref(queue));
    std::thread t5(PushValue, std::ref(queue));
    std::thread t6(PushValue, std::ref(queue));
    std::thread t7(WaitForever, std::ref(queue));

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();









    return 0;
}
