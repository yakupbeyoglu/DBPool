#ifndef TYPES_H_
#define TYPES_H_

namespace db_pool {
namespace types {
enum class Driver : short { MYSQL = 0, POSTGRESQL, MSSQL };

enum class ExecuteType : short {
  SELECT = 0,
  UPDATE = 1 // create, update, delete which effect to rows
};

struct ConnectionProperties {
  std::string host_name; // port should included
  std::string username;
  std::string password;
  std::string schema;
};
} // namespace types
} // namespace db_pool

#endif
