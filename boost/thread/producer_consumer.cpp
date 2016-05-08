/*
[多线程]生产者消费者模型的BOOST实现
http://blog.csdn.net/bendanban/article/details/50681114
http://www.it165.net/pro/html/201602/61109.html
libuv生产者，消费者模型
http://www.it165.net/pro/html/201504/39247.html#
*/
#include <iostream>
#include <queue>

#include "boost/thread.hpp"

#include "boost/thread/condition.hpp"
#include "boost/thread/mutex.hpp"



template<class Product>
class Repository
{
public:
  Repository() : _capacity(2), _emptynum(_capacity){}
  Repository(int capacity) : _emptynum(capacity), _capacity(capacity){}

  // success : when new_capacity > _capacity - _emptynum
  bool set_capacity(int new_capacity){
    boost::mutex::scoped_lock lock();
    if (_capacity - _emptynum < new_capacity ){      
      _emptynum = new_capacity - _capacity + _emptynum;
      _capacity = new_capacity;
      return true;
    }
    return false;
  }

  bool empty(){
    boost::mutex::scoped_lock lock(_mutexer);
    return _is_empty();
  }

  void put(Product &product){
    {
      boost::mutex::scoped_lock lock(_mutexer);
      while (_is_full()){
        // unlock _mutexer and blocked until _condition_put.notifiy_one/all is called.
        _condition_put.wait(_mutexer); 
      }
      _products.push(product); // need implement the copy constructor and = 
      _emptynum--; // decrease empty position
    }
    _condition_get.notify_one(); // have put one, one another thread can get product now
  }

  void get(Product &product){
    {
      // lock this repository
      boost::mutex::scoped_lock lock(_mutexer);
      while (_is_empty()){
        // unlock _mutexer and blocked until _condition_put.notifiy_one/all is called.
        _condition_get.wait(_mutexer);
      }
      product = _products.front(); // need implement the copy constructor and = 
      _products.pop();
      _emptynum++; // increase empty position
    }
    // have removed one product, one another thread can put product now
    _condition_put.notify_one(); 
  }

private:
  inline bool _is_full (){ return _emptynum ==         0; } // if have empty position
  inline bool _is_empty(){ return _emptynum == _capacity; } // 

private:
  int _capacity; // capacity of this repository
  int _emptynum; // number of empty position
  std::queue<Product> _products; // products in a FIFO queue

  boost::mutex _mutexer; // race access
  boost::condition _condition_put; 
  boost::condition _condition_get;
};



boost::mutex g_io_mutexer;

boost::mutex g_mutexer;
bool g_is_finished = false;

Repository<int> g_repository(4);



void producing(){
  for (int i = 0; i < 100; i++){
    {
      boost::mutex::scoped_lock lock(g_io_mutexer);
      std::cout << "Producing product : " << i << std::endl;
    }
    g_repository.put(i);
  }
  boost::mutex::scoped_lock lock(g_mutexer);
  g_is_finished = true;
}

void consuming(){
  int product;
  while (true){
    {
      boost::mutex::scoped_lock lock(g_mutexer);
      if (g_is_finished && g_repository.empty()){
        break;
      }
    }
    g_repository.get(product);
    {
      boost::mutex::scoped_lock lock(g_io_mutexer);
      std::cout << "Consuming product : " << product << std::endl;
    }
  }
}



int main(){
    boost::thread producer(producing);
    boost::thread consumer_1(consuming);
    boost::thread consumer_2(consuming);
    producer.join();
    consumer_1.join();
    consumer_2.join();
    return 0;
}
