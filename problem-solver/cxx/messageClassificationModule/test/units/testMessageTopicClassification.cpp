#include "sc_test.hpp"
#include "scs_loader.hpp"

#include "agent/MessageTopicClassificationAgent.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "utils/ActionUtils.hpp"
#include "client/WitAiClient.hpp"
#include "WitAiCkientMock.hpp"

using namespace messageClassificationModule;

namespace messageTopicClassificationTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = MESSAGE_TOPIC_CLASSIFICATION_MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

using MessageTopicClassificationTest = ScMemoryTest;

void initialize()
{
  scAgentsCommon::CoreKeynodes::InitGlobal();
  MessageClassificationKeynodes::InitGlobal();
  commonModule::Keynodes::InitGlobal();
}

void shutdown()
{
}

TEST_F(MessageTopicClassificationTest, connectionTest)
{
  auto * client = new WitAiClientMock();
  std::string witResponse =
      R"({"entities":{},"intents":[{"confidence":0.8592,"id":"4251337931554570","name":"start_greeting"}],
              "text":"Hello.","traits":{"wit$sentiment":[{"confidence":0.65,"id":"5ac2b50a-44e4-466e-9d49-bad6bd40092c",
              "value":"positive"}]}})";
  EXPECT_CALL(*client, getWitResponse(testing::_))
      .Times(testing::Exactly(1))
      .WillOnce(testing::Return(json::parse(witResponse)));
  json response = client->getWitResponse("Hello maksim from summer");

  EXPECT_FALSE(response.empty());

  delete client;
}

TEST_F(MessageTopicClassificationTest, classifyMessageWithoutEntityTest)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "hello_message.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "wit_concepts.scs");
  initialize();

  ScAddr greetingMessageClass = context.HelperFindBySystemIdtf("concept_greeting_message");
  ScAddr messageAddr = context.HelperFindBySystemIdtf("message");
  EXPECT_TRUE(greetingMessageClass.IsValid());
  EXPECT_TRUE(messageAddr.IsValid());

  auto * client = new WitAiClientMock();
  std::string witResponse =
      R"({"entities":{},"intents":[{"confidence":0.8592,"id":"4251337931554570","name":"start_greeting"}],
              "text":"Hello.","traits":{"wit$sentiment":[{"confidence":0.65,"id":"5ac2b50a-44e4-466e-9d49-bad6bd40092c",
              "value":"positive"}]}})";
  EXPECT_CALL(*client, getWitResponse(testing::_))
      .Times(testing::Exactly(1))
      .WillOnce(testing::Return(json::parse(witResponse)));

  MessageTopicClassifier classifier(&context, client);
  ScAddrVector messageClassificationItems = classifier.classifyMessage(messageAddr);
  EXPECT_FALSE(messageClassificationItems.empty());

  bool isMessageClassified = context.HelperCheckEdge(greetingMessageClass, messageAddr, ScType::EdgeAccessConstPosPerm);
  EXPECT_TRUE(isMessageClassified);

  shutdown();
}

TEST_F(MessageTopicClassificationTest, classifyMessageWithEntityTest)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "hello_message_with_entity.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "wit_concepts.scs");
  initialize();

  ScAddr greetingMessageClass = context.HelperFindBySystemIdtf("concept_greeting_message");
  ScAddr messageAddr = context.HelperFindBySystemIdtf("message");
  ScAddr entityAddr = context.HelperFindBySystemIdtf("maksim");
  ScAddr entityRoleAddr = context.HelperFindBySystemIdtf("rrel_contact");

  EXPECT_TRUE(greetingMessageClass.IsValid());
  EXPECT_TRUE(messageAddr.IsValid());
  EXPECT_TRUE(entityAddr.IsValid());
  EXPECT_TRUE(entityRoleAddr.IsValid());

  auto * client = new WitAiClientMock();
  std::string witResponse =
      R"({"entities":{"wit$contact:contact":[{"body":"Maksim","confidence":0.9059,"end":12,"entities":{},
              "id":"210261890968181","name":"wit$contact", "role":"contact","start":6,"suggested":true,
              "type":"value","value":"Maksim"}]},"intents":[{"confidence":0.9994,"id":"4251337931554570",
              "name":"start_greeting"}],"text":"Hello Maksim","traits":{"wit$sentiment":[{"confidence":0.6224,
              "id":"5ac2b50a-44e4-466e-9d49-bad6bd40092c","value":"neutral"}]}})";
  EXPECT_CALL(*client, getWitResponse(testing::_))
      .Times(testing::Exactly(1))
      .WillOnce(testing::Return(nlohmann::json::parse(witResponse)));

  MessageTopicClassifier classifier(&context, client);

  ScAddrVector messageClassificationItems = classifier.classifyMessage(messageAddr);

  EXPECT_FALSE(messageClassificationItems.empty());

  ScTemplate classificationTemplate;
  classificationTemplate.Triple(greetingMessageClass, ScType::EdgeAccessVarPosPerm, messageAddr);
  classificationTemplate.TripleWithRelation(
      messageAddr, ScType::EdgeAccessVarPosPerm, entityAddr, ScType::EdgeAccessVarPosPerm, entityRoleAddr);

  ScTemplateSearchResult classificationTemplateResult;
  context.HelperSearchTemplate(classificationTemplate, classificationTemplateResult);
  EXPECT_TRUE(classificationTemplateResult.Size() == 1);

  shutdown();
}

TEST_F(MessageTopicClassificationTest, classifyMessageWithTwoEntitiesTest)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "hello_message_with_two_entities.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "wit_concepts.scs");
  initialize();

  ScAddr weatherMessageClass = context.HelperFindBySystemIdtf("concept_general_message_about_weather");
  ScAddr neutralMessageClass = context.HelperFindBySystemIdtf("concept_message_with_neutral_emotional_coloring");
  ScAddr messageAddr = context.HelperFindBySystemIdtf("message");
  ScAddr entityContactAddr = context.HelperFindBySystemIdtf("maksim");
  ScAddr entitySeasonAddr = context.HelperFindBySystemIdtf("summer");
  ScAddr rrelContactAddr = context.HelperFindBySystemIdtf("rrel_contact");
  ScAddr rrelSeasonAddr = context.HelperFindBySystemIdtf("rrel_season");

  EXPECT_TRUE(weatherMessageClass.IsValid());
  EXPECT_TRUE(messageAddr.IsValid());
  EXPECT_TRUE(entityContactAddr.IsValid());
  EXPECT_TRUE(entitySeasonAddr.IsValid());
  EXPECT_TRUE(rrelContactAddr.IsValid());
  EXPECT_TRUE(rrelSeasonAddr.IsValid());

  auto * client = new WitAiClientMock();
  std::string witResponse =
      R"({"entities":
              {"season:season":[{"body":"summer","confidence":0.9995,"end":24,"entities":{},"id":"530932871575541",
              "name":"season","role":"season","start":18,"type":"value","value":"summer"}],"wit$contact:contact":
              [{"body":"Maksim","confidence":0.9356,"end":12,"entities":{},"id":"210261890968181","name":"wit$contact",
              "role":"contact","start":6,"suggested":true,"type":"value","value":"Maksim"}]},"intents":
              [{"confidence":0.6678,"id":"607208973598157","name":"weather"}],"text":"Hello Maksim from summer.",
              "traits":{"wit$sentiment":[{"confidence":0.5628,"id":"5ac2b50a-44e4-466e-9d49-bad6bd40092c",
              "value":"neutral"}]}})";
  EXPECT_CALL(*client, getWitResponse(testing::_))
      .Times(testing::Exactly(1))
      .WillOnce(testing::Return(nlohmann::json::parse(witResponse)));

  MessageTopicClassifier classifier(&context, client);

  ScAddrVector messageClassificationItems = classifier.classifyMessage(messageAddr);

  EXPECT_FALSE(messageClassificationItems.empty());

  ScTemplate classificationTemplate;
  classificationTemplate.Triple(weatherMessageClass, ScType::EdgeAccessVarPosPerm, messageAddr);
  classificationTemplate.Triple(neutralMessageClass, ScType::EdgeAccessVarPosPerm, messageAddr);
  classificationTemplate.TripleWithRelation(
      messageAddr, ScType::EdgeAccessVarPosPerm, entityContactAddr, ScType::EdgeAccessVarPosPerm, rrelContactAddr);
  classificationTemplate.TripleWithRelation(
      messageAddr, ScType::EdgeAccessVarPosPerm, entitySeasonAddr, ScType::EdgeAccessVarPosPerm, rrelSeasonAddr);

  ScTemplateSearchResult classificationTemplateResult;
  context.HelperSearchTemplate(classificationTemplate, classificationTemplateResult);

  EXPECT_TRUE(classificationTemplateResult.Size() == 1);

  shutdown();
}

}  // namespace messageTopicClassificationTest
