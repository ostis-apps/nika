#pragma once

#include <sc-memory/sc_memory.hpp>

class RelationUtils
{
public:
  static ScAddr getIndexRelation(ScMemoryContext * context, int const & index);

  static void eraseAllEdges(
      ScMemoryContext * context,
      const ScAddr & source,
      const ScAddr & target,
      const ScType & edgeType);
};
