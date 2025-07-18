#include "TpccGenerator.hpp"
#include "CsvWriter.hpp"
#include "SqlWriter.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <numeric>

namespace tpcc {

using namespace std;

std::unique_ptr<Writer> TpccGenerator::createWriter(Schema &schema) {
    if (format_ == "csv") {
        return make_unique<CsvWriter>(output_path_, schema);
    } else if (format_ == "sql") {
        return make_unique<SqlWriter>(output_path_, schema);
    }
    throw std::runtime_error("Unsupported format: " + format_);
}

// Schema创建方法
Schema TpccGenerator::createWarehouseSchema() {
    Schema schema("warehouse");
    schema.addColumn("w_id", ColumnType::INT);
    schema.addColumn("w_name", ColumnType::CHAR, 10);
    schema.addColumn("w_street_1", ColumnType::CHAR, 20);
    schema.addColumn("w_street_2", ColumnType::CHAR, 20);
    schema.addColumn("w_city", ColumnType::CHAR, 20);
    schema.addColumn("w_state", ColumnType::CHAR, 2);
    schema.addColumn("w_zip", ColumnType::CHAR, 9);
    schema.addColumn("w_tax", ColumnType::FLOAT);
    schema.addColumn("w_ytd", ColumnType::FLOAT);
    return schema;
}

Schema TpccGenerator::createDistrictSchema() {
    Schema schema("district");
    schema.addColumn("d_id", ColumnType::INT);
    schema.addColumn("d_w_id", ColumnType::INT);
    schema.addColumn("d_name", ColumnType::CHAR, 10);
    schema.addColumn("d_street_1", ColumnType::CHAR, 20);
    schema.addColumn("d_street_2", ColumnType::CHAR, 20);
    schema.addColumn("d_city", ColumnType::CHAR, 20);
    schema.addColumn("d_state", ColumnType::CHAR, 2);
    schema.addColumn("d_zip", ColumnType::CHAR, 9);
    schema.addColumn("d_tax", ColumnType::FLOAT);
    schema.addColumn("d_ytd", ColumnType::FLOAT);
    schema.addColumn("d_next_o_id", ColumnType::INT);
    return schema;
}

Schema TpccGenerator::createCustomerSchema() {
    Schema schema("customer");
    schema.addColumn("c_id", ColumnType::INT);
    schema.addColumn("c_d_id", ColumnType::INT);
    schema.addColumn("c_w_id", ColumnType::INT);
    schema.addColumn("c_first", ColumnType::CHAR, 16);
    schema.addColumn("c_middle", ColumnType::CHAR, 2);
    schema.addColumn("c_last", ColumnType::CHAR, 16);
    schema.addColumn("c_street_1", ColumnType::CHAR, 20);
    schema.addColumn("c_street_2", ColumnType::CHAR, 20);
    schema.addColumn("c_city", ColumnType::CHAR, 20);
    schema.addColumn("c_state", ColumnType::CHAR, 2);
    schema.addColumn("c_zip", ColumnType::CHAR, 9);
    schema.addColumn("c_phone", ColumnType::CHAR, 16);
    schema.addColumn("c_since", ColumnType::CHAR, 15);  // 用字符串表示日期时间
    schema.addColumn("c_credit", ColumnType::CHAR, 2);
    schema.addColumn("c_credit_lim", ColumnType::FLOAT);
    schema.addColumn("c_discount", ColumnType::FLOAT);
    schema.addColumn("c_balance", ColumnType::FLOAT);
    schema.addColumn("c_ytd_payment", ColumnType::FLOAT);
    schema.addColumn("c_payment_cnt", ColumnType::INT);
    schema.addColumn("c_delivery_cnt", ColumnType::INT);
    schema.addColumn("c_data", ColumnType::CHAR, 255);
    return schema;
}

Schema TpccGenerator::createHistorySchema() {
    Schema schema("history");
    schema.addColumn("h_c_id", ColumnType::INT);
    schema.addColumn("h_c_d_id", ColumnType::INT);
    schema.addColumn("h_c_w_id", ColumnType::INT);
    schema.addColumn("h_d_id", ColumnType::INT);
    schema.addColumn("h_w_id", ColumnType::INT);
    schema.addColumn("h_datetime", ColumnType::CHAR, 15);  // 用字符串表示日期时间
    schema.addColumn("h_amount", ColumnType::FLOAT);
    schema.addColumn("h_data", ColumnType::CHAR, 24);
    return schema;
}

Schema TpccGenerator::createItemSchema() {
    Schema schema("item");
    schema.addColumn("i_id", ColumnType::INT);
    schema.addColumn("i_im_id", ColumnType::INT);
    schema.addColumn("i_name", ColumnType::CHAR, 24);
    schema.addColumn("i_price", ColumnType::FLOAT);
    schema.addColumn("i_data", ColumnType::CHAR, 50);
    return schema;
}

Schema TpccGenerator::createStockSchema() {
    Schema schema("stock");
    schema.addColumn("s_i_id", ColumnType::INT);
    schema.addColumn("s_w_id", ColumnType::INT);
    schema.addColumn("s_quantity", ColumnType::INT);
    schema.addColumn("s_dist_01", ColumnType::CHAR, 24);
    schema.addColumn("s_dist_02", ColumnType::CHAR, 24);
    schema.addColumn("s_dist_03", ColumnType::CHAR, 24);
    schema.addColumn("s_dist_04", ColumnType::CHAR, 24);
    schema.addColumn("s_dist_05", ColumnType::CHAR, 24);
    schema.addColumn("s_dist_06", ColumnType::CHAR, 24);
    schema.addColumn("s_dist_07", ColumnType::CHAR, 24);
    schema.addColumn("s_dist_08", ColumnType::CHAR, 24);
    schema.addColumn("s_dist_09", ColumnType::CHAR, 24);
    schema.addColumn("s_dist_10", ColumnType::CHAR, 24);
    schema.addColumn("s_ytd", ColumnType::INT);
    schema.addColumn("s_order_cnt", ColumnType::INT);
    schema.addColumn("s_remote_cnt", ColumnType::INT);
    schema.addColumn("s_data", ColumnType::CHAR, 50);
    return schema;
}

Schema TpccGenerator::createOrdersSchema() {
    Schema schema("orders");
    schema.addColumn("o_id", ColumnType::INT);
    schema.addColumn("o_d_id", ColumnType::INT);
    schema.addColumn("o_w_id", ColumnType::INT);
    schema.addColumn("o_c_id", ColumnType::INT);
    schema.addColumn("o_entry_d", ColumnType::CHAR, 15);  // 用字符串表示日期时间
    schema.addColumn("o_carrier_id", ColumnType::INT);
    schema.addColumn("o_ol_cnt", ColumnType::INT);
    schema.addColumn("o_all_local", ColumnType::INT);
    return schema;
}

Schema TpccGenerator::createNewOrdersSchema() {
    Schema schema("new_orders");
    schema.addColumn("no_o_id", ColumnType::INT);
    schema.addColumn("no_d_id", ColumnType::INT);
    schema.addColumn("no_w_id", ColumnType::INT);
    return schema;
}

Schema TpccGenerator::createOrderLineSchema() {
    Schema schema("order_line");
    schema.addColumn("ol_o_id", ColumnType::INT);
    schema.addColumn("ol_d_id", ColumnType::INT);
    schema.addColumn("ol_w_id", ColumnType::INT);
    schema.addColumn("ol_number", ColumnType::INT);
    schema.addColumn("ol_i_id", ColumnType::INT);
    schema.addColumn("ol_supply_w_id", ColumnType::INT);
    schema.addColumn("ol_delivery_d", ColumnType::CHAR, 15);  // 用字符串表示日期时间
    schema.addColumn("ol_quantity", ColumnType::INT);
    schema.addColumn("ol_amount", ColumnType::FLOAT);
    schema.addColumn("ol_dist_info", ColumnType::CHAR, 24);
    return schema;
}

std::string TpccGenerator::makeAlphaStringAsString(uint32_t min, uint32_t max) {
    const static char *possible_values = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    uint32_t len = makeNumber(min, max);
    std::string result;
    result.reserve(len);

    for (uint32_t i = 0; i < len; i++) {
        result += possible_values[ranny_() % 62];
    }

    return result;
}

std::string TpccGenerator::makeNumberStringAsString(uint32_t min, uint32_t max) {
    const static char *possible_values = "0123456789";

    uint32_t len = makeNumber(min, max);
    std::string result;
    result.reserve(len);

    for (uint32_t i = 0; i < len; i++) {
        result += possible_values[ranny_() % 10];
    }

    return result;
}

std::string TpccGenerator::makeNowAsString() {
    return to_string(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
}

void TpccGenerator::generateWarehouses() {
    cout << "Generating warehouse .. " << flush;

    Schema schema = createWarehouseSchema();

    auto warehouseWriter = createWriter(schema);
    warehouseWriter->writeSchema();

    for (int64_t w_id = 1; w_id <= warehouse_count_; w_id++) {
        Record record;

        // 生成warehouse数据
        record.push_back(static_cast<int64_t>(w_id));
        record.push_back(makeAlphaStringAsString(6, 10));

        // 生成地址信息
        record.push_back(makeAlphaStringAsString(10, 20));  // w_street_1
        record.push_back(makeAlphaStringAsString(10, 20));  // w_street_2
        record.push_back(makeAlphaStringAsString(10, 20));  // w_city
        record.push_back(makeAlphaStringAsString(2, 2));    // w_state
        record.push_back(makeNumberStringAsString(9, 9));   // w_zip

        record.push_back(static_cast<float>(makeNumber(10L, 20L)) / 100.0f);  // w_tax
        record.push_back(3000000.00f);                                        // w_ytd

        warehouseWriter->writeRecord(record);
    }

    cout << "ok !" << endl;
}

void TpccGenerator::generateDistricts() {
    cout << "Generating districts .. " << flush;

    Schema schema = createDistrictSchema();
    auto districtWriter = createWriter(schema);
    districtWriter->writeSchema();

    // Each warehouse has DIST_PER_WARE (10) districts
    for (int64_t d_w_id = 1; d_w_id <= warehouse_count_; d_w_id++) {
        for (int64_t d_id = 1; d_id <= kDistrictsPerWarehouse; d_id++) {
            Record record;

            // 填充district数据
            record.push_back(static_cast<int64_t>(d_id));
            record.push_back(static_cast<int64_t>(d_w_id));
            record.push_back(makeAlphaStringAsString(6, 10));  // d_name

            // 生成地址信息
            record.push_back(makeAlphaStringAsString(10, 20));  // d_street_1
            record.push_back(makeAlphaStringAsString(10, 20));  // d_street_2
            record.push_back(makeAlphaStringAsString(10, 20));  // d_city
            record.push_back(makeAlphaStringAsString(2, 2));    // d_state
            record.push_back(makeNumberStringAsString(9, 9));   // d_zip

            record.push_back(static_cast<float>(makeNumber(10L, 20L)) / 100.0f);  // d_tax
            record.push_back(30000.0f);                                           // d_ytd
            record.push_back(static_cast<int64_t>(3001));                         // d_next_o_id

            districtWriter->writeRecord(record);
        }
    }

    cout << "ok !" << endl;
}

void TpccGenerator::generateCustomerAndHistory() {
    cout << "Generating customers and their history .. " << flush;

    // 生成customer数据
    Schema customerSchema = createCustomerSchema();
    auto customerWriter = createWriter(customerSchema);
    customerWriter->writeSchema();

    // 生成history数据
    Schema historySchema = createHistorySchema();
    auto historyWriter = createWriter(historySchema);
    historyWriter->writeSchema();

    // Each warehouse has DIST_PER_WARE (10) districts
    for (int64_t c_w_id = 1; c_w_id <= warehouse_count_; c_w_id++) {
        for (int64_t c_d_id = 1; c_d_id <= kDistrictsPerWarehouse; c_d_id++) {
            for (int64_t c_id = 1; c_id <= kCustomerPerDistrict; c_id++) {
                std::string c_first = makeAlphaStringAsString(8, 16);
                std::string c_middle = string("OE");
                std::string c_last;
                if (c_id <= 1000) {
                    char nameBuffer[32];
                    makeLastName(c_id - 1, nameBuffer);
                    c_last = string(nameBuffer);
                } else {
                    char nameBuffer[32];
                    makeLastName(makeNonUniformRandom(255, 0, 999), nameBuffer);
                    c_last = string(nameBuffer);
                }
                std::string c_street_1, c_street_2, c_city, c_state, c_zip;
                makeAddress(c_street_1, c_street_2, c_city, c_state, c_zip);
                std::string c_phone = makeNumberStringAsString(16, 16);
                std::string c_credit = makeNumber(0L, 1L) == 0 ? "GC" : "BC";
                float c_credit_lim = 50000;
                float c_discount = ((float) makeNumber(0L, 50L)) / 100.0f;
                float c_balance = -10.0f;
                std::string c_since = makeNowAsString();
                std::string c_data = makeAlphaStringAsString(200, 255);

                // // @formatter:off
                // c_csv << c_id << c_d_id << c_w_id << c_first << c_middle << c_last << c_street_1 << c_street_2 << c_city
                //     << c_state << c_zip << c_phone << c_since << c_credit << csv::Precision(2) << c_credit_lim
                //     << csv::Precision(4) << c_discount << csv::Precision(2) << c_balance << 10.0f << int64_t(1)
                //     << int64_t(0) << c_data << csv::endl;
                // // @formatter:on
                Record record;
                // 填充customer数据
                record.push_back(static_cast<int64_t>(c_id));
                record.push_back(static_cast<int64_t>(c_d_id));
                record.push_back(static_cast<int64_t>(c_w_id));
                record.push_back(c_first);  // c_first
                record.push_back(c_middle);                    // c_middle
                record.push_back(c_last);  // c_last
                record.push_back(c_street_1);  // c_street_1
                record.push_back(c_street_2);  // c_street_2
                record.push_back(c_city);  // c_city
                record.push_back(c_state);  // c_state
                record.push_back(c_zip);  // c_zip
                record.push_back(c_phone);  // c_phone
                record.push_back(c_since);  // c_since
                record.push_back(c_credit);  // c_credit
                record.push_back(static_cast<float>(c_credit_lim));  // c_credit_lim
                record.push_back(static_cast<float>(c_discount));  // c_discount
                record.push_back(static_cast<float>(c_balance));  // c_balance
                record.push_back(static_cast<float>(10.0f));  // 10.0f
                record.push_back(static_cast<int64_t>(1));  // 1
                record.push_back(static_cast<int64_t>(0));  // 0
                record.push_back(c_data);  // c_data

                customerWriter->writeRecord(record);

                // 填充history数据
                // h_amount = 10.0;
                // makeAlphaString(12, 24, h_data.data());

                // // @formatter:off
                // h_csv << c_id << c_d_id << c_w_id << c_d_id << c_w_id << c_since << h_amount << h_data << csv::endl;
                // // @formatter:on

                std::string h_data = makeAlphaStringAsString(12, 24);
                float h_amount = 10.0f;

                Record h_record;
                h_record.push_back(static_cast<int64_t>(c_id));
                h_record.push_back(static_cast<int64_t>(c_d_id));
                h_record.push_back(static_cast<int64_t>(c_w_id));
                h_record.push_back(static_cast<int64_t>(c_d_id));
                h_record.push_back(static_cast<int64_t>(c_w_id));
                h_record.push_back(c_since);
                h_record.push_back(static_cast<float>(h_amount));
                h_record.push_back(h_data);

                historyWriter->writeRecord(h_record);
            }
        }
    }

    cout << "ok !" << endl;
}

void TpccGenerator::generateItems() {
    cout << "Generating items .. " << flush;

    vector<bool> orig(kItemCount, false);

    Schema schema = createItemSchema();
    auto itemWriter = createWriter(schema);
    itemWriter->writeSchema();

    for (uint32_t i = 0; i < kItemCount / 10; i++) {
        uint32_t pos;
        do {
            pos = makeNumber(0L, kItemCount - 1);
        } while (orig[pos]);
        orig[pos] = true;
    }

    for (int64_t i_id = 1; i_id <= kItemCount; i_id++) {
        Record record;

        // 填充item数据
        record.push_back(static_cast<int64_t>(i_id));
        record.push_back(static_cast<int64_t>(makeNumber(0, 10000)));             // i_im_id
        record.push_back(makeAlphaStringAsString(14, 24));                        // i_name
        record.push_back(static_cast<float>(makeNumber(100L, 10000L)) / 100.0f);  // i_price

        string i_data = makeAlphaStringAsString(26, 50);
        if (orig[i_id - 1]) {  // 数组索引从0开始
            if (i_data.length() >= 8) {
                uint32_t pos = makeNumber(0L, i_data.length() - 8);
                i_data.replace(pos, 8, "original");
            }
        }
        record.push_back(i_data);  // i_data

        itemWriter->writeRecord(record);
    }

    cout << "ok !" << endl;
}

void TpccGenerator::generateStock() {
    cout << "Generating stocks .. " << flush;

    vector<bool> orig(kItemCount, false);

    Schema schema = createStockSchema();
    auto stockWriter = createWriter(schema);
    stockWriter->writeSchema();

    for (int64_t s_w_id = 1; s_w_id <= warehouse_count_; s_w_id++) {
        orig.assign(kItemCount, false);

        for (uint32_t i = 0; i < kItemCount / 10; i++) {
            int64_t pos;
            do {
                pos = makeNumber(0L, kItemCount - 1);
            } while (orig[pos]);
            orig[pos] = 1;
        }

        for (int64_t s_i_id = 1; s_i_id <= kItemCount; s_i_id++) {
            Record record;

            // 填充stock数据
            record.push_back(static_cast<int64_t>(s_i_id));
            record.push_back(static_cast<int64_t>(s_w_id));
            record.push_back(static_cast<int64_t>(makeNumber(10L, 100L)));  // s_quantity

            // 生成s_dist_01到s_dist_10
            for (int i = 0; i < 10; i++) {
                record.push_back(makeAlphaStringAsString(24, 24));
            }

            record.push_back(static_cast<int64_t>(0));  // s_ytd
            record.push_back(static_cast<int64_t>(0));  // s_order_cnt
            record.push_back(static_cast<int64_t>(0));  // s_remote_cnt

            string s_data = makeAlphaStringAsString(26, 50);
            if (orig[s_i_id - 1]) {  // 数组索引从0开始
                if (s_data.length() >= 8) {
                    int64_t pos = makeNumber(0L, s_data.length() - 8);
                    s_data.replace(pos, 8, "original");
                }
            }
            record.push_back(s_data);  // s_data

            stockWriter->writeRecord(record);
        }
    }

    cout << "ok !" << endl;
}

void TpccGenerator::generateOrdersAndOrderLines() {
    cout << "Generating orders .. " << flush;

    Schema ordersSchema = createOrdersSchema();
    Schema orderLineSchema = createOrderLineSchema();
    Schema newOrdersSchema = createNewOrdersSchema();

    auto ordersWriter = createWriter(ordersSchema);
    ordersWriter->writeSchema();
    auto orderLineWriter = createWriter(orderLineSchema);
    orderLineWriter->writeSchema();
    auto newOrdersWriter = createWriter(newOrdersSchema);
    newOrdersWriter->writeSchema();

    // Generate ORD_PER_DIST (3000) orders and order line items for each district
    for (int64_t o_w_id = 1L; o_w_id <= warehouse_count_; o_w_id++) {
        for (int64_t o_d_id = 1L; o_d_id <= kDistrictsPerWarehouse; o_d_id++) {
            // Each customer has exactly one order
            vector<uint32_t> customer_id_permutation = makePermutation(1, kCustomerPerDistrict + 1);

            for (int64_t o_id = 1; o_id <= OrdersPerDistrict; o_id++) {
                int64_t o_ol_cnt = makeNumber(5L, 15L);

                Record record;
                // 填充order数据
                record.push_back(static_cast<int64_t>(o_id));
                record.push_back(static_cast<int64_t>(o_d_id));
                record.push_back(static_cast<int64_t>(o_w_id));
                record.push_back(static_cast<int64_t>(customer_id_permutation[o_id - 1]));
                record.push_back(makeNowAsString());                                              // o_entry_d
                record.push_back(static_cast<int64_t>((o_id > 2100) ? 0 : makeNumber(1L, 10L)));  // o_carrier_id，0表示null
                record.push_back(static_cast<int64_t>(o_ol_cnt));                      // o_ol_cnt
                record.push_back(static_cast<int64_t>(1));                                        // o_all_local

                ordersWriter->writeRecord(record);

                // Order line items
                for (int64_t ol_number = 1; ol_number <= o_ol_cnt; ol_number++) {
                    int64_t ol_i_id = makeNumber(1L, kItemCount);
                    int64_t ol_quantity = 5;
                    string ol_dist_info = makeAlphaStringAsString(24, 24);
                    float ol_amount;

                    Record ol_record;
                    if (o_id > 2100) {
                        ol_amount = (float)(makeNumber(10L, 10000L)) / 100.0f;
                        // // @formatter:off
                        // ol_csv << o_id << o_d_id << o_w_id << ol_number << ol_i_id << o_w_id << kNull
                        //        << ol_quantity << csv::Precision(2) << ol_amount << ol_dist_info << csv::endl;
                        // // @formatter:on
                        ol_record.push_back(static_cast<int64_t>(o_id));
                        ol_record.push_back(static_cast<int64_t>(o_d_id));
                        ol_record.push_back(static_cast<int64_t>(o_w_id));
                        ol_record.push_back(static_cast<int64_t>(ol_number));
                        ol_record.push_back(static_cast<int64_t>(ol_i_id));
                        ol_record.push_back(static_cast<int64_t>(o_w_id));
                        ol_record.push_back(string(""));
                        ol_record.push_back(static_cast<int64_t>(ol_quantity));
                        ol_record.push_back(static_cast<float>(ol_amount));
                        ol_record.push_back(ol_dist_info);
                    } else {
                        ol_amount = 0.0f;
                        // // @formatter:off
                        // ol_csv << o_id << o_d_id << o_w_id << ol_number << ol_i_id << o_w_id
                        //        << o_entry_d.data() << ol_quantity << csv::Precision(2) << ol_amount << ol_dist_info << csv::endl;
                        // // @formatter:on
                        ol_record.push_back(static_cast<int64_t>(o_id));
                        ol_record.push_back(static_cast<int64_t>(o_d_id));
                        ol_record.push_back(static_cast<int64_t>(o_w_id));
                        ol_record.push_back(static_cast<int64_t>(ol_number));
                        ol_record.push_back(static_cast<int64_t>(ol_i_id));
                        ol_record.push_back(static_cast<int64_t>(o_w_id));
                        ol_record.push_back(makeNowAsString());
                        ol_record.push_back(static_cast<int64_t>(ol_quantity));
                        ol_record.push_back(static_cast<float>(ol_amount));
                        ol_record.push_back(ol_dist_info);
                    }
                    orderLineWriter->writeRecord(ol_record);
                }

                // Generate a new order entry for the order for the last 900 rows
                if (o_id > 2100) {
                    // no_csv << o_id << o_d_id << o_w_id << csv::endl;
                    Record record;
                    record.push_back(static_cast<int64_t>(o_id));
                    record.push_back(static_cast<int64_t>(o_d_id));
                    record.push_back(static_cast<int64_t>(o_w_id));
                    newOrdersWriter->writeRecord(record);
                }

            }
        }
    }

    cout << "ok !" << endl;
}

// 以下是数据生成的辅助方法，保持原有逻辑不变
uint32_t TpccGenerator::makeAlphaString(uint32_t min, uint32_t max, char *dest) {
    const static char *possible_values = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    uint32_t len = makeNumber(min, max);
    for (uint32_t i = 0; i < len; i++) {
        dest[i] = possible_values[ranny_() % 62];
    }
    if (len < max) {
        dest[len] = '\0';
    }
    return len;
}

uint32_t TpccGenerator::makeNumberString(uint32_t min, uint32_t max, char *dest) {
    const static char *possible_values = "0123456789";

    uint32_t len = makeNumber(min, max);
    for (uint32_t i = 0; i < len; i++) {
        dest[i] = possible_values[ranny_() % 10];
    }
    if (len < max) {
        dest[len] = '\0';
    }
    return len;
}

void TpccGenerator::makeAddress(char *street1, char *street2, char *city, char *state, char *zip) {
    makeAlphaString(10, 20, street1);
    makeAlphaString(10, 20, street2);
    makeAlphaString(10, 20, city);
    makeAlphaString(2, 2, state);
    makeNumberString(9, 9, zip);  // XXX
}

void TpccGenerator::makeAddress(std::string &street1, std::string &street2, std::string &city, std::string &state, std::string &zip) {
    street1 = makeAlphaStringAsString(10, 20);
    street2 = makeAlphaStringAsString(10, 20);
    city = makeAlphaStringAsString(10, 20);
    state = makeAlphaStringAsString(2, 2);
    zip = makeNumberStringAsString(9, 9);
}

uint32_t TpccGenerator::makeNumber(uint32_t min, uint32_t max) {
    return ranny_() % (max - min + 1) + min;
}

uint32_t TpccGenerator::makeNonUniformRandom(uint32_t A, uint32_t x, uint32_t y) {
    return ((makeNumber(0, A) | makeNumber(x, y)) + 42) % (y - x + 1) + x;  // XXX
}

vector<uint32_t> TpccGenerator::makePermutation(uint32_t min, uint32_t max) {
    assert(max > min);
    const uint32_t count = max - min;
    vector<uint32_t> result(count);
    iota(result.begin(), result.end(), min);

    for (uint32_t i = 0; i < count; i++) {
        swap(result[i], result[ranny_() % count]);
    }
    return result;
}

void TpccGenerator::makeLastName(int64_t num, char *name) {
    static const char *n[] = {"BAR", "OUGHT", "ABLE", "PRI", "PRES", "ESE", "ANTI", "CALLY", "ATION", "EING"};
    strcpy(name, n[num / 100]);
    strcat(name, n[(num / 10) % 10]);
    strcat(name, n[num % 10]);
}

void TpccGenerator::makeNow(char *str) {
    string s = to_string(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());  // XXX
    strncpy(str, s.data(), s.size());
}

}  // namespace tpcc
