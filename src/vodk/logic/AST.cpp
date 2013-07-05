
#include "vodk/logic/AST.hpp"
#include <stdio.h>

namespace vodk {
namespace logic {

void dumpIndentation(uint32_t i)
{
	while (i--) { printf("  "); }
}

void ConditionASTNode::dump(uint32_t indent)
{
	dumpIndentation(indent);
	printf("[if] ");
	condition->dump();
	printf("\n");
	if (then) {
		then->dump(indent+1);
	}
	if (otherwise) {
		dumpIndentation(indent);
		printf("[else]\n");
		otherwise->dump(indent+1);
	}
	if (next) {
		next->dump(indent);
	}
}

void ActionASTNode::dump(uint32_t indent)
{
	dumpIndentation(indent);
	printf("[action] ");
	action->dump();
	printf("\n");
	if (next) {
		next->dump(indent);
	}
}

void IteratorASTNode::dump(uint32_t indent)
{
	dumpIndentation(indent);
	printf("[foreach]\n");
	if (child) {
		child->dump(indent+1);
	}
	if (next) {
		next->dump(indent);
	}
}

void AlwaysTrue::dump()
{
	printf("(true)");
}

void DoNothing::dump()
{
	printf("(do nothing)");
}


namespace ast {

ASTNode* ForEach(ASTNode* child)
{
	return new IteratorASTNode(child, nullptr);
}

ASTNode* If(Condition* condition, ASTNode* then, ASTNode* otherwise)
{
	return new ConditionASTNode(condition, then, otherwise);
}

ASTNode* Then(Action* a)
{
	return new ActionASTNode(a);
}

ASTNode* Not(Condition* condition, ASTNode* then)
{
	return If(condition, nullptr, then);
}

ASTNode* And(Condition* a, Condition* b, ASTNode* then)
{
	return If(a,If(b, then));
}

const char* nodeTypeStr(ASTNodeType type)
{
	switch (type) {
		case AST_ITERATE: return "foreach";
		case AST_CONDITION: return "if";
		case AST_ACTION: return "action";
		default: return "";
	}
}

void unittest()
{
	ASTNode* n1 = ForEach(
		If(new AlwaysTrue,
			Then(new DoNothing)
		,// else
			Then(new DoNothing)
		)
	);
	n1->dump();
	delete n1;
}

} // ast

} // logic
} // vodk