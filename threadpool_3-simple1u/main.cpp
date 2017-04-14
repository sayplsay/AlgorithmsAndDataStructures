#include "threadpool.h"
#include <tr1/functional>
#include <stdio.h>

class TestTask: public common::Task {
public:
  typedef std::tr1::function<void()> CompleteHandler;
  TestTask(const CompleteHandler& complete_handler):complete_handler_(complete_handler) {}
  void operator()() {
    complete_handler_();
  }
  ~TestTask() {}
  
  private:
    CompleteHandler complete_handler_;
};

void Print(int num) {
  printf("this is processing = %d\n", num);
}

int main() {  
  int thread_num = 20;
  common::ThreadPool threadpool(thread_num);
  threadpool.Init();
  for(int i = 0; i < thread_num; ++i) {
    boost::shared_ptr<TestTask> task(new TestTask(std::tr1::bind(Print, i)));
    threadpool.AddTask(task);
  } 
  sleep(10);
  return 0;
} 


/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */
