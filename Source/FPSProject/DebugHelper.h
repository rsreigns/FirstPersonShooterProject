#pragma once
namespace DEBUG
{
	static void PrintString(FString Msg, float Duration = 2.f, FColor color = FColor::Green)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, Duration, color, Msg);
		}
	}
}