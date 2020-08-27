#pragma once
#include "date.h"
#include <map>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include "node.h"
#include <algorithm>
#include <sstream>
#include "test_runner.h"

using namespace std;

struct DBItem{
	vector<string> _vec_;
	set<string> _set_;
};

class Database {
public:
	void Add(const Date& date, const std::string& event);
	void Print(std::ostream & os) const;

	template<typename RemoveFunc>
	int RemoveIf(RemoveFunc func);

	template<typename FindFunc>
	std::vector<std::string> FindIf(FindFunc func) const;


	std::string Last(const Date& date) const;

private:
	std::map<Date,DBItem> database;
};

void TestDataBase();

template <class RemoveFunc>
int Database::RemoveIf(RemoveFunc func){
	int ans = 0;
	vector<Date> del;
	for (auto & item : database){
		auto & perem = item.second;
		auto prom = stable_partition(perem._vec_.begin(),perem._vec_.end(),
				[func,item](const string& str){return !func(item.first,str);});

		if((perem._vec_.end() - prom) > 0){
			ans+=perem._vec_.end() - prom;

			set<string> diff;
			set<string> minus(prom, perem._vec_.end());
		    set_difference(perem._set_.begin(), perem._set_.end(), minus.begin(), minus.end(),
		                        inserter(diff, diff.begin()));

		    perem._set_.clear();
		    perem._set_ = diff;

			perem._vec_.erase(prom,perem._vec_.end());

			if(perem._vec_.size() == 0){
				del.push_back(item.first);
			}
		}

	}
	for (const auto& x: del){
		database.erase(x);
	}
	return ans;
}
//WARNING
template <typename FindFunc>
vector<string> Database::FindIf(FindFunc func) const{
	vector<string> ans;
	for (auto item : database){
		auto & perem = item.second._vec_;
		copy_if(perem.begin(),perem.end(),back_inserter(ans),
				[func,item](string& str){
			if(func(item.first,str)){
				str = item.first.Print()+" " + str;
				return true;
			}
			return false;});
	}

	return ans;
}
