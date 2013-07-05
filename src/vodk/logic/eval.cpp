
#include "vodk/logic/eval.hpp"
#include "vodk/logic/AST.hpp"
#include "vodk/core/Entity.hpp"
#include "vodk/core/Range.hpp"

namespace vodk {
namespace logic {

struct RuntimeContext
{
	RuntimeContext(Range<Entity> e)
	: entities(e)
	{
		params[0] = nullptr;
		params[1] = nullptr;
		params[2] = nullptr;
		params[3] = nullptr;
	}
		

	Range<Entity> entities;
	void* params[4];
};

void eval(ASTNode* node, RuntimeContext* ctx)
{
	if (!node) {
		return;
	}

	switch (node->type()) {
		case AST_CONDITION: {
			ConditionASTNode* cond = node->AsCondition();
			void* params[4];
			for (unsigned i = 0; i < cond->nParams(); ++i) {
				params[i] = ctx->params[cond->paramRegister(i)];
			}
			bool res = cond->evaluate(params);
			if (res && cond->then) {
				eval(cond->then, ctx);
			} else if (!res && cond->otherwise) {
				eval(cond->otherwise, ctx);
			}
			break;
		}
		case AST_ACTION: {
			ActionASTNode* act = node->AsAction();
			void* params[4];
			for (unsigned i = 0; i < act->nParams(); ++i) {
				params[i] = ctx->params[act->paramRegister(i)];
			}
			act->run(params);
			break;
		}
		case AST_ITERATE: {
			IteratorASTNode* n = node->AsIterator();
			Range<Entity> entities = ctx->entities;
			for (unsigned i = 0; i < entities.size(); ++i) {
				if (n->child) {
					ctx->params[n->reg] = &ctx->entities[i];
					eval(n->child, ctx);
				}
			}
		}
	}

	// evaluate next sibling node
	if (node->next) {
		eval(node->next, ctx);
	}
}

void eval_test(ASTNode* n) {
	Entity dummy[10];
	RuntimeContext ctx(
		Range<Entity>(&dummy[0], 10)
	);
	eval(n, &ctx);
}

} // logic
} // vodk
