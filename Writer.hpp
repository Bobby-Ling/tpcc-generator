#pragma once

#include "Schema.hpp"
#include <string>

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