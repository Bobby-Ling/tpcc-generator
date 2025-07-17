#include "SqlWriter.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

namespace tpcc {

SqlWriter::SqlWriter(const std::string& output_path)
    : Writer(output_path), current_schema_(nullptr) {
}

void SqlWriter::beginTable(const Schema& schema) {
    current_schema_ = &schema;
    // 组合完整的文件路径
    std::string file_path = output_path_ + "/" + schema.getTableName() + ".sql";
    out_.open(file_path);
    if (!out_.good()) {
        std::cout << "\nOh no, I can not create file: '" << file_path << "'." << std::endl;
        std::cout << "abooooooorting !!!!!!" << std::endl;
        exit(-1);
    }
}

void SqlWriter::endTable() {
    if (out_.is_open()) {
        out_.close();
    }
    current_schema_ = nullptr;
}

std::string SqlWriter::escapeSqlString(const std::string& str) {
    std::string result = "'";
    for (char c : str) {
        if (c == '\'') {
            result += "''";  // 转义单引号
        } else if (c == '\0') {
            break;  // 遇到字符串结束符停止
        } else {
            result += c;
        }
    }
    result += "'";
    return result;
}

std::string SqlWriter::formatValue(const Value& value, ColumnType expected_type) {
    std::ostringstream oss;

    switch (expected_type) {
        case ColumnType::INT:
            oss << std::get<int64_t>(value);
            break;
        case ColumnType::FLOAT:
            oss << std::fixed << std::setprecision(2) << std::get<float>(value);
            break;
        case ColumnType::CHAR:
            return escapeSqlString(std::get<std::string>(value));
    }

    return oss.str();
}

void SqlWriter::writeRecord(const Record& record) {
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

    // 生成INSERT语句
    out_ << "INSERT INTO " << current_schema_->getTableName() << " VALUES (";

    for (size_t i = 0; i < record.size(); ++i) {
        if (i > 0) out_ << ", ";
        out_ << formatValue(record[i], columns[i].type);
    }

    out_ << ");\n";
}

} // namespace tpcc