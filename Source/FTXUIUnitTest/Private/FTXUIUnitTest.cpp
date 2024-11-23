#include "FTXUIUnitTest.h"

DEFINE_LOG_CATEGORY(FTXUIUnitTest);

#define LOCTEXT_NAMESPACE "FFTXUIUnitTest"

void FFTXUIUnitTestModule::StartupModule()
{
	UE_LOG(FTXUIUnitTest, Warning, TEXT("FTXUIUnitTest module has been loaded"));
}

void FFTXUIUnitTestModule::ShutdownModule()
{
	UE_LOG(FTXUIUnitTest, Warning, TEXT("FTXUIUnitTest module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFTXUIUnitTestModule, FTXUIUnitTest)

#include <gtest/gtest.h>

#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFTXUIUnitTest, "FTXUI.UnitTest", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFTXUIUnitTest::RunTest(const FString& Parameters)
{ 
	::testing::InitGoogleTest();

	return RUN_ALL_TESTS() == 0;
}