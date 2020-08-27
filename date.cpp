#include "date.h"

using namespace std;

Year::Year(const int& new_value){
	if (new_value >= 0){
		value = new_value;
	}
	else{
		stringstream ss;
		ss<<"Invalid format of year: "<< new_value<<endl;
		throw invalid_argument(ss.str());
	}
}


Month::Month(const int& new_value){
	if (new_value > 0 && new_value <= 12){
		value = new_value;
	}
	else{
		stringstream ss;
		ss<<"Invalid format of month: "<< new_value<<endl;
		throw invalid_argument(ss.str());
	}
}

Day::Day(const int& new_value){
	if (new_value > 0 && new_value <= 31){
		value = new_value;
	}
	else{
		stringstream ss;
		ss<<"Invalid format of day: "<< new_value<<endl;
		throw invalid_argument(ss.str());
	}
}

Date::Date(const Year& new_year,const Month& new_month, const Day & new_day)
			:year(new_year), month(new_month), day(new_day){}

int Date::GetYear() const{
	return year.value;
}

int Date::GetMonth() const{
	return month.value;
}

int Date::GetDay() const{
	return day.value;
}
string Date::Print() const{
	stringstream ans;
	ans<<setfill('0');
	ans<<setw(4)<<year.value<<"-";
	ans<<setw(2)<<month.value<<"-";
	ans<<setw(2)<<day.value;
	return ans.str();
}
ostream & operator<<(ostream& os, const Date& date){
	os<<date.Print();
	return os;
}

Date ParseDate(istream & is){
	int y,m,d;
	char c1,c2;
	is>>y>>c1>>m>>c2>>d;
	if(c1!= '-' || c2 != '-'){
		throw invalid_argument("wrong date format");
	}
	return {y,m,d};
}


bool operator<(const Date& lhs, const Date& rhs){
	return make_tuple(lhs.GetYear(),lhs.GetMonth(),lhs.GetDay()) <
			make_tuple(rhs.GetYear(),rhs.GetMonth(),rhs.GetDay());
}
bool operator<=(const Date& lhs, const Date& rhs){
	return make_tuple(lhs.GetYear(),lhs.GetMonth(),lhs.GetDay()) <=
			make_tuple(rhs.GetYear(),rhs.GetMonth(),rhs.GetDay());
}
bool operator==(const Date& lhs, const Date& rhs){
	return make_tuple(lhs.GetYear(),lhs.GetMonth(),lhs.GetDay()) ==
			make_tuple(rhs.GetYear(),rhs.GetMonth(),rhs.GetDay());
}
bool operator>=(const Date& lhs, const Date& rhs){
	return make_tuple(lhs.GetYear(),lhs.GetMonth(),lhs.GetDay()) >=
			make_tuple(rhs.GetYear(),rhs.GetMonth(),rhs.GetDay());
}
bool operator>(const Date& lhs, const Date& rhs){
	return make_tuple(lhs.GetYear(),lhs.GetMonth(),lhs.GetDay()) >
			make_tuple(rhs.GetYear(),rhs.GetMonth(),rhs.GetDay());
}
bool operator!=(const Date& lhs, const Date& rhs){
	return make_tuple(lhs.GetYear(),lhs.GetMonth(),lhs.GetDay()) !=
			make_tuple(rhs.GetYear(),rhs.GetMonth(),rhs.GetDay());
}
/*
Date ParseDate(istream & is){
	string str;
	getline(is,str,' ');
	cout<<str<<endl;
    auto first = find_if(str.begin(),str.end(),[](const char& c){return c != ' ';});
    str = {first,str.end()};
	vector<int> box;

	for(auto i = str.begin(); i!= str.end();){
		auto prom = find(i,str.end(),'-');
		box.push_back(stoi(string(i,prom)));
		i = prom;
		if (i!=str.end()){
			i++; }
	}
	if(box.size()!= 3){
		throw invalid_argument("Invalid data format: "+str);
	}

	return {box[0],box[1],box[2]};
}
*/

/*
 * Add 2017-06-01 1st of June
Add 2017-07-08 8th of July
Add 2017-07-08 Someone's birthday
Del date == 2017-07-08
 * */
