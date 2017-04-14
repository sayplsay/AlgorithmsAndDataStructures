#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdint.h>
#include <queue>
#include <pthread.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp> 
namespace common {
  class Task {    // 需继承此类来添加task
  public:
      Task() {}
      virtual ~Task() {}
      virtual void operator()() = 0;
  };

  class ThreadPool {
  public:
    ThreadPool(uint32_t max_threads);
    ~ThreadPool();

    bool Init();
    bool AddTask(boost::shared_ptr<Task> task);
  private:
    void Run();
  private:
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    std::queue<boost::shared_ptr<Task> > tasks_; // task共享队列
    boost::thread *threads; // 创建的多个线程
    uint32_t max_threads_;
  };

}
/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */

#endif
