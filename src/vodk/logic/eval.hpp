#ifndef VODK_LOGIC_EVAL_HPP
#define VODK_LOGIC_EVAL_HPP

namespace vodk {
namespace logic {

class ASTNode;
class RuntimeContext;

void eval(ASTNode* tree, RuntimeContext* ctx);
void eval_test(ASTNode* tree);

} // logic
} // vodk

#endif
