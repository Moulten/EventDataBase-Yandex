#pragma once

#include "date.h"
#include "string"
#include "token.h"
#include <memory>

struct Event_cond{
	std::string name;
	Comparison sign;
};

class Node{
public:
	Node(const Comparison & cmp);
	virtual bool Evaluate(const Date& date, const string& event) const = 0;
	virtual ~Node(){}
protected:
	Comparison comp;
};
class EmptyNode : public Node{
public:
	EmptyNode();
	bool Evaluate(const Date& date, const string& event) const override;
};

class DateComparisonNode : public Node{
public:
	DateComparisonNode(const Comparison& input_comp, const Date& input_date);
	bool Evaluate(const Date& date, const string& event) const override;
private:
	Date date_comp;
};

class EventComparisonNode : public Node{
public:
	EventComparisonNode(const Comparison& input_comp, const string& input_name);
	bool Evaluate(const Date& date, const string& event) const override;

private:
	std::string name;
};

class LogicalOperationNode : public Node{
public:
	LogicalOperationNode(const LogicalOperation& input_op, shared_ptr<Node> lhs, shared_ptr<Node> rhs);
	bool Evaluate(const Date& date, const string& event) const override;

protected:
	shared_ptr<Node> left, right;
	LogicalOperation op;
};
void TestNode();
