#include "../../Public/Actors/sfMissingActor.h"
#include "../../Public/sfUnrealUtils.h"
#include "../../Public/sfObjectMap.h"
#include "../../Public/Consts.h"
#include "../../Public/Translators/sfActorTranslator.h"
#include "../../Public/SceneFusion.h"
#include "../../Public/sfReferenceFinder.h"
#include "../../Public/sfMissingObjectManager.h"
#include <Developer/HotReload/Public/IHotReload.h>

FString& AsfMissingActor::MissingClass()
{
    return ClassName;
}

TMap<FName, FsfByteArray>& AsfMissingActor::SerializedProperties()
{
    return SerializedData;
}

TMap<uint32, UObject*>& AsfMissingActor::References()
{
    return ReferenceMap;
}

bool AsfMissingActor::Reload()
{
    TSharedPtr<sfActorTranslator> actorTranslatorPtr = SceneFusion::Get().GetTranslator<sfActorTranslator>(
        sfType::Actor);
    if (!actorTranslatorPtr.IsValid())
    {
        return true;
    }
    sfObject::SPtr objPtr = sfObjectMap::Remove(this);
    if (objPtr == nullptr)
    {
        // The actor translator needs an sfObject for the parent level, so create one.
        sfObject::SPtr levelObjPtr = sfObjectMap::GetOrCreateSFObject(GetLevel(), sfType::Level);
        // Create an sfObject for the actor
        objPtr = actorTranslatorPtr->CreateObject(this);
        if (objPtr == nullptr)
        {
            return true;
        }
        // Create a parent sfObject and attach to it.
        USceneComponent* rootPtr = GetRootComponent();
        if (rootPtr == nullptr || rootPtr->GetAttachParent() == nullptr)
        {
            levelObjPtr->AddChild(objPtr);
        }
        else
        {
            sfObject::SPtr parentPtr = sfObjectMap::GetOrCreateSFObject(rootPtr->GetAttachParent(), sfType::Component);
            parentPtr->AddChild(objPtr);
            // The parent needs to be a descendant of the level object. It doesn't matter if the parent actor has
            // another parent since we are not recreating the parent actor.
            levelObjPtr->AddChild(parentPtr);
        }
    }
    // Remove child component objects from the sfObjectMap
    objPtr->ForEachDescendant([](sfObject::SPtr childPtr)
    {
        if (childPtr->Type() != sfType::Component)
        {
            return false;
        }
        sfObjectMap::Remove(childPtr);
        return true;
    });
    // Rename this actor so the replacement can use its name
    sfUnrealUtils::Rename(this, GetName() + " (deleted)");
    // Create a new actor of the correct class for this sfObject
    actorTranslatorPtr->OnCreate(objPtr, 0);
    if (IsSelected())
    {
        // Unselect this actor and select the replacement actor
        GEditor->SelectActor(this, false, true);
        AActor* actorPtr = sfObjectMap::Get<AActor>(objPtr);
        if (actorPtr != nullptr)
        {
            GEditor->SelectActor(actorPtr, true, true);
        }
    }
    if (SceneFusion::Service->Session() == nullptr)
    {
        AActor* actorPtr = sfObjectMap::Get<AActor>(objPtr);
        if (actorPtr != nullptr)
        {
            sfReferenceFinder::Get().AddReplacement(this, actorPtr);
        }
        sfObjectMap::Clear();
        // We don't destroy the actor here; it will be destroyed after all references are updated by the reference
        // finder.
    }
    else
    {
        actorTranslatorPtr->DestroyActor(this);
    }
    return true;
}

void AsfMissingActor::PostLoad()
{
    Super::PostLoad();
    if (!ClassName.IsEmpty())
    {
        sfMissingObjectManager::Get().AddStandIn(this);
    }
}

void AsfMissingActor::PostDuplicate(bool bDuplicateForPie)
{
    Super::PostDuplicate(bDuplicateForPie);
    if (!ClassName.IsEmpty() && !bDuplicateForPie)
    {
        sfMissingObjectManager::Get().AddStandIn(this);
    }
}

void AsfMissingActor::BeginDestroy()
{
    sfMissingObjectManager::Get().RemoveStandIn(this);
    Super::BeginDestroy();
}
