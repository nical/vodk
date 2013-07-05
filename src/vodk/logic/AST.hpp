#ifndef VODK_LOGIC_AST_HPP
#define VODK_LOGIC_AST_HPP

#include <stdint.h>

namespace vodk {
namespace logic {

enum ASTNodeType {
	AST_ITERATE,
	AST_CONDITION,
	AST_ACTION,
	AST_NONE
};

class ConditionParam {

};

class Condition {
public:
	virtual ~Condition() {}
	virtual bool isSame(Condition* other) = 0;
	virtual bool evaluate(ConditionParam* params) = 0;
	virtual void dump() = 0;
};

class Action {
public:
	virtual ~Action() {}
	virtual void dump() = 0;
};

typedef uint32_t ForEachClassID;

class ConditionASTNode;
class ActionASTNode;
class IteratorASTNode;

class ASTNode {
public:
	ASTNode(ASTNode* nextSibling) : next(nextSibling) {}

	virtual ~ASTNode() {}

	virtual ASTNodeType type() const = 0;

	virtual ConditionASTNode* AsCondition() { return nullptr; }

	virtual ActionASTNode* AsAction() { return nullptr; }

	virtual IteratorASTNode* AsIterator() { return nullptr; }

	virtual void dump(uint32_t indent = 0) = 0;

	ASTNode* next;
};

class ConditionASTNode : public ASTNode
{
public:
	ConditionASTNode(Condition* cond,
					 ASTNode* aThen = nullptr,
					 ASTNode* aElse = nullptr,
					 ASTNode* aNext = nullptr)
	: ASTNode(aNext)
	, condition(cond)
	, then(aThen)
	, otherwise(aElse)
	{}

	virtual ~ConditionASTNode()
	{
		delete condition;
		delete then;
		delete otherwise;
	}

	virtual ASTNodeType type() const override { return AST_CONDITION; };

	virtual ConditionASTNode* AsCondition() override { return this; }

	virtual void dump(uint32_t indent = 0) override;

	Condition* condition;
	ASTNode* then;
	ASTNode* otherwise;
};

class ActionASTNode : public ASTNode
{
public:
	ActionASTNode(Action* a, ASTNode* aNext = nullptr)
	: ASTNode(aNext)
	, action(a)
	{}

	virtual ~ActionASTNode() { delete action; }

	virtual ASTNodeType type() const override { return AST_ACTION; };

	virtual ActionASTNode* AsAction() override { return this; }

	virtual void dump(uint32_t indent = 0) override;

	Action* action;
};

class IteratorASTNode : public ASTNode
{
public:
	IteratorASTNode(ASTNode* child, ASTNode* aNext = nullptr)
	: ASTNode(aNext)
	, child(child)
	{}

	virtual ~IteratorASTNode() { delete child; }

	virtual ASTNodeType type() const override { return AST_ITERATE; };

	virtual IteratorASTNode* AsIterator() override { return this; }

	virtual void dump(uint32_t indent = 0) override;

	ASTNode* child;
};

class AlwaysTrue : public Condition
{
public:
	virtual bool isSame(Condition* c) { return false; }
	virtual bool evaluate(ConditionParam* p) { return true; }
	virtual void dump() override;
};

class DoNothing : public Action
{
public:
	virtual void dump() override;
};


namespace ast {

ASTNode* ForEach(ASTNode* child);
ASTNode* If(Condition* a, ASTNode* then, ASTNode* otherwise = nullptr);
ASTNode* Not(Condition* a, ASTNode* then);
ASTNode* And(Condition* a, Condition* b, ASTNode* then);
ASTNode* Then(Action* a);

void mergeTrees(ASTNode* a, ASTNode* b);

void unittest();

} // ast

} // logic
} // vodk


#endif
