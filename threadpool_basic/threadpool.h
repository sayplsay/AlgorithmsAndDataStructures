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
      ThreadPool(uint32_t max_threads, uint32_t max_queue = 0);
      ~ThreadPool();

      bool Init();
      void Destroy();
      bool AddTask(boost::shared_ptr<Task> task);

  private:
      void WorkerMain();

  private:
      pthread_mutex_t mutex_;
      pthread_cond_t cond_;
      std::queue<boost::shared_ptr<Task> > tasks_;
      bool quit_;  // 退出标识
      uint32_t max_threads_;
      uint32_t counter_; // 已经创建的所有thread个数
      int idle_;    // 等待在条件变量thread个数
      int idle_interval_;
      uint32_t queue_length_;
      uint32_t max_queue_length_;
  };

}
/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */

#endif
