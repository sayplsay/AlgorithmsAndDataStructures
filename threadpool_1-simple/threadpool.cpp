#include "threadpool.h"
#include <tr1/functional>
#include <boost/thread.hpp>

using namespace std;
namespace common {
  ThreadPool::ThreadPool(uint32_t max_threads):max_threads_(max_threads) {
  }

  ThreadPool::~ThreadPool() {
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_);
    delete[] threads;
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

    threads = new (std::nothrow) boost::thread[max_threads_];
    if (NULL == threads) {
      pthread_mutex_destroy(&mutex_);
      pthread_cond_destroy(&cond_);
      return false;
    }

    for (unsigned int i = 0; i < max_threads_; ++i) {
      threads[i] = boost::thread(&ThreadPool::Run, this);
      threads[i].detach();
    }
    
    return true;
  }

  void ThreadPool::Run() {
    while(1) {
      pthread_mutex_lock(&mutex_);
      while (0 == tasks_.size()) {
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
}
/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */
