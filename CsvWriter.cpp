#include "CsvWriter.hpp"
#include <iomanip>
#include <iostream>

namespace tpcc {

CsvWriter::CsvWriter(const std::string& output_path)
    : Writer(output_path), first_word_in_line_(true), current_schema_(nullptr) {
}

void CsvWriter::beginTable(const Schema& schema) {
    current_schema_ = &schema;
    // 组合完整的文件路径
    std::string file_path = output_path_ + "/" + schema.getTableName() + ".csv";
    out_.open(file_path);
    if (!out_.good()) {
        std::cout << "\nOh no, I can not create file: '" << file_path << "'." << std::endl;
        std::cout << "abooooooorting !!!!!!" << std::endl;
        exit(-1);
    }
    first_word_in_line_ = true;
}

void CsvWriter::endTable() {
    if (out_.is_open()) {
        out_.close();
    }
    first_word_in_line_ = true;
    current_schema_ = nullptr;
}

void CsvWriter::prePrint() {
    if (first_word_in_line_) {
        first_word_in_line_ = false;
    } else {
        out_ << ',';
    }
}

void CsvWriter::writeValue(const Value& value, ColumnType expected_type) {
    prePrint();

    switch (expected_type) {
        case ColumnType::INT:
            out_ << std::get<int64_t>(value);
            break;
        case ColumnType::FLOAT:
            out_ << std::fixed << std::setprecision(2) << std::get<float>(value);
            break;
        case ColumnType::CHAR:
            // 对于字符串，写入时不需要引号（CSV中通常字符串不加引号除非包含特殊字符）
            out_ << std::get<std::string>(value);
            break;
    }
}

void CsvWriter::writeRecord(const Record& record) {
    if (!current_schema_) {
        std::cout << "Error: No schema set for writing record" << std::endl;
        return;
    }

    const auto& columns = current_schema_->getColumns();
    if (record.size() != columns.size()) {
        std::cout << "Error: Record size (" << record.size()
                  << ") doesn't match schema column count (" << columns.size() << ")" << std::endl;
        return;
    }

    // 写入每个字段
    for (size_t i = 0; i < record.size(); ++i) {
        writeValue(record[i], columns[i].type);
    }

    // 写入换行
    out_ << "\n";
    first_word_in_line_ = true;
}

} // namespace tpcc
