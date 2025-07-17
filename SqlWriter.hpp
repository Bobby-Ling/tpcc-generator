#pragma once

#include "Writer.hpp"
#include <fstream>

namespace tpcc {

class SqlWriter : public Writer {
  private:
    std::ofstream out_;

    // 辅助方法：转义SQL字符串
    std::string escapeSqlString(const std::string &str);

    // 辅助方法：格式化值为SQL格式
    std::string formatValue(const Value &value, ColumnType expected_type);

  protected:
    void beginTable() override;
    void endTable() override;

  public:
    SqlWriter(const std::string &output_path, Schema &schema) : Writer(output_path, schema) {
      beginTable();
    }
    ~SqlWriter() override {
      endTable();
    }

    void writeRecord(const Record &record) override;
};

}  // namespace tpcc