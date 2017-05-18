#include "tc_threadlocalsingleton.h"
#include "tc_current_thread.h"
#include "tc_thread.h"

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <stdio.h>

class Test : boost::noncopyable
{
 public:
  Test()
  {
    printf("tid=%d, constructing %p\n", taf::CurrentThread::tid(), this);
  }

  ~Test()
  {
    printf("tid=%d, destructing %p %s\n", taf::CurrentThread::tid(), this, name_.c_str());
  }

  const std::string& name() const { return name_; }
  void setName(const std::string& n) { name_ = n; }

 private:
  std::string name_;
};

void threadFunc(const char* changeTo)
{
  printf("tid=%d, %p name=%s\n",
         taf::CurrentThread::tid(),
         &taf::ThreadLocalSingleton<Test>::instance(),
         taf::ThreadLocalSingleton<Test>::instance().name().c_str());
  taf::ThreadLocalSingleton<Test>::instance().setName(changeTo);
  printf("tid=%d, %p name=%s\n",
         taf::CurrentThread::tid(),
         &taf::ThreadLocalSingleton<Test>::instance(),
         taf::ThreadLocalSingleton<Test>::instance().name().c_str());

  // no need to manually delete it
  // taf::ThreadLocalSingleton<Test>::destroy();
}

int main()
{
  taf::ThreadLocalSingleton<Test>::instance().setName("main one");
  taf::Thread t1(boost::bind(threadFunc, "thread1"));
  taf::Thread t2(boost::bind(threadFunc, "thread2"));
  t1.start();
  t2.start();
  t1.join();
  printf("tid=%d, %p name=%s\n",
         taf::CurrentThread::tid(),
         &taf::ThreadLocalSingleton<Test>::instance(),
         taf::ThreadLocalSingleton<Test>::instance().name().c_str());
  t2.join();

  pthread_exit(0);
}
