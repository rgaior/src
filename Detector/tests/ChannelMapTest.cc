#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include <Configuration.h>
#include <ChannelMap.h>
#include <AugerUnits.h>


using namespace std;


TEST(ChannelMapTest, HandlesMissingConfigLink)
{
  Configuration::GetInstance().DeInit();
  Configuration::GetInstance().Init("HandlesMissingConfigLink-Config.xml", 0);
  ASSERT_THROW({
      ChannelMap channelMap;
    }, runtime_error);
}


TEST(ChannelMapTest, HandlesMissingConfigFile)
{
  Configuration::GetInstance().DeInit();
  Configuration::GetInstance().Init("HandlesMissingConfigFile-Config.xml", 0);
  ASSERT_THROW(ChannelMap channelMap;, runtime_error);
}


TEST(ChannelMapTest, HandlesRedundantDigitizerDefinitions)
{
  Configuration::GetInstance().DeInit();
  Configuration::GetInstance().
    Init("HandlesRedundantDigitizerDefinitions-Config.xml", 0);

  ASSERT_THROW(ChannelMap channelMap;, runtime_error);
}


TEST(ChannelMapTest, HandlesRedundantAntennaDefinitions)
{
  Configuration::GetInstance().DeInit();
  Configuration::GetInstance().
    Init("HandlesRedundantAntennaDefinitions-Config.xml", 0);

  ASSERT_THROW(ChannelMap channelMap;, runtime_error);
}


TEST(ChannelMapTest, HandlesRedundantKGStationDefinitions)
{
  Configuration::GetInstance().DeInit();
  Configuration::GetInstance().
    Init("HandlesRedundantKGStationDefinitions-Config.xml", 0);

  ASSERT_THROW(ChannelMap channelMap;, runtime_error);
}


TEST(ChannelMapTest, HandlesMissingKGStationTriggerTimeRange)
{
  Configuration::GetInstance().DeInit();
  Configuration::GetInstance().
    Init("HandlesMissingKGStationTriggerTimeRange-Config.xml", 0);

  ASSERT_THROW(ChannelMap channelMap;, runtime_error);
}


TEST(ChannelMapTest, HandlesWrongKGStationTriggerTimeRange)
{
  Configuration::GetInstance().DeInit();
  Configuration::GetInstance().
    Init("HandlesWrongKGStationTriggerTimeRange-Config.xml", 0);

  ASSERT_THROW(ChannelMap channelMap;, runtime_error);
}

TEST(ChannelMapTest, HandlesMissingKGStationTriggerTimeOffset)
{
  Configuration::GetInstance().DeInit();
  Configuration::GetInstance().
    Init("HandlesMissingKGStationTriggerTimeOffset-Config.xml", 0);

  ASSERT_THROW(ChannelMap channelMap;, runtime_error);
}


TEST(ChannelMapTest, ParsesCorrectly)
{
  Configuration::GetInstance().DeInit();
  Configuration::GetInstance().Init("ParsesCorrectly-Config.xml", 0);

  ChannelMap channelMap;

  EXPECT_EQ(eAntenna, channelMap.GetSignalSourceType(5, 6));
  EXPECT_EQ((pair<unsigned int, unsigned int>(1, 2)),
            channelMap.GetAntennaChannelId(5, 6));
  EXPECT_THROW(channelMap.GetStationId(5, 6);, runtime_error);

  EXPECT_EQ(eKGStation, channelMap.GetSignalSourceType(11, 12));
  EXPECT_EQ((unsigned int) 19, channelMap.GetStationId(11, 12));
  EXPECT_EQ((pair<double, double>(-2.0 * utl::microsecond,
                                  -1.0 * utl::microsecond)),
            channelMap.GetStationTriggerTimeRange(19));
  EXPECT_EQ((double) -3.2 * utl::nanosecond,
            channelMap.GetStationTriggerTimeOffset(19));
  EXPECT_THROW(channelMap.GetAntennaChannelId(11, 12);, runtime_error);
}
