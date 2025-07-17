#pragma once

#include "Schema.hpp"
#include "Writer.hpp"
#include <cstdint>
#include <memory>
#include <random>
#include <string>

namespace tpcc {

class TpccGenerator {
  private:
    const static uint32_t kItemCount = 100000;
    const static uint32_t kCustomerPerDistrict = 3000;
    const static uint32_t kDistrictsPerWarehouse = 10;
    const static uint32_t OrdersPerDistrict = 3000;

    // If these are different the order generation needs to be changed.
    // Right now there is a 1:1 relationship between customers and orders.
    static_assert(kCustomerPerDistrict == OrdersPerDistrict, "These should match, see comment.");

    const int64_t warehouse_count_;
    const std::string output_path_;
    const std::string format_;

    std::mt19937 ranny_;

    // Schema创建方法
    Schema createWarehouseSchema();
    Schema createDistrictSchema();
    Schema createCustomerSchema();
    Schema createHistorySchema();
    Schema createItemSchema();
    Schema createStockSchema();
    Schema createOrdersSchema();
    Schema createNewOrdersSchema();
    Schema createOrderLineSchema();

    uint32_t makeAlphaString(uint32_t min, uint32_t max, char *dest);
    uint32_t makeNumberString(uint32_t min, uint32_t max, char *dest);
    uint32_t makeNumber(uint32_t min, uint32_t max);
    uint32_t makeNonUniformRandom(uint32_t A, uint32_t x, uint32_t y);
    std::vector<uint32_t> makePermutation(uint32_t min, uint32_t max);
    void makeAddress(char *str1, char *street2, char *city, char *state, char *zip);
    void makeLastName(int64_t num, char *name);
    void makeNow(char *str);

    // 生成固定长度的字符串
    std::string makeAlphaStringAsString(uint32_t min, uint32_t max);
    std::string makeNumberStringAsString(uint32_t min, uint32_t max);
    std::string makeNowAsString();

    std::unique_ptr<Writer> createWriter(Schema &schema);

  public:
    TpccGenerator(int64_t warehouse_count, std::string output_path, std::string format)
      :warehouse_count_(warehouse_count), output_path_(output_path), format_(format), ranny_(42) {}

    void setRandomSeed(uint32_t seed) { ranny_.seed(seed); }

    // 数据生成
    void generateWarehouses();
    void generateDistricts();
    void generateCustomerAndHistory();
    void generateItems();
    void generateStock();
    void generateOrdersAndOrderLines();
};

}  // namespace tpcc