
#ifndef VODK_LOGIC_BASICNODES_HPP
#define VODK_LOGIC_BASICNODES_HPP

namespace vodk {
namespace logic {

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

class DoNothing : public ActionASTNode
{
public:
	DoNothing(ASTNode* next = nullptr)
	: ActionASTNode(next)
	{}

	virtual const char* name() const override { return "do_nothing"; };

	virtual void run(void* param) override {
		printf("do nothing\n");
	};
};


	
} // logic
} // vodk

#endif
