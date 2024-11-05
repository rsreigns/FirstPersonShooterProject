// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "HttpModule.h"
#include "Json.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "BoxToSpawn.h"
#include "MyUserScoreWidget.h"
#include "Blueprint/UserWidget.h"
#include "HISMSpawner.h"


//#include "TESTSpawnBox.h"
#include "FPSProject/DebugHelper.h"
#include "DrawDebugHelpers.h"




void AFPSGameMode::RetrieveDataFromJSON()
{
	DEBUG::PrintString("retrieving data");
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");
	Request->SetVerb("GET");

	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded)
	{
		if (bSucceeded && Response.IsValid())
		{
			FString ResponseString = Response->GetContentAsString();

			TSharedPtr<FJsonObject> JSONObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

			// Deserialize JSON string into a JSON object
			if (FJsonSerializer::Deserialize(Reader, JSONObject) && JSONObject.IsValid())
			{
				// Retrieve the "types" array from the JSON object
				const TArray<TSharedPtr<FJsonValue>>* TypesArray;
				if (JSONObject->TryGetArrayField("types", TypesArray))
				{
					for (const auto& JsonValue : *TypesArray)
					{
						TSharedPtr<FJsonObject> TypeObject = JsonValue->AsObject();
						if (TypeObject.IsValid())
						{
							FJSONObjectData CurrentData;

							CurrentData.Name = TypeObject->GetStringField("name");
							CurrentData.Health = TypeObject->GetNumberField("health");
							CurrentData.Score = TypeObject->GetNumberField("score");
							// Retrieve the color array and set RGB values
							const TArray<TSharedPtr<FJsonValue>>* ColorArray;
							if (TypeObject->TryGetArrayField("color", ColorArray) && ColorArray->Num() == 3)
							{
								CurrentData.ColorR = (*ColorArray)[0]->AsNumber(); 
								CurrentData.ColorG = (*ColorArray)[1]->AsNumber();
								CurrentData.ColorB = (*ColorArray)[2]->AsNumber();
							}
							else
							{
								DEBUG::PrintString("Color array missing or invalid", 5.f, FColor::Red);
							}

							// Add the CurrentData to JSONObjectData array
							JSONObjectData.Add(CurrentData);
						}
					}
				}
				else
				{
					DEBUG::PrintString("Types array not found", 5.f, FColor::Red);
				}
				const TArray<TSharedPtr<FJsonValue>>* ObjectsArray;
				if (JSONObject->TryGetArrayField("objects", ObjectsArray))
				{
					int32  InstanceIndex = 0;
					for (const auto& JsonValue : *ObjectsArray)
					{
						TSharedPtr<FJsonObject> TypeObject = JsonValue->AsObject();
						if (TypeObject.IsValid())
						{
							for (FJSONObjectData& It : JSONObjectData)
							{
								
								if (It.Name == TypeObject->GetStringField("type"))
								{
									const TArray<TSharedPtr<FJsonValue>>* LocationArray;
									const TArray<TSharedPtr<FJsonValue>>* RotationArray;
									const TArray<TSharedPtr<FJsonValue>>* ScaleArray;
									FVector Location, Scale;
									FRotator Rotation;

									if (TypeObject->HasTypedField<EJson::Object>("transform"))
									{
										TSharedPtr<FJsonObject> TransformObject = TypeObject->GetObjectField("transform");
										if (TransformObject->TryGetArrayField("location", LocationArray) && LocationArray->Num() == 3)
										{
											Location= FVector(
												(*LocationArray)[0]->AsNumber(),
												(*LocationArray)[1]->AsNumber(),
												(*LocationArray)[2]->AsNumber()
											);
										}
										if (TransformObject->TryGetArrayField("rotation", RotationArray) && RotationArray->Num() == 3)
										{
											Rotation= FRotator(
												(*RotationArray)[0]->AsNumber(),
												(*RotationArray)[1]->AsNumber(),
												(*RotationArray)[2]->AsNumber()
											);
										}
										if (TransformObject->TryGetArrayField("scale", ScaleArray) && ScaleArray->Num() == 3)
										{
											Scale = FVector(
												(*ScaleArray)[0]->AsNumber(),
												(*ScaleArray)[1]->AsNumber(),
												(*ScaleArray)[2]->AsNumber()
											);
										}

									}

									//spawn logic
									FActorSpawnParameters SpawnParams;
									SpawnParams.Owner = this;
									SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
									
									ABoxToSpawn* SpawnedBox = GetWorld()->SpawnActor<ABoxToSpawn>(ABoxToSpawn::StaticClass()
										, Location, Rotation, SpawnParams);
									if (SpawnedBox)
									{
										FTransform Transform(Rotation, Location, Scale);
		
										HISMObject->GetPoolObject()->ApplyDefaults(It.ColorR, It.ColorG, It.ColorB, It.Health, It.Score, Transform, HISMObject->ISMComp, InstanceIndex);
										
										InstanceIndex+=1;
										//DEBUG::PrintString(FString::Printf(TEXT("Health : %f, Score : %f, Color : %f / %f / %f"),
										//	It.Health,It.Score, SpawnedBox->ColorX , SpawnedBox->ColorY, SpawnedBox->ColorZ),30.f,FColor::Black);
									}
									else
									{
										DEBUG::PrintString("Could not spawn", 5.f, FColor::Red);
									}

								}
							}
							
						}
					}
				}
				else
				{
					DEBUG::PrintString("Objects array not found", 5.f, FColor::Red);
				}
			}
			else
			{
				DEBUG::PrintString("Failed to deserialize JSON", 5.f, FColor::Red);
			}
		}
		else
		{
			DEBUG::PrintString("Failed or response invalid", 5.f, FColor::Red);
		}
	});
	OnScoreChanged.Broadcast(PlayerScore);
	Request->ProcessRequest();
}

void AFPSGameMode::AddPlayerScore(float ScoreToAdd)
{
	PlayerScore += ScoreToAdd;
	OnScoreChanged.Broadcast(PlayerScore);
}

void AFPSGameMode::BeginPlay()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform NewTransform(FRotator(), FVector(0.f,0.f,-2000.f));
	HISMObject = GetWorld()->SpawnActor<AHISMSpawner>(AHISMSpawner::StaticClass(), NewTransform, SpawnParams);
	RetrieveDataFromJSON(); // remove this function call from bp
}
