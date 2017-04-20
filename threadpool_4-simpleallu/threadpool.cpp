#include "threadpool.h"
#include <tr1/functional>
#include <boost/thread.hpp>

using namespace std;
namespace common {
  ThreadPool::ThreadPool(uint32_t max_threads):max_threads_(max_threads)
  ,want_exit_(false)
  ,count_(0) {
  }

  ThreadPool::~ThreadPool() {
    want_exit_ = true;
    pthread_mutex_lock(&mutex_);

    if (count_ > 0) {
      want_exit_ = true;
      for(int i = 0; i < max_threads_; ++i) {
        pthread_cond_broadcast(&cond_);
      }
      while (count_ > 0) {
        pthread_cond_wait(&cond_, &mutex_);
      }
    }
    
    pthread_mutex_unlock(&mutex_);
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_);
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


    for (unsigned int i = 0; i < max_threads_; ++i) {
      boost::shared_ptr<boost::thread> thread (new boost::thread(std::tr1::bind(&ThreadPool::Run, this)));
      thread->detach();
      count_++;
    }
    
    return true;
  }

  void ThreadPool::Run() {
    while(1) {
      pthread_mutex_lock(&mutex_);
      while (tasks_.empty()) {
        if (want_exit_) {
          if (--count_ == 0 ) {
            pthread_cond_broadcast(&cond_);
          }
          pthread_mutex_unlock(&mutex_);
          return;
        }
        pthread_cond_wait(&cond_, &mutex_);
      }

      boost::shared_ptr<Task> task = tasks_.front();
      tasks_.pop();
      pthread_mutex_unlock(&mutex_);
      if (NULL != task) {
        (*task)();
      } 
    }
  }

  bool ThreadPool::AddTask(boost::shared_ptr<Task> task) {
    int status;
    status = pthread_mutex_lock(&mutex_);
    if ( 0 != status) {
        return false;
    }
    tasks_.push(task);
    pthread_cond_broadcast(&cond_);
    pthread_mutex_unlock(&mutex_);
    return true;
  }

  bool ThreadPool::AddMultiTasks(std::vector<boost::shared_ptr<Task> > tasks) {
    int status;
    status = pthread_mutex_lock(&mutex_);
    if ( 0 != status) {
      return false;
    }
    for(unsigned int i = 0; i < tasks.size(); ++i) {
      tasks_.push(tasks[i]);
      pthread_cond_broadcast(&cond_);
    }
    pthread_mutex_unlock(&mutex_);
    return true;
  }

}
/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */
