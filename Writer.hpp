#pragma once

#include "Schema.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace tpcc {

// 写入器基类
class Writer {
  protected:
    virtual void beginTable() = 0;
    virtual void endTable() = 0;

  public:
    Writer(const std::string &output_path, Schema &schema) : output_path_(output_path), schema_(schema) {}
    virtual ~Writer() = default;

    virtual void writeRecord(const Record &record) = 0;

    virtual void writeSchema() {
        std::string schema_dir = output_path_ + "/schema/";
        std::string file_path = schema_dir + schema_.getTableName() + ".sql";
        if (!std::filesystem::exists(schema_dir)) {
            std::filesystem::create_directory(schema_dir);
        }
        std::ofstream out(file_path);
        out << "CREATE TABLE " << schema_.getTableName() << " (" << std::endl;
        for (const auto &column : schema_.getColumns()) {
            out << "  " << column.name << " " << column.getSqlType() << "," << std::endl;
        }
        out << ");" << std::endl;
        if (!out.good()) {
            std::cout << "\nOh no, I can not create file: '" << file_path << "'." << std::endl;
            std::cout << "abooooooorting !!!!!!" << std::endl;
            exit(-1);
        }
    }

  protected:
    std::string output_path_;
    Schema schema_;
};

// 用于支持精度控制
struct Precision {
    Precision(int p) : p(p) {}
    int p;
};

// 用于行结束
struct EndlStruct {};
extern EndlStruct writer_endl;

}  // namespace tpcc