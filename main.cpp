#include "TpccGenerator.hpp"
#include "CsvWriter.hpp"
#include "SqlWriter.hpp"

#include <iostream>
#include <memory>
#include <filesystem>

using namespace std;
using namespace tpcc;

int main(int argc, char **argv) {
    // Check input
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <warehouse_count> <output_path> <format(csv|sql)>" << endl;
        return -1;
    }

    // Parse input
    char *end_ptr;
    long warehouse_count = strtol(argv[1], &end_ptr, 10);
    if (*end_ptr != '\0' || warehouse_count > (1ll << 32) - 1 || warehouse_count < 0) {
        cout << "ERROR: I can not parse '" << argv[1] << "' as a 32 bit unsigned integer :(" << endl;
        cout << "Usage: " << argv[0] << " <warehouse_count> <output_path> <format(csv|sql)>" << endl;
        return -1;
    }

    string output_path = argv[2];
    string format = argv[3];

    if (!filesystem::exists(output_path)) {
        filesystem::create_directories(output_path);
    }


    // 创建对应的Writer
    unique_ptr<Writer> writer;
    if (format == "csv") {
        writer = make_unique<CsvWriter>(output_path);
        cout << "Using CSV format" << endl;
    } else if (format == "sql") {
        writer = make_unique<SqlWriter>(output_path);
        cout << "Using SQL format" << endl;
    } else {
        cout << "ERROR: Unsupported format '" << format << "'. Supported formats: csv, sql" << endl;
        cout << "Usage: " << argv[0] << " <warehouse_count> <output_path> <format(csv|sql)>" << endl;
        return -1;
    }

    // Generate tpcc data
    TpccGenerator generator((uint32_t) warehouse_count, std::move(writer));

    cout << "I am loading TPCC data for " << warehouse_count << " warehouse"
         << (warehouse_count != 1 ? "s" : "") << ", hold on .." << endl << endl;

    generator.generateWarehouses();
    generator.generateDistricts();
    generator.generateCustomerAndHistory();
    generator.generateItems();
    generator.generateStock();
    generator.generateOrdersAndOrderLines();

    cout << endl << ".. data generation completed successfully :)" << endl;

    return 0;
}
