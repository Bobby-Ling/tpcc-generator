#pragma once

#include "Writer.hpp"
#include <fstream>

namespace tpcc {

class CsvWriter : public Writer {
private:
    std::ofstream out_;
    bool first_word_in_line_;
    const Schema* current_schema_;  // 当前表的Schema

    // 辅助方法：打印前的预处理（添加逗号）
    void prePrint();

    // 辅助方法：写入一个值
    void writeValue(const Value& value, ColumnType expected_type);

public:
    CsvWriter(const std::string& output_path);

    // 实现Writer接口
    void beginTable(const Schema& schema) override;
    void writeRecord(const Record& record) override;
    void endTable() override;
};

} // namespace tpcc
