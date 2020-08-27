#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <string>

struct Day{
	int value;
	Day(const int& new_value);
};

struct Month{
	int value;
	Month(const int& new_value);
};

struct Year{
	int value;
	Year(const int& new_value);
};


class Date{
public:
	Date(const Year& new_year,const Month& new_month, const Day & new_day);

	int GetYear() const;

	int GetMonth() const;

	int GetDay() const;

	std::string Print() const;


private:
	Year year;
	Month month;
	Day day;
};

 std::ostream& operator<<(std::ostream& os, const Date& date);

 bool operator<(const Date& lhs, const Date& rhs);
 bool operator<=(const Date& lhs, const Date& rhs);
 bool operator==(const Date& lhs, const Date& rhs);
 bool operator>=(const Date& lhs, const Date& rhs);
 bool operator>(const Date& lhs, const Date& rhs);
 bool operator!=(const Date& lhs, const Date& rhs);
 Date ParseDate(std::istream & is);

 void TestDate();
