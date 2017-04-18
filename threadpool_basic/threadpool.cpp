#include "threadpool.h"
#include <tr1/functional>
#include <boost/thread.hpp>

namespace common {
  using namespace std;

  ThreadPool::ThreadPool(uint32_t max_threads, uint32_t max_queue)
    : quit_(false)
    , max_threads_(max_threads)
    , counter_(0)
    , idle_(0)
    , idle_interval_(5)
    , queue_length_(0)
    , max_queue_length_(max_queue) {
  }

  ThreadPool::~ThreadPool() {
    Destroy();
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_);
  }

  void ThreadPool::Destroy() {
    int status;

    if (pthread_mutex_lock(&mutex_) != 0) {
      return;
    }

    if (counter_ > 0) {
      quit_ = true;
      if (idle_ > 0) {
        status = pthread_cond_broadcast(&cond_);
        if (status != 0) {
          pthread_mutex_unlock(&mutex_);
          return;
        }
      }

      while (counter_ > 0) {
        status = pthread_cond_wait(&cond_, &mutex_);
        if (status != 0) {
          pthread_mutex_unlock(&mutex_);
          return;
        }
      }
    }

    pthread_mutex_unlock(&mutex_);
  }

  bool ThreadPool::Init() {
    int status;

    status = pthread_mutex_init(&mutex_, NULL);
    if (status != 0) {
      return false;
    }

    status = pthread_cond_init(&cond_, NULL);
    if (status != 0) {
      pthread_mutex_destroy(&mutex_);
      return false;
    }

    return true;
  }

  bool ThreadPool::AddTask(boost::shared_ptr<Task> task) {
    int status;

    // 对队列的大小有限制
    if (max_queue_length_ != 0 && tasks_.size() >= max_queue_length_) {
      return false;
    }

    status = pthread_mutex_lock(&mutex_);
    if (status != 0) {
      return false;
    }

    tasks_.push(task);

    // 当idle_大于0时，代表有空闲的线程在监听条件变量，这样就可以不用创建新的thread了
    if (idle_ > 0) {
      status = pthread_cond_signal(&cond_);
      if (status != 0) {
        pthread_mutex_unlock(&mutex_);
        return false;
      }
    } else if (counter_ < max_threads_) { 
      // 创建新的thread
      boost::shared_ptr<boost::thread> thread(new boost::thread(std::tr1::bind(&ThreadPool::WorkerMain, this)));
      thread->detach();
      counter_++;
    } 
    pthread_mutex_unlock(&mutex_);
    return true;
  }

  void ThreadPool::WorkerMain() {
    while (1) {
      bool timedout = false;
      int status;

      if (pthread_mutex_lock(&mutex_)) {
        return;
      }

      while (tasks_.empty()) {
        if (quit_) {
          if (--counter_ == 0) {
            pthread_cond_broadcast(&cond_);
          }

          pthread_mutex_unlock(&mutex_);
          return;
        }

        if (timedout) {
          counter_--;
          pthread_mutex_unlock(&mutex_);
          return;
        }

        struct timespec timeout;
        timeout.tv_sec = time(NULL) + idle_interval_;
        timeout.tv_nsec = 0;
        
        idle_++;
        status = pthread_cond_timedwait(&cond_, &mutex_, &timeout);
        idle_--;

        if (status == ETIMEDOUT) {
          timedout = true;
          continue;
        } else if (status != 0) {
          counter_--;
          pthread_mutex_unlock(&mutex_);
          return;
        } 
      }

      boost::shared_ptr<Task> task = tasks_.front();
      tasks_.pop();
      pthread_mutex_unlock(&mutex_);

      if (task != NULL) {
        (*task)();
      }
    }
  }

}
/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */
