// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "LGUIToolsBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

USTRUCT(BlueprintType)
struct FLGUIRetValue

{
	GENERATED_USTRUCT_BODY()
		FLGUIRetValue() {};
	FLGUIRetValue(FString InName):Name(InName){};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) //
	TArray<UActorComponent*>  LGUIActorComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString SpriteName;
	
	UPROPERTY( EditAnywhere,BlueprintReadWrite)
	class UUIItem* UIItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUISprite* UISprite;
	
	void Reset()
	{
		Name.Empty();
		LGUIActorComponent.Reset();
	}
	void SetName(const FString& InName) {
		Name = InName;
	};
	void SetSpriteName(const FString& InName) {
		SpriteName = InName;
	};
};

USTRUCT(Blueprintable, BlueprintType)
struct FLGUIFindRetValue

{
	GENERATED_USTRUCT_BODY();
	FLGUIFindRetValue()
	{
	}
	FLGUIFindRetValue(TMap<AActor*, FLGUIRetValue> InLGUIFindRetValue) :
		LGUIFindRetValue(InLGUIFindRetValue)
		
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)// 
	TMap<AActor*, FLGUIRetValue> LGUIFindRetValue;
		
};

UCLASS()
class ULGUIToolsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "LGUITools sample test testing"), Category = "LGUIToolsTesting")
	static float LGUIToolsSampleFunction(float Param);

	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnLGuiReturnValue, FLGUIFindRetValue, IsDestroyActor);
	UFUNCTION(BlueprintCallable)
	static bool testLGUI(FAssetData AssetData, TSubclassOf<AUIBaseActor> InUGUIClass,TSubclassOf<UObject> FindSubcomponent, FOnLGuiReturnValue OnLGuiReturnValue);

	static UObject* GetAsset(FName ObjectPath);

	UFUNCTION(BlueprintCallable)
	static void ApplyPrefab();

	static UWorld* world;
	static class ULGUIPrefab* UIprefab;
	static AActor* LoadedRootActor;
};
