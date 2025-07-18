#include "CsvWriter.hpp"
#include <iomanip>
#include <iostream>

namespace tpcc {

void CsvWriter::beginTable() {
    // 组合完整的文件路径
    std::string file_path = output_path_ + "/" + schema_.getTableName() + ".csv";
    out_.open(file_path);
    if (!out_.good()) {
        std::cout << "\nOh no, I can not create file: '" << file_path << "'." << std::endl;
        std::cout << "abooooooorting !!!!!!" << std::endl;
        exit(-1);
    }
    // 打印表头
    // w_id,w_name,w_street_1,w_street_2,w_city,w_state,w_zip,w_tax,w_ytd
    auto columns = schema_.getColumns();
    for (size_t i = 0; i < columns.size(); i++) {
        const auto& column = columns[i];
        out_ << column.name;
        if (i != columns.size() - 1) {
            out_ << ",";
        }
    }
    out_ << "\n";
    first_word_in_line_ = true;
}

void CsvWriter::endTable() {
    if (out_.is_open()) {
        out_.close();
    }
    first_word_in_line_ = true;
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
    const auto& columns = schema_.getColumns();
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
