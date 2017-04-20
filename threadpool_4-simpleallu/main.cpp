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
  for (int j = 0; j < 50; ++j) {
    int thread_num = 20;
    std::cout << "num = xxxxxx" << j << std::endl;
    std::cout << "init 1111111111" << std::endl;
    common::ThreadPool threadpool(thread_num);
    std::cout << "init 333333333" << std::endl;
    threadpool.Init();
    std::cout << "init 2222222222" << std::endl;
    for(int i = 0; i < 30; ++i) {
      boost::shared_ptr<TestTask> task(new TestTask(boost::bind(Print, i)));
      threadpool.AddTask(task);
    }
    std::cout << "num = " << j << std::endl;
  } 
  sleep(10);
  return 0;
} 


/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */
