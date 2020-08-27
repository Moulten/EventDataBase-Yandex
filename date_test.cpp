#include "date.h"
#include "test_runner.h"
#include <sstream>
#include <iostream>

using namespace std;

void TestDate(){
	//Тестируем идеальную строку
	{
		istringstream is("2020-04-05");
		auto date = ParseDate(is);
		AssertEqual(date.Print(),is.str(),"ideal string testing");
		AssertEqual(date.GetYear(),2020,"ideal string testing (year)");
		AssertEqual(date.GetMonth(),4,"ideal string testing (month)");
		AssertEqual(date.GetDay(),5,"ideal string testing (day)");
	}
	//Тестируем строку с пробелом
	{
		istringstream is(" 2020-04-05");
		auto date = ParseDate(is);
		AssertEqual(date.Print(),"2020-04-05","string with space testing");
		AssertEqual(date.GetYear(),2020,"string with space testing (year)");
		AssertEqual(date.GetMonth(),4,"string with space testing (month)");
		AssertEqual(date.GetDay(),5,"string with space testing (day)");
	}
	//Тестируем неправильный формат для года
/*	{
		string str_error;
		try{
			istringstream is("-20-04-05");
			auto date = ParseDate(is);
			cout<<date.GetYear()<<endl;
		} catch (exception& er){
			cout<< er.what();
			cout<<str_error;
		}

		AssertEqual(str_error,"Invalid format of year: -20","Wrong format for year testing");
	}
	//Тестируем неправильный формат для месяца
	{
		string str_error;
		try{
			istringstream is("2020-30-05");
			auto date = ParseDate(is);
		} catch (invalid_argument& er){
			str_error = er.what();
		}
		AssertEqual(str_error,"Invalid format of month: 30","Wrong format for month testing");
	}
	//Тестируем неправильный формат для дня
	{
		string error;
		try{
			istringstream is("2020-03-45");
			auto date = ParseDate(is);
		} catch (invalid_argument& er){
			error = er.what();
		}
		AssertEqual(error,"Invalid format of day: 45","Wrong format for day testing");
	}*/
}
