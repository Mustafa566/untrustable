#include "../../Public/Components/sfMissingSceneComponent.h"
#include "../../Public/sfUnrealUtils.h"
#include "../../Public/sfActorUtil.h"
#include "../../Public/sfObjectMap.h"
#include "../../Public/Consts.h"
#include "../../Public/Translators/sfComponentTranslator.h"
#include "../../Public/SceneFusion.h"
#include "../../Public/sfMissingObjectManager.h"
#include "../../Public/sfReferenceFinder.h"
#include <Developer/HotReload/Public/IHotReload.h>

FString& UsfMissingSceneComponent::MissingClass()
{
    return ClassName;
}

TMap<FName, FsfByteArray>& UsfMissingSceneComponent::SerializedProperties()
{
    return SerializedData;
}

TMap<uint32, UObject*>& UsfMissingSceneComponent::References()
{
    return ReferenceMap;
}

bool UsfMissingSceneComponent::Reload()
{
    TSharedPtr<sfComponentTranslator> componentTranslatorPtr
        = SceneFusion::Get().GetTranslator<sfComponentTranslator>(sfType::Component);
    if (!componentTranslatorPtr.IsValid())
    {
        return true;
    }
    sfObject::SPtr objPtr = sfObjectMap::Remove(this);
    if (objPtr == nullptr)
    {
        // Create an sfObject for the component
        objPtr = componentTranslatorPtr->CreateObject(this);
        if (objPtr == nullptr)
        {
            return true;
        }
        // The component object needs to be a descendant of an actor object
        sfObject::SPtr actorObjPtr = sfObjectMap::GetOrCreateSFObject(GetOwner(), sfType::Actor);
        // Create a parent sfObject and attach to it.
        if (GetAttachParent() == nullptr || GetAttachParent()->GetOwner() != GetOwner())
        {
            actorObjPtr->AddChild(objPtr);
        }
        else
        {
            sfObject::SPtr parentPtr = sfObjectMap::GetOrCreateSFObject(GetAttachParent(), sfType::Component);
            parentPtr->AddChild(objPtr);
            actorObjPtr->AddChild(parentPtr);
        }
    }
    // Rename this component so the replacement can use its name
    sfUnrealUtils::Rename(this, GetName() + " (deleted)");
    // Create a new component of the correct class for this sfObject
    componentTranslatorPtr->OnCreate(objPtr, 0);
    if (IsSelected())
    {
        // Unselect this component and select the replacement component
        GEditor->SelectComponent(this, false, true);
        UActorComponent* componentPtr = sfObjectMap::Get<UActorComponent>(objPtr);
        if (componentPtr != nullptr)
        {
            GEditor->SelectComponent(componentPtr, true, true);
        }
    }
    if (SceneFusion::Service->Session() == nullptr)
    {
        USceneComponent* componentPtr = sfObjectMap::Get<USceneComponent>(objPtr);
        if (componentPtr != nullptr)
        {
            sfReferenceFinder::Get().AddReplacement(this, componentPtr);
        }
        sfObjectMap::Clear();
        // We don't destroy the component here; it will be destroyed after all references are updated by the reference
        // finder.
    }
    else
    {
        AActor* ownerPtr = GetOwner();
        DestroyComponent();
        sfActorUtil::Reselect(ownerPtr);
    }
    return true;
}

void UsfMissingSceneComponent::OnComponentCreated()
{
    Super::OnComponentCreated();
    if (!ClassName.IsEmpty())
    {
        sfMissingObjectManager::Get().AddStandIn(this);
    }
}

void UsfMissingSceneComponent::PostLoad()
{
    Super::PostLoad();
    if (!ClassName.IsEmpty())
    {
        sfMissingObjectManager::Get().AddStandIn(this);
    }
}

void UsfMissingSceneComponent::BeginDestroy()
{
    sfMissingObjectManager::Get().RemoveStandIn(this);
    Super::BeginDestroy();
}
