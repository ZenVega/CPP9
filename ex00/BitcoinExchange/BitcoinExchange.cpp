#include "BitcoinExchange.hpp"
#include <ctime>
#include <fstream>
#include <string>

using std::string;

BitcoinExchange::BitcoinExchange() {};

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {
  *this = other;
};

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
  if (this != &other) {
    _db = other._db;
  }
  return (*this);
};

BitcoinExchange::~BitcoinExchange() {};

time_t date_to_time_t(string date) {
  struct tm tm;
  if (strptime(date.c_str(), "%Y-%m-%d", &tm)) {
    return mktime(&tm);
  } else
    throw std::exception();
}

void BitcoinExchange::parse_row(string row) {

  time_t time;
  try {
    time = date_to_time_t(row.substr(0, 7));
  } catch (std::exception) {
    time = NULL;
  }
}

void BitcoinExchange::parse_csv(char *fileName) {

  string DB_row;
  std::ifstream DB_file(fileName);
  bool end_of_file = false;

  while (getline(DB_file, DB_row)) {
    BitcoinExchange::parse_row(DB_row);
  }

  DB_file.close();
}

const char *BitcoinExchange::InvalidDateException::what() const throw() {
  return "Invalid Date";
}

const char *BitcoinExchange::InvalidRateException::what() const throw() {
  return "Invalid Date";
}
