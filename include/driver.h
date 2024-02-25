#ifndef DRIVER_H_
#define DRIVER_H_

#include "types.h"
#include <any>
#include <string>
#include <vector>

namespace db_pool {
namespace db_driver {

/**
 * @file driver.h
 * @brief Interface for interacting with a database driver.
 *
 * This class defines methods for connecting to a database,
 * executing queries, and checking the connection status, so all basic
 * operations for the database drivers.
 *
 */
class Driver {
public:
  /**
   * @brief Establishes a connection to the database.
   *
   * @return true if the connection is successful, false otherwise.
   */
  virtual bool Connect() = 0;

  /**
   * @brief Closes the connection to the database.
   *
   * @return true if disconnection is successful, false otherwise.
   */
  virtual bool Disconnect() = 0;

  /**
   * @brief Checks if the driver is currently connected to the database.
   *
   * @return true if the driver is connected, false otherwise.
   */
  virtual bool IsConnected() = 0;

  /**
   * @brief Executes a SELECT query on the database.
   *
   * This method executes a SELECT query with the provided SQL string and
   * optional parameters, and returns the result set.
   *
   * @param query The SQL SELECT query to execute.
   * @param params Variable number of string parameters to bind to the query.
   * @param keys, Keys that should be mapped for results
   * @return The result set obtained from executing the query.
   */
  virtual types::SqlResults ExecuteSelect(const std::string &query,
                                          const std::vector<std::any> &params,
                                          std::vector<std::string> keys) = 0;

  /**
   * @brief Executes a SELECT query on the database.
   *
   * This method does not use prepared statement and can cause SQL injection.
   *
   * @param query The SQL SELECT query to execute.
   * @param keys, Keys that should be mapped for results
   * @return The result set obtained from executing the query.
   */
  virtual types::SqlResults ExecuteSelect(const std::string &query,
                                          std::vector<std::string> keys) = 0;

  /**
   * @brief Executes a non-SELECT query on the database.
   *
   * This method executes a non-SELECT query as prepared statement (e.g.,
   * INSERT, UPDATE, DELETE) with the provided SQL string and optional
   * parameters.
   *
   * @param query The SQL query to execute.
   * @param params Variable number of string parameters to bind to the query.
   * @return true if the query execution is successful, false otherwise.
   */
  virtual bool Execute(const std::string &query,
                       const std::vector<std::any> &params) = 0;

  /**
   * @brief Executes a non-SELECT query on the database.
   *
   * This method executes a non-SELECT query as prepared statement (e.g.,
   * INSERT, UPDATE, DELETE) with the provided SQL string and optional
   * parameters.
   *
   * @param query The SQL query to execute.
   * @param params Variable number of parameters to bind to the query.
   * @return The number of rows affected by the query execution.
   */
  virtual int ExecuteUpdate(const std::string &query,
                            const std::vector<std::any> &params) = 0;

  /**
   * @brief Executes a non-SELECT query on the database.
   *
   * This method executes a non-SELECT query with given command(e.g., INSERT,
   * UPDATE, DELETE) with the provided SQL string and optional parameters.
   *
   * Note that it can be not good and cause for SQL injection.
   *
   * @param query The SQL query to execute.
   * @return true if the query execution is successful, false otherwise.
   */
  virtual bool Execute(const std::string &query) = 0;

  /**
   * @brief Executes a non-SELECT query on the database.
   *
   * This method executes a non-SELECT query as prepared statement (e.g.,
   * INSERT, UPDATE, DELETE) with the provided SQL string.
   *
   * @param query The SQL query to execute.
   * @return The number of rows affected by the query execution.
   */
  virtual int ExecuteUpdate(const std::string &query) = 0;
};

} // namespace db_driver
} // namespace db_pool
#endif // DRIVER_H_
