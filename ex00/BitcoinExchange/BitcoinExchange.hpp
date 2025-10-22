#ifndef BITCOIN_EXCHANGE_HPP
#define BITCOIN_EXCHANGE_HPP

#include <ctime>
#include <exception>
#include <map>
#include <string>

class BitcoinExchange {
private:
  std::map<time_t, float> _db;
  void parse_row(std::string row);

public:
  BitcoinExchange();
  BitcoinExchange(const BitcoinExchange &other);
  BitcoinExchange &operator=(const BitcoinExchange &other);
  ~BitcoinExchange();

  void parse_csv(char *fileName);

  class InvalidDateException : public std::exception {
    virtual const char *what() const throw();
  };
  class InvalidRateException : public std::exception {
    virtual const char *what() const throw();
  };
};

#endif // BITCOIN_EXCHANGE_HPP
