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

  if (ss.fail() || !ss.eof())
    throw BitcoinExchange::InvalidRateException();
  return converted;
};

void BitcoinExchange::parse_db_row(string row) {

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

void BitcoinExchange::parse_data(void) {

  string DB_row;
  std::ifstream DB_file("data.csv");
  bool first_line = true;

  while (getline(DB_file, DB_row)) {
    if (first_line)
      first_line = false;
    else
      BitcoinExchange::parse_db_row(DB_row);
  }
  DB_file.close();
}

void BitcoinExchange::parse_input_val(string row,
                                      std::map<time_t, float>::iterator it) {
  int index_val = row.find('|');
  if (index_val == -1)
    throw BitcoinExchange::InvalidRateException();
  val = val_str_to_float(row.substr(index_val + 1));
}

void BitcoinExchange::parse_input_row(string row) {

  time_t time;
  float rate;
  std::map<time_t, float>::iterator it;

  try {
    time = date_to_time_t(row.substr(0, 10));
    it = _db.lower_bound(time);
    if (it == _db.end())
      throw BitcoinExchange::DateLateException();
    if (it->first == time) {
      std::cout << row.substr(0, 10);
      parse_input_rate(row, it);
    } else if (it == _db.begin())
      throw BitcoinExchange::DateEarlyException();
    else {
      std::cout << row.substr(0, 10);
      parse_input_val(row, --it);
    }
  } catch (std::exception &e) {
    std::cout << "Exception caught: " << e.what() << std::endl;
  }
}

void BitcoinExchange::parse_input(const char *fileName) {

  string input_row;
  std::ifstream input_file(fileName);
  bool first_line = true;

  while (getline(input_file, input_row)) {
    if (first_line)
      first_line = false;
    else
      BitcoinExchange::parse_input_row(input_row);
  }
  input_file.close();
}
const char *BitcoinExchange::InvalidDateException::what() const throw() {
  return "Invalid Date";
}

const char *BitcoinExchange::DateLateException::what() const throw() {
  return "Date is too late";
}

const char *BitcoinExchange::DateEarlyException::what() const throw() {
  return "Date is too early";
}

const char *BitcoinExchange::InvalidRateException::what() const throw() {
  return "Invalid Rate";
}
