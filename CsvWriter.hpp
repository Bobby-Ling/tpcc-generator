#pragma once

#include "Writer.hpp"
#include <fstream>

namespace tpcc {

class CsvWriter : public Writer {
private:
    std::ofstream out_;
    bool first_word_in_line_;

    // 辅助方法：打印前的预处理（添加逗号）
    void prePrint();

    // 辅助方法：写入一个值
    void writeValue(const Value& value, ColumnType expected_type);

  protected:
    void beginTable() override;
    void endTable() override;

public:
    CsvWriter(const std::string& output_path, Schema &schema) : Writer(output_path, schema) {
      beginTable();
    }
    ~CsvWriter() override {
      endTable();
    }

    void writeRecord(const Record& record) override;
};

} // namespace tpcc
