#ifndef DRIVER_FACTORY_H_
#define DRIVER_FACTORY_H_
#include "driver.h"
#include "mysqldriver.h"
#include <memory>
namespace db_pool {
namespace db_driver {
class DriverFactory {
public:
  template <typename SqlResult>
  static std::shared_ptr<Driver<SqlResult>>
  createDriver(types::Driver type,
               const types::ConnectionProperties &properties) {
    switch (type) {
    case types::Driver::MYSQL:
      return std::make_shared<MYSQLDriver>(properties);
    // Add cases for other driver types if needed
    default:
      throw std::invalid_argument("Unsupported database driver type.");
    }
  }
};
} // namespace db_driver
} // namespace db_pool

#endif
