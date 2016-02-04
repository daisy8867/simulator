#include <queue>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

class thread_pool
{
private:
  std::queue< boost::function< void(int) > > tasks_;
  boost::thread_group threads_;
  std::size_t available_;
  boost::mutex mutex_;
  boost::condition_variable condition_;
  bool running_;
public:

  /// @brief Constructor.
  thread_pool( std::size_t pool_size );
  /// @brief Destructor.
  ~thread_pool();
  /// @brief Add task to the thread pool if a thread is currently available.
  template < typename Task >
  void run_task( Task task );
private:
  /// @brief Entry point for pool threads.
  void pool_main();
};
