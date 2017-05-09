#include "tc_boundedblockingqueue.h"
#include "tc_countdownlatch.h"
#include "tc_thread.h"


#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include <stdio.h>

class Test
{
 public:
  Test(int numThreads)
    : queue_(20),
      latch_(numThreads),
      threads_(numThreads)
  {
    for (int i = 0; i < numThreads; ++i)
    {
      char name[32];
      snprintf(name, sizeof name, "work thread %d", i);
      threads_.push_back(new taf::Thread(
            boost::bind(&Test::threadFunc, this), std::string(name)));
    }
    for_each(threads_.begin(), threads_.end(), boost::bind(&taf::Thread::start, _1));
  }

  void run(int times)
  {
    printf("waiting for count down latch\n");
    latch_.wait();
    printf("all threads started\n");
    for (int i = 0; i < times; ++i)
    {
      char buf[32];
      snprintf(buf, sizeof buf, "hello %d", i);
      queue_.put(buf);
      printf("tid=%d, put data = %s, size = %zd\n", taf::CurrentThread::tid(), buf, queue_.size());
    }
  }

  void joinAll()
  {
    for (size_t i = 0; i < threads_.size(); ++i)
    {
      queue_.put("stop");
    }

    for_each(threads_.begin(), threads_.end(), boost::bind(&taf::Thread::join, _1));
  }

 private:

  void threadFunc()
  {
    printf("tid=%d, %s started\n",
           taf::CurrentThread::tid(),
           taf::CurrentThread::name());

    latch_.countDown();
    bool running = true;
    while (running)
    {
      std::string d(queue_.take());
      printf("tid=%d, get data = %s, size = %zd\n", taf::CurrentThread::tid(), d.c_str(), queue_.size());
      running = (d != "stop");
    }

    printf("tid=%d, %s stopped\n",
           taf::CurrentThread::tid(),
           taf::CurrentThread::name());
  }

  taf::BoundedBlockingQueue<std::string> queue_;
  taf::CountDownLatch latch_;
  boost::ptr_vector<taf::Thread> threads_;
};

int main()
{
  printf("pid=%d, tid=%d\n", ::getpid(), taf::CurrentThread::tid());
  Test t(5);
  t.run(100);
  t.joinAll();

  printf("number of created threads %d\n", taf::Thread::numCreated());
}
