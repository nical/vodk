#ifndef VODK_LOGIC_EVAL_HPP
#define VODK_LOGIC_EVAL_HPP

#include <stdint.h>
#include <vodk/core/Range.hpp>

namespace vodk {
namespace logic {

class ASTNode;
class RuntimeContext;

typedef uint8_t RuntimeModuleID;

class RuntimeModule
{
public:
	virtual RuntimeModuleID get_id() = 0;
	virtual DynamicRange getElements(int32_t flags = 0);
};



void eval(ASTNode* tree, RuntimeContext* ctx);

void eval_test(ASTNode* tree);

} // logic
} // vodk

#endif
