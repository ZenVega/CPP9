#include "BitcoinExchange/BitcoinExchange.hpp"

int main(int argc, char **argv) {
  BitcoinExchange bt_ex = BitcoinExchange();
  if (argc == 2)
    bt_ex.parse_csv(argv[1]);
  return 0;
}
