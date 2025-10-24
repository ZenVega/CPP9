#include "BitcoinExchange/BitcoinExchange.hpp"

int main(int argc, char **argv) {
  BitcoinExchange bt_ex = BitcoinExchange();
  std::string data_file = "data.csv";
  if (argc == 2 && argv[1]) {
    bt_ex.parse_data();
    bt_ex.parse_input(argv[1]);
  }
  return 0;
}
