#ifndef DRIVER_POOL_H_
#define DRIVER_POOL_H_

#include "driver_factory.h"
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace db_pool {

/**
 * @brief Database connection pool for managing database connections.
 *
 * This class manages a pool of database connections to optimize connection
 * creation and reuse. It provides methods to obtain and return database
 * connections.
 *
 * @tparam SqlResult Type representing the result set from a database query.
 */
template <typename SqlResult> class DatabaseConnectionPool {
public:
  /**
   * @brief Constructs a new DatabaseConnectionPool object.
   *
   * @param type The type of database driver to be used.
   * @param properties Connection properties for establishing database
   * connections.
   * @param pool_size The maximum size of the connection pool.
   */
  DatabaseConnectionPool(types::Driver type,
                         const types::ConnectionProperties &properties,
                         size_t pool_size)
      : driver_type(type), connection_properties(properties),
        pool_size(pool_size) {
    // Create database connections and add them to the pool
    for (size_t i = 0; i < pool_size; ++i) {
      auto connection = db_driver::DriverFactory::createDriver<SqlResult>(
          driver_type, properties);
      connection->Connect();
      pool.push(connection);
    }
  }

  /**
   * @brief Get a database connection from the pool.
   *
   * This method retrieves a database connection from the pool. If no connection
   * is available, it waits for one to become available or until a timeout
   * occurs.
   *
   * @param timeout Timeout value of receiving connection from the pool, default
   * is 30 seconds
   *
   * @return A shared pointer to a database connection, in caase of timeout it
   * will return nullptr
   *
   */
  std::shared_ptr<db_driver::Driver<SqlResult>>
  getConnection(std::size_t timeout = 30) {
    std::unique_lock<std::mutex> lock(mutex);
    // Use a predicate function to check if the pool is empty
    auto predicate = [&] { return !pool.empty(); };
    // Wait until the pool is not empty or timeout occurs
    if (!cv.wait_for(lock, std::chrono::seconds(timeout), predicate)) {
      // Timeout occurred, return nullptr
      return nullptr;
    }
    // Connection is available, retrieve and return it
    auto connection = pool.front();
    // delete connection from pool
    pool.pop();
    return connection;
  }

  /**
   * @brief Return a database connection to the pool.
   *
   * This method returns a database connection to the pool, making it available
   * for reuse.
   *
   * @param connection The database connection to return to the pool.
   */
  void
  returnConnection(std::shared_ptr<db_driver::Driver<SqlResult>> connection) {
    std::lock_guard<std::mutex> lock(mutex);
    if (pool.size() + 1 > pool_size)
      throw std::out_of_range(
          "Given connection cause to push more than the pool limit !");
    if (connection == nullptr)
      throw std::runtime_error("pushed connection is nullptr!");
    // If connection is not connected, connect and repush
    if (!connection->IsConnected())
      connection->Connect();
    pool.push(connection);
    cv.notify_one();
  }

private:
  types::Driver driver_type; /**< The type of database driver. */
  types::ConnectionProperties
      connection_properties; /**< Connection properties for establishing
                                database connections. */
  size_t pool_size;          /**< The maximum size of the connection pool. */
  std::queue<std::shared_ptr<db_driver::Driver<SqlResult>>>
      pool; /**< Queue for storing database connections. */
  std::mutex
      mutex; /**< Mutex for synchronizing access to the connection pool. */
  std::condition_variable
      cv; /**< Condition variable for waiting on available connections. */
};

} // namespace db_pool

#endif // DRIVER_POOL_H_
