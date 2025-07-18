#pragma once

#include <string>
#include <vector>
#include <optional>
#include <variant>

namespace tpcc {

enum class ColumnType {
    INT,    // int
    FLOAT,  // float
    CHAR    // char(n)
};

struct Column {
    std::string name;        // 列名
    ColumnType type;         // 列类型
    std::optional<int> size; // 对于CHAR类型，表示长度；对于其他类型可选

    Column(const std::string &name, ColumnType type, std::optional<int> size = std::nullopt)
        : name(name), type(type), size(size) {}

    std::string getSqlType() const {
        switch (type) {
            case ColumnType::INT:
                return "int";
            case ColumnType::FLOAT:
                return "float";
            case ColumnType::CHAR:
                return "char(" + std::to_string(size.value()) + ")";
        }
        return "";
    }
};

// 数据值类型
using Value = std::variant<int64_t, float, std::string>;

// 数据记录(一行数据)
using Record = std::vector<Value>;

// 表的Schema
class Schema {
private:
    std::string table_name_;
    std::vector<Column> columns_;

public:
    Schema(const std::string& table_name) : table_name_(table_name) {}

    void addColumn(const std::string& name, ColumnType type, std::optional<int> size = std::nullopt) {
        columns_.emplace_back(name, type, size);
    }

    const std::string& getTableName() const { return table_name_; }

    const std::vector<Column>& getColumns() const { return columns_; }

    size_t getColumnCount() const { return columns_.size(); }

    const Column& getColumn(size_t index) const { return columns_[index]; }
};

} // namespace tpcc