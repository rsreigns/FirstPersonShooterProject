// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSProjectGameMode.h"
#include "FPSProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "HttpModule.h"
#include "Json.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "DebugHelper.h"

AFPSProjectGameMode::AFPSProjectGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

//void AFPSProjectGameMode::RetrieveDataFromJSON()
//{
//	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
//	Request->SetURL("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");
//	Request->SetVerb("GET");
//
//	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded)
//	{
//		if (bSucceeded && Response.IsValid())
//		{
//			FString ResponseString = Response->GetContentAsString();
//
//			TArray<TSharedPtr<FJsonValue>> JSONArray;
//			TSharedPtr<FJsonObject> JSONObject;
//
//			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
//			if (FJsonSerializer::Deserialize(Reader, JSONArray))
//			{
//				for (const auto& JsonValue : JSONArray)
//				{
//					if (JSONArray.IsEmpty()) return;
//
//					TArray<TSharedPtr<FJsonValue>> TypesArray = JSONObject->GetArrayField("types");
//					if (TypesArray.Num()>0)
//					{
//						FJSONObjectData CurrentData;
//
//						CurrentData.Name = JSONObject->GetStringField("name");
//						DEBUG::PrintString(CurrentData.Name);
//						CurrentData.Health = JSONObject->GetNumberField("health");
//						CurrentData.Score = JSONObject->GetNumberField("score");
//
//						TArray<TSharedPtr<FJsonValue>> ColorArray = JSONObject->GetArrayField("color");
//
//						CurrentData.ColorR = ColorArray[0]->AsNumber();
//						CurrentData.ColorG = ColorArray[1]->AsNumber();
//						CurrentData.ColorB = ColorArray[2]->AsNumber();
//
//						JSONObjectData.Add(CurrentData);
//					}
//
//
//					
//
//
//
//
//
//					
//				}
//			}
//		}
//	}
//	);
//
//	Request->ProcessRequest();
//}
//
//void AFPSProjectGameMode::PostLogin(APlayerController* NewPlayer)
//{
//	DEBUG::PrintString("Game mode post login inititated");
//	if (NewPlayer)
//	{
//		RetrieveDataFromJSON();
//	}
//}
//
