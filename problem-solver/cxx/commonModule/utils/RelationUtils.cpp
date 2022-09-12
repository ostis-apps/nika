#include "RelationUtils.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-memory/sc_memory.hpp"

ScAddr RelationUtils::getIndexRelation(ScMemoryContext * context, int const & index)
{
  if (index <= 0)
  {
    throw std::runtime_error("Invalid index.");
  }
  if (index == 1)
  {
    return scAgentsCommon::CoreKeynodes::rrel_1;
  }
  else if (index == 2)
  {
    return scAgentsCommon::CoreKeynodes::rrel_2;
  }
  else
  {
    return context->HelperFindBySystemIdtf("rrel_" + std::to_string(index));
  }
}

void RelationUtils::eraseAllEdges(
    ScMemoryContext * context,
    const ScAddr & source,
    const ScAddr & target,
    const ScType & edgeType)
{
  ScIterator3Ptr iterator3 = context->Iterator3(source, edgeType, target);
  while (iterator3->Next())
  {
    context->EraseElement(iterator3->Get(1));
  }
};
