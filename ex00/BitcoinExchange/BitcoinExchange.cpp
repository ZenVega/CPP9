#include "BitcoinExchange.hpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
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

float rate_str_to_float(string rate_str) {
  std::stringstream ss(rate_str);
  float converted;
  ss >> converted;

  if (ss.fail() || !ss.eof()) // detect invalid input like "12.3abc"
    throw BitcoinExchange::InvalidRateException();
  if (converted >= 0 && converted <= 1000)
    return converted;
  else
    throw BitcoinExchange::InvalidRateException();
};

void BitcoinExchange::parse_row(string row) {

  time_t time;
  float rate;

  try {
    time = date_to_time_t(row.substr(0, 10));
    int index_rate = row.find(',');
    if (index_rate == -1)
      throw BitcoinExchange::InvalidDateException();
    rate = rate_str_to_float(row.substr(index_rate + 1));
    _db[time] = rate;
  } catch (std::exception &e) {
    std::cout << "Exception caught: " << e.what() << std::endl;
  }
}

void BitcoinExchange::parse_csv(void) {

  string DB_row;
  std::ifstream DB_file("data.csv");
  bool first_line = true;

  while (getline(DB_file, DB_row)) {
    if (first_line)
      first_line = false;
    else
      BitcoinExchange::parse_row(DB_row);
  }

  for (std::map<time_t, float>::iterator it = _db.begin(); it != _db.end();
       it++)
    std::cout << it->first << " : " << it->second << std::endl;
  DB_file.close();
}

void BitcoinExchange::parse_input(char *fileName) {
  if (fileName)
    return;
  return;
}

const char *BitcoinExchange::InvalidDateException::what() const throw() {
  return "Invalid Date";
}

const char *BitcoinExchange::InvalidRateException::what() const throw() {
  return "Invalid Date";
}
