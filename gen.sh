#!/bin/bash

WAREHOUSE_NUM=$1
if [ -z "$WAREHOUSE_NUM" ]; then
    WAREHOUSE_NUM=2
fi

# ./build/tpcc-generator 2 tpcc_sql/ sql

# head -1000 tpcc_sql/*.sql | grep -v "==>" > demo.sql

#  wc -l tpcc_sql/*.sql
#     60000 tpcc_sql/customer.sql
#        20 tpcc_sql/district.sql
#     60000 tpcc_sql/history.sql
#    100000 tpcc_sql/item.sql
#     18000 tpcc_sql/new_orders.sql
#    601255 tpcc_sql/order_line.sql
#     60000 tpcc_sql/orders.sql
#    200000 tpcc_sql/stock.sql
#         2 tpcc_sql/warehouse.sql
#   1099277 总计

# 2个warehouse
./build/tpcc-generator $WAREHOUSE_NUM tpcc_csv/ csv
wc -l tpcc_csv/*.csv

./build/tpcc-generator $WAREHOUSE_NUM tpcc_sql/ sql
wc -l tpcc_sql/*.sql

CSV_DIR=tpcc_csv
NOHEADER_CSV_DIR=tpcc_csv_no_header
mkdir -p $NOHEADER_CSV_DIR
for file in $(ls $CSV_DIR | grep ".csv"); do
    tail -n+2 $CSV_DIR/$file > $NOHEADER_CSV_DIR/$file
done;
wc -l tpcc_csv_no_header/*.csv

# head -2 tpcc_csv_no_header/*.csv

# ./gen.sh 2
# I am loading TPCC data for 2 warehouses, hold on ..

# Generating warehouse .. ok !
# Generating districts .. ok !
# Generating customers and their history .. ok !
# Generating items .. ok !
# Generating stocks .. ok !
# Generating orders .. ok !

# .. data generation completed successfully :)
#     60001 tpcc_csv/customer.csv
#        21 tpcc_csv/district.csv
#     60001 tpcc_csv/history.csv
#    100001 tpcc_csv/item.csv
#     18001 tpcc_csv/new_orders.csv
#    600321 tpcc_csv/order_line.csv
#     60001 tpcc_csv/orders.csv
#    200001 tpcc_csv/stock.csv
#         3 tpcc_csv/warehouse.csv
#   1098351 总计
# I am loading TPCC data for 2 warehouses, hold on ..

# Generating warehouse .. ok !
# Generating districts .. ok !
# Generating customers and their history .. ok !
# Generating items .. ok !
# Generating stocks .. ok !
# Generating orders .. ok !

# .. data generation completed successfully :)
#     60000 tpcc_sql/customer.sql
#        20 tpcc_sql/district.sql
#     60000 tpcc_sql/history.sql
#    100000 tpcc_sql/item.sql
#     18000 tpcc_sql/new_orders.sql
#    600320 tpcc_sql/order_line.sql
#     60000 tpcc_sql/orders.sql
#    200000 tpcc_sql/stock.sql
#         2 tpcc_sql/warehouse.sql
#   1098342 总计
#     60000 tpcc_csv_no_header/customer.csv
#        20 tpcc_csv_no_header/district.csv
#     60000 tpcc_csv_no_header/history.csv
#    100000 tpcc_csv_no_header/item.csv
#     18000 tpcc_csv_no_header/new_orders.csv
#    600320 tpcc_csv_no_header/order_line.csv
#     60000 tpcc_csv_no_header/orders.csv
#    200000 tpcc_csv_no_header/stock.csv
#         2 tpcc_csv_no_header/warehouse.csv
#   1098342 总计