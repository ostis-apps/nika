#include "MathConstantsManager.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"

#include "keynodes/Keynodes.hpp"

namespace mathModule
{
MathConstantsManager::MathConstantsManager(ScMemoryContext * context)
  : context(context)
{
}

ScAddrVector MathConstantsManager::manage(ScAddrVector const & processParameters) const
{
  ScAddr addr = processParameters.at(0);

  std::string name = getMathConstantName(addr);
  std::string value = getMathConstantValue(name);
  ScAddrVector answerElements;

  const auto & valueElements = addTempLinkWithRelation(addr, value, Keynodes::nrel_math_constant_value);
  answerElements.insert(answerElements.cend(), valueElements.cbegin(), valueElements.cend());

  return answerElements;
}

std::string MathConstantsManager::getMathConstantName(ScAddr addr) const
{
  const auto & mathConstantNameIterator = context->Iterator5(
      addr,
      ScType::EdgeDCommonConst,
      ScType::LinkConst,
      ScType::EdgeAccessConstPosPerm,
      Keynodes::nrel_math_constant_name);
  if (mathConstantNameIterator->Next())
  {
    return utils::CommonUtils::getLinkContent(context, mathConstantNameIterator->Get(2));
  }
  else
  {
    SC_THROW_EXCEPTION(
        utils::ExceptionItemNotFound,
        "MathConstantsManager: The value not found for " + context->HelperGetSystemIdtf(addr));
  }
}

std::string MathConstantsManager::getMathConstantValue(const std::string & mathConstantName) const
{
  std::ifstream in(MATH_CONSTANTS_RESOURCES);

  std::string line;
  while (std::getline(in, line))
  {
    size_t i = line.find('=');
    if (i == std::string::npos || i + 1 >= line.length())
    {
      SC_THROW_EXCEPTION(
          utils::ExceptionParseError, "MathConstantsManager: The value not found for " + mathConstantName);
    }

    std::string name = line.substr(0, i);
    if (name == mathConstantName)
    {
      return line.substr(i + 1);
    }
  }

  SC_THROW_EXCEPTION(utils::ExceptionParseError, "MathConstantsManager: The value not found for " + mathConstantName);
}

ScAddrVector MathConstantsManager::addTempLinkWithRelation(
    ScAddr addr,
    const std::string & linkContent,
    ScAddr relation) const
{
  const auto & link = context->CreateLink(ScType::LinkConst);
  context->SetLinkContent(link, ScStreamConverter::StreamFromString(linkContent));
  const auto & edgeDCommon = context->CreateEdge(ScType::EdgeDCommonConst, addr, link);
  const auto & tempArc = context->CreateArc(ScType::EdgeAccessConstPosTemp, relation, edgeDCommon);
  return {link, relation, edgeDCommon, tempArc};
}

}  // namespace mathModule