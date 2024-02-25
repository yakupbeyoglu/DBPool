#ifndef TYPES_H_
#define TYPES_H_
#include <algorithm>
#include <ctime>   // For std::tm
#include <iomanip> // For std::get_time
#include <map>     // For std::map
#include <sstream> // For std::istringstream
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

/**
 * @brief A struct representing a value that can be converted to various types.
 */
struct SqlValue {
  std::string value; /**< The raw string value. */
  SqlValue(const std::string &value) : value(value) {}

  /**
   * @brief Constructor to initialize the SqlValue with a given string value.
   * @param value The string value to initialize the SqlValue.
   */
  SqlValue(const SqlValue &other) : value(other.value) {}

  /**
   * @brief Move constructor.
   * @param other The rvalue reference to the SqlValue object to move from.
   */
  SqlValue(SqlValue &&other) noexcept : value(std::move(other.value)) {
    other.value.clear();
  }

  /**
   * @brief Assignment operator using the Copy and Swap idiom.
   * @param other The SqlValue object to assign from.
   * @return Reference to the modified SqlValue object.
   */
  SqlValue &operator=(SqlValue other) {
    swap(*this, other);
    return *this;
  }

  /**
   * @brief Convert the raw string to an integer.
   * @return The converted integer value. If conversion fails, returns 0.
   */
  const std::string &Get() const { return value; }

  /**
   * @brief Convert the raw string to an integer.
   * @return The converted integer value. If conversion fails, returns 0.
   */
  int toInt() const {
    try {
      return std::stoi(value);
    } catch (...) {
      return 0; // Return default value if conversion fails
    }
  }

  /**
   * @brief Convert the raw string to an int 64.
   * @return The converted integer value. If conversion fails, returns 0.
   */
  int64_t toInt64() const {
    try {
      return std::stoll(value);
    } catch (...) {
      return 0; // Return default value if conversion fails
    }
  }

  /**
   * @brief Convert the raw string to a double.
   * @return The converted double value. If conversion fails, returns 0.0.
   */
  double toDouble() const {
    try {
      return std::stod(value);
    } catch (...) {
      return 0.0; // Return default value if conversion fails
    }
  }

  /**
   * @brief Convert the raw string to a boolean.
   * @return The converted boolean value. Returns true if the raw string is
   * "true" or "1", false otherwise.
   */
  bool toBool() const { return value == "true" || value == "1"; }

  /**
   * @brief Convert the raw string to a std::tm (timestamps).
   * @return The converted timestamp value. If conversion fails, returns a
   * default non-meaningful timestamp.
   */
  std::tm toTimestamp() const {
    std::tm timestamp = {};
    std::istringstream ss(value);
    ss >> std::get_time(&timestamp, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
      // If parsing fails, return a default non-meaningful timestamp
      return std::tm{};
    }
    return timestamp;
  }

  /**
   * @brief Swap function to efficiently exchange values between two SqlValue
   * objects.
   * @param first The first SqlValue object.
   * @param second The second SqlValue object.
   */
  friend void swap(SqlValue &first, SqlValue &second) noexcept {
    using std::swap;
    swap(first.value, second.value);
  }
};

using SqlBlock = std::map<std::string, SqlValue>;

using SqlResults = std::vector<SqlBlock>;

} // namespace types
} // namespace db_pool

#endif
