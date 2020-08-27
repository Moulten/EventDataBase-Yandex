#include "database.h"

using namespace std;

void Database::Add(const Date& date, const string& event){
	auto& x = database[date];
	if(x._set_.count(event) == 0){
		x._vec_.push_back(event);
		x._set_.insert(event);
	}

}
void Database::Print(ostream & os) const{
	for (const auto& item: database){
		for (const auto& i: item.second._vec_){
			os<<item.first<<" "<<i<<endl;
		}
	}
}


string Database::Last(const Date& date) const{
	auto x = database.upper_bound(date);
	if(x == database.begin()){
		throw invalid_argument("");
	}
	stringstream ans;
	ans<<(--x)->first<<" "<< (x)->second._vec_.back();

	return ans.str();

}
