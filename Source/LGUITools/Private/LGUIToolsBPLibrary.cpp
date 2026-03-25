// Copyright Epic Games, Inc. All Rights Reserved.

#include "LGUIToolsBPLibrary.h"

#include "ActorSerializer.h"
#include "Editor.h"
#include "LGUIPrefabActor.h"
#include "LGUITools.h"
#include "UIBaseActor.h"
#include "UISpriteActor.h"
#include "PrefabSystem/LGUIPrefab.h"

UWorld* ULGUIToolsBPLibrary::world = nullptr;
ULGUIPrefab* ULGUIToolsBPLibrary::UIprefab= nullptr;
AActor* ULGUIToolsBPLibrary::LoadedRootActor= nullptr;
ULGUIToolsBPLibrary::ULGUIToolsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float ULGUIToolsBPLibrary::LGUIToolsSampleFunction(float Param)
{
	return -1;
}

bool ULGUIToolsBPLibrary::testLGUI(FAssetData AssetData, TSubclassOf<AUIBaseActor> InUGUIClass,
	TSubclassOf<UObject> FindSubcomponent, FOnLGuiReturnValue OnLGuiReturnValue)
{
	FName objectPath;
	objectPath = AssetData.ObjectPath;
	// UPrefabAsset* prefabasset = Cast<UPrefabAsset>(GetAsset(objectPath));
	UIprefab = Cast<ULGUIPrefab>(GetAsset(objectPath));
	
	for (auto& cont : GEditor->GetWorldContexts())
	{
		if (cont.WorldType == EWorldType::Editor)
		{
			world = cont.World();
			break;
		}

	}
	TArray<AActor*> AllLoadedActorArray;
	AActor* UIrootactor = LoadedRootActor =  LGUIPrefabSystem::ActorSerializer::LoadPrefabForEdit(world,UIprefab,nullptr,AllLoadedActorArray);
	FLGUIFindRetValue LGUIFindRetValue ;
	for(auto& actor:AllLoadedActorArray)
	{
		if(InUGUIClass == actor->GetClass())
		{
			FLGUIRetValue LGUIRetValue ;
			
			for(auto component:actor->GetComponents())
			{
				if(FindSubcomponent == component->GetClass())
				{
					
				}
				LGUIRetValue.LGUIActorComponent.Emplace(component);
			}
			LGUIRetValue.Name = actor->GetActorLabel();
			
			AUIBaseActor* UIbaseActor = Cast<AUIBaseActor>(actor);
			if(IsValid(UIbaseActor))
			{
				LGUIRetValue.UIItem = UIbaseActor->GetUIItem();
			}else if(AUISpriteActor* UISpriteActor = Cast<AUISpriteActor>(actor))
			{
				LGUIRetValue.UISprite = UISpriteActor->GetUISprite();
			}
			
			LGUIFindRetValue.LGUIFindRetValue.Add(actor,LGUIRetValue);
		}
	}

	bool isdesactor = OnLGuiReturnValue.Execute(LGUIFindRetValue);
	if(isdesactor)
	{
		UIrootactor->K2_DestroyActor();
	}
	return false;
}

UObject* ULGUIToolsBPLibrary::GetAsset(FName ObjectPath)
{
	FString String = ObjectPath.ToString();
	if(String.Find(".") == -1)
	{
		String = FString::Printf(TEXT("%s.%s"),*String,*FPackageName::GetLongPackageAssetName(String));
	}
	UObject* Asset = FindObject<UObject>(nullptr, *String);
	if ( Asset == nullptr)
	{
		Asset = LoadObject<UObject>(nullptr, *String);
		
	}
	return Asset;
}

void ULGUIToolsBPLibrary::ApplyPrefab()
{
	auto prefabActor = world->SpawnActor<ALGUIPrefabActor>();
	if (prefabActor != nullptr)
	{
		prefabActor->GetPrefabComponent()->SetPrefabAsset(UIprefab);
		prefabActor->GetPrefabComponent()->LoadedRootActor = LoadedRootActor;
		prefabActor->GetPrefabComponent()->SavePrefab();
	}

	prefabActor->K2_DestroyActor();
}
