
#include "vodk/logic/AST.hpp"
#include "vodk/logic/runtime.hpp"
#include "vodk/logic/BasicNodes.hpp"
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
	printf("[if] %s(", name());
	for (unsigned i = 0; i < num_params(); ++i) {
		printf("%c", param_registers(i)+'A');
		if (i+1 < num_params()) {
			printf(", ");
		}
	}
	printf(")\n");
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
	printf("[action] %s(", name());
	for (unsigned i = 0; i < num_params(); ++i) {
		printf("%c", param_registers(i)+'A');
		if (i+1 < num_params()) {
			printf(", ");
		}
	}
	printf(")\n");
	if (next) {
		next->dump(indent);
	}
}

void IteratorASTNode::dump(uint32_t indent)
{
	dumpIndentation(indent);
	printf("[foreach %c]\n", (char)reg+'A');
	if (child) {
		child->dump(indent+1);
	}
	if (next) {
		next->dump(indent);
	}
}

namespace ast {

ASTNode* ForEach(ASTRegister reg, ASTNode* child)
{
	return new IteratorASTNode(reg, child, nullptr);
}

ASTNode* If(ConditionASTNode* condition, ASTNode* then, ASTNode* otherwise)
{
	condition->then = then;
	condition->otherwise = otherwise;
	return condition;
}

ASTNode* Then(ActionASTNode* a)
{
	return a;
}

ASTNode* Not(ConditionASTNode* condition, ASTNode* then)
{
	return If(condition, nullptr, then);
}

ASTNode* And(ConditionASTNode* a, ConditionASTNode* b, ASTNode* then)
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

} // ast
} // logic

namespace unittest {

using namespace vodk::logic::ast;
using namespace vodk::logic;

void ast()
{
    ASTNode* n1 = ForEach( A,
        If(new Sometimes,
            ForEach( B,
                If(new Compare(A, B),
                    Then(new DoNothing)
                )
            )
        ,// else
            Then(new DoNothing)
        )
    );
    n1->dump();
    eval_test(n1);
    delete n1;
}

} // unittest

} // vodk