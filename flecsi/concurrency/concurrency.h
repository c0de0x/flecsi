/*~--------------------------------------------------------------------------~*
 *  @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
 * /@@/////  /@@          @@////@@ @@////// /@@
 * /@@       /@@  @@@@@  @@    // /@@       /@@
 * /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
 * /@@////   /@@/@@@@@@@/@@       ////////@@/@@
 * /@@       /@@/@@//// //@@    @@       /@@/@@
 * /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
 * //       ///  //////   //////  ////////  //
 *
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_concurrency_h
#define flecsi_concurrency_h

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

/*!
 * \file concurrency.h
 * \authors nickm
 * \date Initial file creation: May 12, 2016
 */

namespace flecsi
{
  class virtual_semaphore{
  public:
    using lock_t = std::unique_lock<std::mutex>;

    virtual_semaphore(int count=0)
    : count_(count),
    max_count_(0){
      done_ = false;
    }
    
    virtual_semaphore(int count, int max_count)
    : count_(count),
    max_count_(max_count){
      done_ = false;
    }
    
    ~virtual_semaphore(){}
    
    bool acquire(){
      if(done_){
        return false;
      }

      lock_t lock(mutex_);
      
      while(count_ <= 0){
        cond_.wait(lock);
      }
      
      --count_;
      
      return true;
    }
    
    bool try_acquire(){
      if(done_){
        return false;
      }

      lock_t lock(mutex_);

      if(count_ > 0){
        --count_;
        return true;
      }

      return false;
    }
    
    void release(){
      lock_t lock(mutex_);

      if(max_count_ == 0 || count_ < max_count_){
        ++count_;
      }

      cond_.notify_one();
    }

    void interrupt(){
      done_ = true;
      cond_.notify_all();
    }
    
    virtual_semaphore& operator=(const virtual_semaphore&) = delete;
    
    virtual_semaphore(const virtual_semaphore&) = delete;

  private:
    std::mutex mutex_;
    std::condition_variable cond_;

    int count_;
    int max_count_;
    std::atomic_bool done_;
  };

  class thread_pool{
  public:
    using function_type = std::function<void(void*)>;

    thread_pool(){
      done_ = false;
    }

    void run_(){
      for(;;){
        sem_.acquire();
        mutex_.lock();

        if(done_){
          mutex_.unlock();
          return;
        }

        auto item = queue_.front();
        queue_.pop_front();
        mutex_.unlock();
        item.f(item.args);
      }
    }

    void queue(function_type f, void* args = nullptr){
      mutex_.lock();
      queue_.emplace_back(item(f, args));
      mutex_.unlock();
      sem_.release();
    }

    void start(){
      auto t = new std::thread(&thread_pool::run_, this);
      threads_.push_back(t);
    }

    void join(){
      done_ = true;
      sem_.interrupt();

      for(auto t : threads_){
        t->join();
      }
    }

  private:
    struct item{
      item(function_type f, void* args)
      : f(f),
      args(args){}

      function_type f;
      void* args;
    };

    std::mutex mutex_;
    std::deque<item> queue_;
    std::vector<std::thread*> threads_;
    virtual_semaphore sem_;
    std::atomic_bool done_;    
  };

} // namespace flecsi

#endif // flecsi_concurrency_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
