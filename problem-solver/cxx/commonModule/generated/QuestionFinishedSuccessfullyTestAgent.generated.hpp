#include <memory>

#include "sc-memory/sc_memory.hpp"


#include "sc-memory/sc_event.hpp"




#define QuestionFinishedSuccessfullyTestAgent_hpp_14_init  bool _InitInternal(ScAddr const & outputStructure = ScAddr::Empty) override \
{ \
    ScMemoryContext ctx(sc_access_lvl_make_min, "QuestionFinishedSuccessfullyTestAgent::_InitInternal"); \
    ScSystemIdentifierFiver fiver; \
    bool result = true; \
    return result; \
}


#define QuestionFinishedSuccessfullyTestAgent_hpp_14_initStatic static bool _InitStaticInternal(ScAddr const & outputStructure = ScAddr::Empty)  \
{ \
    ScMemoryContext ctx(sc_access_lvl_make_min, "QuestionFinishedSuccessfullyTestAgent::_InitStaticInternal"); \
    ScSystemIdentifierFiver fiver; \
    bool result = true; \
    return result; \
}


#define QuestionFinishedSuccessfullyTestAgent_hpp_14_decl \
private:\
	typedef ScAgent Super;\
protected: \
	QuestionFinishedSuccessfullyTestAgent(char const * name, sc_uint8 accessLvl) : Super(name, accessLvl) {}\
	virtual sc_result Run(ScAddr const & listenAddr, ScAddr const & edgeAddr, ScAddr const & otherAddr) override; \
private:\
	static std::unique_ptr<ScEvent> ms_event;\
    static std::unique_ptr<ScMemoryContext> ms_context;\
public: \
	static bool handler_emit(ScAddr const & addr, ScAddr const & edgeAddr, ScAddr const & otherAddr)\
	{\
		QuestionFinishedSuccessfullyTestAgent Instance("QuestionFinishedSuccessfullyTestAgent", sc_access_lvl_make_min);\
		return Instance.Run(addr, edgeAddr, otherAddr) == SC_RESULT_OK;\
	}\
	static void RegisterHandler()\
	{\
		ms_context.reset(new ScMemoryContext(sc_access_lvl_make_min, "handler_QuestionFinishedSuccessfullyTestAgent"));\
		ms_event.reset(new ScEvent(*ms_context, scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge, &QuestionFinishedSuccessfullyTestAgent::handler_emit));\
        if (ms_event.get())\
        {\
            SC_LOG_INFO("Register agent QuestionFinishedSuccessfullyTestAgent");\
        }\
        else\
        {\
            SC_LOG_ERROR("Can't register agent QuestionFinishedSuccessfullyTestAgent");\
        }\
	}\
	static void UnregisterHandler()\
	{\
		ms_event.reset();\
		ms_context.reset();\
	}

#define QuestionFinishedSuccessfullyTestAgent_hpp_QuestionFinishedSuccessfullyTestAgent_impl \
std::unique_ptr<ScEvent> QuestionFinishedSuccessfullyTestAgent::ms_event;\
std::unique_ptr<ScMemoryContext> QuestionFinishedSuccessfullyTestAgent::ms_context;

#undef ScFileID
#define ScFileID QuestionFinishedSuccessfullyTestAgent_hpp

