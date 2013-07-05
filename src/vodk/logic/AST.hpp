#ifndef VODK_LOGIC_AST_HPP
#define VODK_LOGIC_AST_HPP

#include <stdint.h>
#include <stdio.h>

namespace vodk {
namespace logic {

enum ASTNodeType {
	AST_ITERATE,
	AST_CONDITION,
	AST_ACTION,
	AST_NONE
};

enum ASTRegister { A, B, C, D, X };

class ActionParam {

};

/**
 * TODO - this should inherit from ActionASTNode instead;
 */
class Action {
public:
	virtual ~Action() {}
	virtual void run(ActionParam* param) = 0;
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
	ConditionASTNode(ASTNode* aThen = nullptr,
					 ASTNode* aElse = nullptr,
					 ASTNode* aNext = nullptr)
	: ASTNode(aNext)
	, then(aThen)
	, otherwise(aElse)
	{
	}

	virtual ~ConditionASTNode()
	{
		delete then;
		delete otherwise;
	}

	virtual ASTNodeType type() const override { return AST_CONDITION; };

	virtual ConditionASTNode* AsCondition() override { return this; }

	virtual void dump(uint32_t indent = 0) override;
	virtual const char* name() const = 0;

	virtual bool evaluate(void* paramas) = 0;

	/**
	 * number of arguments
	 */
	virtual uint8_t nParams() { return 0; }
	/**
	 * what register to fetch for a given argument
	 */
	virtual ASTRegister paramRegister(uint8_t param) { return X; };

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
	IteratorASTNode(uint8_t regIndex, ASTNode* child, ASTNode* aNext = nullptr)
	: ASTNode(aNext)
	, child(child)
	, reg(regIndex)
	{}

	virtual ~IteratorASTNode() { delete child; }

	virtual ASTNodeType type() const override { return AST_ITERATE; };

	virtual IteratorASTNode* AsIterator() override { return this; }

	virtual void dump(uint32_t indent = 0) override;

	/**
	 * First child node to evaluate for each element
	 */
	ASTNode* child;
	/**
	 * ID of the module to iterate on (0 for entities)
	 */
	uint8_t srcID;
	/**
	 * index of the register in which the currently iterated element is kept
	 */
	uint8_t reg;
};

class AlwaysTrue : public ConditionASTNode
{
public:
	AlwaysTrue(ASTNode* aThen = nullptr,
			   ASTNode* aElse = nullptr,
			   ASTNode* aNext = nullptr)
	: ConditionASTNode(aThen, aElse, aNext)
	{}
	virtual bool evaluate(void* args) { return true; }
	virtual const char* name() const override { return "always_true"; }
};

class Sometimes : public ConditionASTNode
{
public:
	Sometimes(ASTNode* aThen = nullptr,
			   ASTNode* aElse = nullptr,
			   ASTNode* aNext = nullptr)
	: ConditionASTNode(aThen, aElse, aNext)
	, thisTime(false)
	{}
	virtual bool evaluate(void* args) {
		thisTime = !thisTime;
		printf("sometimes ? %i\n", (int)thisTime);
		return thisTime;
	}
	virtual const char* name() const override { return "sometimes"; }
	bool thisTime;
};

class Compare : public ConditionASTNode
{
public:
	Compare(ASTRegister p0, ASTRegister p1,
			ASTNode* aThen = nullptr,
			ASTNode* aElse = nullptr,
			ASTNode* aNext = nullptr)
	: ConditionASTNode(aThen, aElse, aNext)
	{
		regs[0] = p0;
		regs[1] = p1;
	}

	virtual uint8_t nParams() override { return 2; }

	virtual ASTRegister paramRegister(uint8_t p) override { return regs[p]; }

	virtual bool evaluate(void* args) override {
		printf("compare ? %p, %p\n", ((void**)args)[0], ((void**)args)[1]);
		return true;
	}

	virtual const char* name() const override {return "compare"; }

	ASTRegister regs[2];
};

class DoNothing : public Action
{
public:
	virtual void dump() override;
	virtual void run(ActionParam* param) override {
		printf("do nothing\n");
	};

};


namespace ast {

ASTNode* ForEach(ASTNode* child);
ASTNode* If(ConditionASTNode* a, ASTNode* then, ASTNode* otherwise = nullptr);
ASTNode* Not(ConditionASTNode* a, ASTNode* then);
ASTNode* And(ConditionASTNode* a, ConditionASTNode* b, ASTNode* then);
ASTNode* Then(Action* a);

void mergeTrees(ASTNode* a, ASTNode* b);

void unittest();

} // ast

} // logic
} // vodk


#endif
