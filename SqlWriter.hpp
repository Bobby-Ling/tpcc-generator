#pragma once

#include "Writer.hpp"
#include <fstream>

namespace tpcc {

class SqlWriter : public Writer {
  private:
    std::ofstream out_;
    const Schema *current_schema_;  // 当前表的Schema

    // 辅助方法：转义SQL字符串
    std::string escapeSqlString(const std::string &str);

    // 辅助方法：格式化值为SQL格式
    std::string formatValue(const Value &value, ColumnType expected_type);

  public:
    SqlWriter(const std::string &output_path);

    // 实现Writer接口
    void beginTable(const Schema &schema) override;
    void writeRecord(const Record &record) override;
    void endTable() override;
};

}  // namespace tpcc