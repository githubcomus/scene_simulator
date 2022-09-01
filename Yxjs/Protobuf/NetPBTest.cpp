#include "NetPBTest.h"
#include "Core/BaseGameInstance.h"
#include "Net/NetMainComponent.h"
#include "test.pb.h"

#include "Yxjs/../ThirdPart/bp/include/Project2.h"
#include "Yxjs/../ThirdPart/bp/include/TestOutClass.h"

//
NetPBTest::NetPBTest(UBaseGameInstance* gameInstance_, UNetMainComponent* netMainComponent_)
{
	// UE_LOG(LogTemp, Log, TEXT("[%x] [NetPBTest::NetPBTest]"), this);

	{
		char str[128];
		// sprintf_s(str,128, "aaaaaa%d", SSSSRRR);
		// sprintf_s(str, 128, "aaaaaa%d", fnProject2());

		std::shared_ptr<TestOutClass> testOutClass = std::make_shared<TestOutClass>();

		sprintf_s(str, 128, "aaaaaa%d", testOutClass->Add(33, 55));
		std::string str2 = str;
		netMainComponent_->ShowLog(str2);
	}

	{
		Gamedata::FinalMsg info;
		info.set_username("ssr");
		info.set_password("ccsse");

		std::string msg;
		info.SerializeToString(&msg);

		netMainComponent_->ShowLog(msg);
	}
}

//
NetPBTest::~NetPBTest()
{
}
