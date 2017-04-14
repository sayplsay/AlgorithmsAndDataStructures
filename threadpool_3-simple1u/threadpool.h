#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdint.h>
#include <queue>
#include <pthread.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
namespace common {
  class Task {
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
    bool AddMultiTasks(std::vector<boost::shared_ptr<Task> > tasks);
  private:
    void Run();
  private:
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    std::queue<boost::shared_ptr<Task> > tasks_;
    boost::thread *threads;
    uint32_t max_threads_;
    bool want_exit_;
  };

}
/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */

#endif // CLOUD_UCS_THREAD_POOL_H_
