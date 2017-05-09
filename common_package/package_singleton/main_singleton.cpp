#include "tc_singleton.h"
#include "tc_current_thread.h"
#include "tc_thread.h"

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

class TestNoDestroy : boost::noncopyable
{
 public:
  // Tag member for Singleton<T>
  void no_destroy();

  TestNoDestroy()
  {
    printf("tid=%d, constructing TestNoDestroy %p\n", taf::CurrentThread::tid(), this);
  }

  ~TestNoDestroy()
  {
    printf("tid=%d, destructing TestNoDestroy %p\n", taf::CurrentThread::tid(), this);
  }
};

void threadFunc()
{
  printf("tid=%d, %p name=%s\n",
         taf::CurrentThread::tid(),
         &taf::Singleton<Test>::instance(),
         taf::Singleton<Test>::instance().name().c_str());
  taf::Singleton<Test>::instance().setName("only one, changed");
}

int main()
{
  taf::Singleton<Test>::instance().setName("only one");
  taf::Thread t1(threadFunc);
  t1.start();
  t1.join();
  printf("tid=%d, %p name=%s\n",
         taf::CurrentThread::tid(),
         &taf::Singleton<Test>::instance(),
         taf::Singleton<Test>::instance().name().c_str());
  taf::Singleton<TestNoDestroy>::instance();
  printf("with valgrind, you should see %zd-byte memory leak.\n", sizeof(TestNoDestroy));
}
