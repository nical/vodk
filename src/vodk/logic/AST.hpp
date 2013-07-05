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

/**
 * Node that represents a branch.
 * have no side-effect
 */
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

/**
 * Node that represents an action
 * may have side effetcs.
 */
class ActionASTNode : public ASTNode
{
public:
	ActionASTNode(ASTNode* aNext = nullptr)
	: ASTNode(aNext)
	{}

	virtual ~ActionASTNode() {}

	virtual ASTNodeType type() const override { return AST_ACTION; };

	virtual ActionASTNode* AsAction() override { return this; }

	virtual void dump(uint32_t indent = 0) override;

	virtual const char* name() const = 0;

	virtual void run(void* param) = 0;

	/**
	 * number of arguments
	 */
	virtual uint8_t nParams() { return 0; }
	/**
	 * what register to fetch for a given argument
	 */
	virtual ASTRegister paramRegister(uint8_t param) { return X; };
};

/**
 * A Node that lets us traverse all the objects and execute it's sub-tree in
 * function of the object
 */
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


namespace ast {

ASTNode* ForEach(ASTNode* child);
ASTNode* If(ConditionASTNode* a, ASTNode* then, ASTNode* otherwise = nullptr);
ASTNode* Not(ConditionASTNode* a, ASTNode* then);
ASTNode* And(ConditionASTNode* a, ConditionASTNode* b, ASTNode* then);
ASTNode* Then(ActionASTNode* a);

void MergeTrees(ASTNode* a, ASTNode* b);

void unittest();

} // ast

} // logic
} // vodk

#endif
