#include "database.h"
#include "test_runner.h"
#include "condition_parser.h"
#include "node.h"


void TestDataBase_2();

void TestDataBase(){
	Database base;
	string cerr_str;
	//Проверяем метод Last на пустой базе
    try {
    	base.Last(Date(2017,3,3));
    } catch (invalid_argument&) {
        cerr_str = "No entries";
    }
	AssertEqual(cerr_str,"No entries", "Command Last. Empty base");

	//Добавляем значения в базу
	base.Add({2017,3,3},"first");
	base.Add({2017,3,4},"second");

	vector<string> events = {
		"Monday", "Tuesday",
		"Wednesday", "Thursday",
		"Friday", "Saturday",
		"Sunday"
	};
	int x = 3;
	for (const auto& ev: events){
		base.Add({2017,3,x},ev);
		x++;
	}
	cerr_str = "";
    try {
    	base.Last(Date(2017,3,2));
    } catch (invalid_argument&) {
        cerr_str = "No entries";
    }
    //Проверяем метод Last с датой меньшей чем минимальная в словаре
	AssertEqual(cerr_str,"No entries", "Command Last. The first date in the base");
	// Проверяем корректность ответа при гарантированном наличии события
	AssertEqual(base.Last({2017,3,4}),"2017-03-04 Tuesday", "Command Last. Normal work of the base");

	//Тестируем удаление по условию
	//по дате
	istringstream is("date > 2017-03-08");

    auto condition = ParseCondition(is);
    auto predicate = [&condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
	AssertEqual(base.RemoveIf(predicate),1, "Remove function for date testing");

	// По событию
	is = istringstream ("event == \"Saturday\"");
    condition = ParseCondition(is);
	AssertEqual(base.RemoveIf(predicate),1, "Remove function for event testing");
	// Лоическая операция
	is = istringstream ("date > 2017-03-06 OR event == \"second\"");
    condition = ParseCondition(is);
	AssertEqual(base.RemoveIf(predicate),2, "Remove function for logical operations testing");

	// Тестируем функцию поиска по условию
	// По  дате
	is = istringstream ("date > 2017-03-05");
	condition = ParseCondition(is);
	vector<string> prov1 = {"2017-03-06 Thursday"};
	AssertEqual(base.FindIf(predicate),prov1, "Find function for date testing");

	//По событию
	is = istringstream ("event == \"first\"");
    condition = ParseCondition(is);
    vector<string> prov2 = {"2017-03-03 first"};
	AssertEqual(base.FindIf(predicate),prov2, "Find function for event testing");

	//Логическая операция по дате и событию
	is = istringstream ("date == 2017-03-04 OR event == \"first\"");
    condition = ParseCondition(is);
    vector<string> prov3 = {"2017-03-03 first", "2017-03-04 Tuesday"};
	AssertEqual(base.FindIf(predicate),prov3, "Find function for logical operations testing");

	//Проверяем что выводится последнее добавленное условие
	AssertEqual(base.Last({2017,3,3}),"2017-03-03 Monday", "Command Last. Normal work of the base for vector.size()>1");

	//Проверка Last для даты больше чем максимальная дата в базе
	AssertEqual(base.Last({2017,3,14}),"2017-03-06 Thursday", "Command Last. Normal work of the base for last value");

	//Добавляем к последеней дате еще одно событие и снова делаем проверку Last
	base.Add({2017,3,6},"One_more_event");
	AssertEqual(base.Last({2017,3,14}),"2017-03-06 One_more_event", "Command Last. Normal work of the base for changed last value");

	// Удаляем день посреди недели и подаем эту дату на вход Last
	is = istringstream ("date == 2017-03-04");
    condition = ParseCondition(is);
	AssertEqual(base.RemoveIf(predicate),1, "Remove 2017-03-04 for Last() testing");
	AssertEqual(base.Last({2017,3,4}),"2017-03-03 Monday", "Command Last. Normal work of the base for changed last value");

	TestDataBase_2();
}




void TestDataBase_2() {
  istringstream empty_is("");
  auto empty_condition = ParseCondition(empty_is);
  //cout<<empty_condition->Evaluate({2017,3,14},"");
  auto empty_predicate = [empty_condition](const Date& date, const string& event) {
    return empty_condition->Evaluate(date, event);
  };

  // Add 2 - Del 1 - Add deleted again
  {
    Database db;
    Date d(2019, 1, 1);
    db.Add(d, "e1");
    db.Add(d, "e2");
    istringstream is(R"(event == "e1")");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 1, "Db Add2-Del-Add 1");
    db.Add(d, "e1");
    AssertEqual(db.FindIf(empty_predicate).size(), 2, "Db Add2-Del-Add 2 here");
  }

  // Add
  {
    Database db;
    Date d(2019, 1, 1);
    db.Add(d, "e1");
    db.Add(d, "e1");
    istringstream is("date == 2019-01-01");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.FindIf(predicate).size(), 1, "Db Add Duplicates 1");
  }

  // Last
  {
    Database db;
    Date d(2019, 1, 1);
    Date d1(2019, 1, 2);
    Date d2(2018, 12, 22);
    db.Add(d1, "e1");
    db.Add(d2, "e2");
    AssertEqual(db.Last(d), "2018-12-22 e2", "Db Last 1");
    Date d3(2018, 12, 24);
    db.Add(d3, "e3");
    AssertEqual(db.Last(d), "2018-12-24 e3", "Db Last 2");

    // Get last event for date before first event
    try {
      Date d4(2017, 2, 2);
      db.Last(d4);
      Assert(false, "Db Last 3");
    } catch (invalid_argument e) {
      // Pass
    }

    // Delete event and get last
    istringstream is("date == 2018-12-24");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    db.RemoveIf(predicate);
    AssertEqual(db.Last(d), "2018-12-22 e2", "Db Last 4");

    AssertEqual(db.Last(d1), "2019-01-02 e1", "Db Last 5");
    db.Add(d2, "e4");
    AssertEqual(db.Last(d2), "2018-12-22 e4", "Db Last 6");
  }

  // Del
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is("date == 2018-01-07");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 2, "Db Del 1");
  }
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is("date >= 2018-01-07 AND date <= 2020-01-01");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 4, "Db Del 2");
  }
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    AssertEqual(db.RemoveIf(empty_predicate), 4, "Db Del 3");
  }
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is(R"(event == "e1")");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 1, "Db Del 4");
  }

  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is(R"(event == "e1" OR date == 2019-01-01)");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 2, "Db Del 5");
  }

  // Find
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is("date == 2018-01-07");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.FindIf(predicate).size(), 2, "Db Find 1");
  }
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is("date >= 2018-01-07 AND date <= 2020-01-01");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.FindIf(predicate).size(), 4, "Db Find 2");
  }
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    AssertEqual(db.FindIf(empty_predicate).size(), 4, "Db Find 3");
  }
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is(R"(event == "e1")");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.FindIf(predicate).size(), 1, "Db Find 4");
  }

  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is(R"(event == "e1" OR date == 2019-01-01)");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.FindIf(predicate).size(), 2, "Db Find 5");
  }

  // Add - Del - Add - Del
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    AssertEqual(db.RemoveIf(empty_predicate), 2, "Db Add-Del-Add-Del 1");

    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    AssertEqual(db.RemoveIf(empty_predicate), 2, "Db Add-Del-Add-Del 1");
  }
}
