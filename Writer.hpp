#pragma once

#include "Schema.hpp"
#include <string>

namespace tpcc {

// 写入器基类
class Writer {
  public:
    Writer(const std::string &output_path) : output_path_(output_path) {}
    virtual ~Writer() = default;

    // 开始新表的写入
    virtual void beginTable(const Schema &schema) = 0;

    // 写入一条记录
    virtual void writeRecord(const Record &record) = 0;

    // 结束表写入
    virtual void endTable() = 0;

  protected:
    std::string output_path_;
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