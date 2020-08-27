#include "node.h"

using namespace std;

Node::Node(const Comparison & cmp) : comp(cmp){}

EmptyNode::EmptyNode()
	:Node(Comparison::Equal){}

bool EmptyNode::Evaluate(const Date& date, const string& event) const {
	return true;
}

DateComparisonNode::DateComparisonNode(const Comparison& input_comp, const Date& input_date)
					: Node(input_comp), date_comp(input_date){}

bool DateComparisonNode::Evaluate(const Date& date, const string& event) const{
	switch(comp){
		case Comparison::Equal:
			return date == date_comp;
		case Comparison::Greater:
			return date > date_comp;
		case Comparison::GreaterOrEqual:
			return date >= date_comp;
		case Comparison::Less:
			return date < date_comp;
		case Comparison::LessOrEqual:
			return date <= date_comp;
		case Comparison::NotEqual:
			return date != date_comp;
		default:
			return false;
	}
}

EventComparisonNode::EventComparisonNode(const Comparison& input_comp, const string& input_name)
						:Node(input_comp),name(input_name){}

bool EventComparisonNode::Evaluate(const Date& date, const string& event) const {
	switch(comp){
		case Comparison::Equal:
			return event == name;
		case Comparison::Greater:
			return event > name;
		case Comparison::GreaterOrEqual:
			return event >= name;
		case Comparison::Less:
			return event < name;
		case Comparison::LessOrEqual:
			return event <= name;
		case Comparison::NotEqual:
			return event != name;
		default:
			return false;
	}
}

LogicalOperationNode::LogicalOperationNode(const LogicalOperation& input_op, shared_ptr<Node> lhs, shared_ptr<Node> rhs)
						: Node(Comparison::Equal),op(input_op),
						  left(lhs), right(rhs){}

bool LogicalOperationNode::Evaluate(const Date& date, const string& event) const {
	switch(op){
		case LogicalOperation::And:
			return left->Evaluate(date,event) && right->Evaluate(date,event);
		case LogicalOperation::Or:
			return left->Evaluate(date,event) || right->Evaluate(date,event);
		default:
			return false;
	}
}

