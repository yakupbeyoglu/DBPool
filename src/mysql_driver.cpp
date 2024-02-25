#include "mysqldriver.h"
#include <algorithm>

namespace db_pool {
namespace db_driver {

bool MYSQLDriver::Connect() {
  try {
    if (conn == nullptr || conn->isClosed()) {
      driver = get_driver_instance();
      conn = driver->connect(connection_properties.host_name,
                             connection_properties.username,
                             connection_properties.password);
      conn->setSchema(connection_properties.schema);
    }
  } catch (const sql::SQLException &e) {
    throw std::runtime_error(
        std::string("Failed to connect to the database : ") + e.what());
  }

  return conn->isValid();
}

bool MYSQLDriver::Disconnect() {
  try {
    if (conn == nullptr || conn->isClosed())
      return false;
    conn->close();
  } catch (const sql::SQLException &e) {
    throw std::runtime_error(
        std::string("Failed to disconnect from the database : ") + e.what());
  }
  return conn->isClosed();
}

SqlResult MYSQLDriver::ExecuteSelect(const std::string &query,
                                     const std::vector<std::any> &params) {
  try {
    if (conn == nullptr || conn->isClosed())
      throw std::runtime_error(
          "The databsae connection is not valid to execute query !");

    if (std::count(std::begin(query), std::end(query), '?') != params.size())
      throw std::out_of_range("The number of parameter does not math with ? "
                              "inside of the prepareStatement");

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query));
    for (std::size_t i = 0; i < params.size(); ++i) {
      QueryBuilder(*pstmt, i, params[i]);
    }
    return SqlResult(pstmt->executeQuery());

  } catch (const sql::SQLException &e) {
    throw std::runtime_error(std::string("Error executing SELECT query: ") +
                             e.what());
  } catch (const std::exception &e) {
    throw std::runtime_error(
        std::string("Run time error on execute select : ") + e.what());
  }
}

SqlResult MYSQLDriver::ExecuteSelect(const std::string &query) {
  try {

    if (conn == nullptr || conn->isClosed())
      throw std::runtime_error(
          "The databsae connection is not valid to execute query !");
    std::unique_ptr<sql::Statement> pstmt(conn->createStatement());
    return SqlResult(pstmt->executeQuery(query));
  } catch (const sql::SQLException &e) {
    throw std::runtime_error(std::string("Error executing SELECT query: ") +
                             e.what());
  }
}

bool MYSQLDriver::Execute(const std::string &query,
                          const std::vector<std::any> &params) {
  try {
    if (conn == nullptr || conn->isClosed())
      throw std::runtime_error(
          "The databsae connection is not valid to execute query !");

    if (std::count(std::begin(query), std::end(query), '?') != params.size())
      throw std::out_of_range("The number of parameter does not math with ? "
                              "inside of the prepareStatement");

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query));
    for (std::size_t i = 0; i < params.size(); ++i) {
      QueryBuilder(*pstmt, i, params[i]);
    }

    return pstmt->execute();
  } catch (const sql::SQLException &e) {
    throw std::runtime_error(std::string("Error executing SELECT query: ") +
                             e.what());
  }
}

int MYSQLDriver::ExecuteUpdate(const std::string &query,
                               const std::vector<std::any> &params) {
  try {
    if (conn == nullptr || conn->isClosed())
      throw std::runtime_error(
          "The databsae connection is not valid to execute query !");

    if (std::count(std::begin(query), std::end(query), '?') != params.size())
      throw std::out_of_range("The number of parameter does not math with ? "
                              "inside of the prepareStatement");

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query));
    for (std::size_t i = 0; i < params.size(); ++i) {
      QueryBuilder(*pstmt, i, params[i]);
    }

    return pstmt->executeUpdate();
  } catch (const sql::SQLException &e) {
    throw std::runtime_error(
        std::string(
            "Error executing ExecuteUpdate Prepeeared statement query") +
        e.what());
  }
}

bool MYSQLDriver::Execute(const std::string &query) {
  try {
    if (conn == nullptr || conn->isClosed())
      throw std::runtime_error(
          "The databsae connection is not valid to execute query !");
    std::unique_ptr<sql::Statement> pstmt(conn->createStatement());
    return pstmt->execute(query);
  } catch (const sql::SQLException &e) {
    throw std::runtime_error(std::string("Error executing Execute query: ") +
                             e.what());
  }
}

int MYSQLDriver::ExecuteUpdate(const std::string &query) {
  try {
    if (conn == nullptr || conn->isClosed())
      throw std::runtime_error(
          "The databsae connection is not valid to execute query !");
    std::unique_ptr<sql::Statement> pstmt(conn->createStatement());
    return pstmt->executeUpdate(query);
  } catch (const sql::SQLException &e) {
    throw std::runtime_error(
        std::string("Error executing Execute Update query : ") + e.what());
  }
}

void MYSQLDriver::QueryBuilder(sql::PreparedStatement &pstmt, std::size_t index,
                               std::any arg) {
  if (index < 0)
    throw std::out_of_range(
        "The index in the prepeared statemet should be > 0");
  if (arg.type() == typeid(int))
    pstmt.setInt(index + 1, any_cast<int>(arg));
  else if (arg.type() == typeid(int32_t))
    pstmt.setInt(index + 1, any_cast<int32_t>(arg));
  else if (arg.type() == typeid(int64_t))
    pstmt.setInt(index + 1, any_cast<int64_t>(arg));
  else if (arg.type() == typeid(uint32_t))
    pstmt.setUInt(index + 1, any_cast<uint32_t>(arg));
  else if (arg.type() == typeid(uint64_t))
    pstmt.setUInt64(index + 1, any_cast<uint64_t>(arg));
  else if (arg.type() == typeid(double))
    pstmt.setDouble(index + 1, any_cast<double>(arg));
  else if (arg.type() == typeid(float))
    pstmt.setDouble(index + 1, any_cast<float>(arg));
  else if (arg.type() == typeid(std::string))
    pstmt.setString(index + 1, any_cast<std::string>(arg));
  else if (arg.type() == typeid(std::string))
    pstmt.setString(index + 1, any_cast<std::string>(arg));
  else if (arg.type() == typeid(const char *))
    pstmt.setString(index + 1, any_cast<const char *>(arg));
  else if (arg.type() == typeid(sql::SQLString))
    pstmt.setString(index + 1, any_cast<sql::SQLString>(arg));
  else if (arg.type() == typeid(bool))
    pstmt.setBoolean(index + 1, any_cast<bool>(arg));
  else
    throw std::runtime_error("Query builder receive unexpected data type !");
}

} // namespace db_driver
} // namespace db_pool
