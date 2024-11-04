// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "HttpModule.h"
#include "Json.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "BoxToSpawn.h"

#include "TESTSpawnBox.h"
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

									//DEBUG::PrintString(FString::Printf(TEXT("Scale : %s"), *Scale.ToString()),12.f);
									//DrawDebugSphere(GetWorld(), Location, 40.f, 12, FColor::Red, true, -1.f, (uint8)0U, 1.f);
									ABoxToSpawn* SpawnedBox = GetWorld()->SpawnActor<ABoxToSpawn>(ABoxToSpawn::StaticClass()
										, Location, Rotation, SpawnParams);
									if (SpawnedBox)
									{
										SpawnedBox->SetActorScale3D(Scale);
										SpawnedBox->GivenHealth = It.Health;
										SpawnedBox->ScoreToAward = It.Score;
										SpawnedBox->BoxName = It.Name;
										SpawnedBox->ApplyMaterialToBox(It.ColorR, It.ColorG, It.ColorB);
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

	Request->ProcessRequest();
}