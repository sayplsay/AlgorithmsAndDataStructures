#include "tc_threadpool.h"
#include "tc_countdownlatch.h"
#include "tc_current_thread.h"

#include <boost/bind.hpp>
#include <stdio.h>
#include <iostream>
void print()
{
  printf("tid=%d\n", taf::CurrentThread::tid());
}

void printString(const std::string& str)
{
  std::cout << str << std::endl;
  usleep(100*1000);
}

void test(int maxSize)
{
  std::cout << "Test ThreadPool with max queue size = " << maxSize << std::endl;
  taf::ThreadPool pool("MainThreadPool");
  pool.setMaxQueueSize(maxSize);
  pool.start(5);

  std::cout << "Adding" << std::endl;
  pool.run(print);
  pool.run(print);
  for (int i = 0; i < 100; ++i)
  {
    char buf[32];
    snprintf(buf, sizeof buf, "task %d", i);
    pool.run(boost::bind(printString, std::string(buf)));
  }
  std::cout << "Done" << std::endl;

  taf::CountDownLatch latch(1);
  pool.run(boost::bind(&taf::CountDownLatch::countDown, &latch));
  latch.wait();
  pool.stop();
}

int main()
{
  test(0);
  test(1);
  test(5);
  test(10);
  test(50);
}
