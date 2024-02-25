#ifndef MYSQLDRIVER_H_
#define MYSQLDRIVER_H_

#include <atomic>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <driver.h>
#include <memory>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>

namespace db_pool {
namespace db_driver {

class MYSQLDriver : public Driver {
public:
  MYSQLDriver(const types::ConnectionProperties &properties)
      : connection_properties(properties) {}

  virtual ~MYSQLDriver() {
    if (conn != nullptr) {
      Disconnect();
      delete conn;
    }
  }

  virtual bool Connect() override;

  virtual bool Disconnect() override;

  inline virtual bool IsConnected() override {
    return conn == nullptr ? false : conn->isValid();
  }

  virtual types::SqlResults
  ExecuteSelect(const std::string &query, const std::vector<std::any> &params,
                std::vector<std::string> keys) override;

  virtual types::SqlResults
  ExecuteSelect(const std::string &query,
                std::vector<std::string> keys) override;

  virtual bool Execute(const std::string &query,
                       const std::vector<std::any> &params) override;

  virtual int ExecuteUpdate(const std::string &query,
                            const std::vector<std::any> &params) override;

  virtual bool Execute(const std::string &query) override;

  virtual int ExecuteUpdate(const std::string &query) override;

private:
  void QueryBuilder(sql::PreparedStatement &pstmt, std::size_t index,
                    std::any arg);

  types::ConnectionProperties connection_properties;
  std::atomic_bool is_connected;
  sql::Driver *driver = nullptr;
  sql::Connection *conn = nullptr;
};
} // namespace db_driver
} // namespace db_pool
#endif
