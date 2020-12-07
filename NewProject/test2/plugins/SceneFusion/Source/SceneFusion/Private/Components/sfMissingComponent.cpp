#include "../../Public/Components/sfMissingComponent.h"
#include "../../Public/sfUnrealUtils.h"
#include "../../Public/sfActorUtil.h"
#include "../../Public/sfObjectMap.h"
#include "../../Public/Consts.h"
#include "../../Public/Translators/sfComponentTranslator.h"
#include "../../Public/SceneFusion.h"
#include "../../Public/sfMissingObjectManager.h"
#include "../../Public/sfReferenceFinder.h"
#include <Developer/HotReload/Public/IHotReload.h>

FString& UsfMissingComponent::MissingClass()
{
    return ClassName;
}

TMap<FName, FsfByteArray>& UsfMissingComponent::SerializedProperties()
{
    return SerializedData;
}

TMap<uint32, UObject*>& UsfMissingComponent::References()
{
    return ReferenceMap;
}

bool UsfMissingComponent::Reload()
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
        // Create a parent sfObject and attach to it.
        sfObject::SPtr parentPtr = sfObjectMap::GetOrCreateSFObject(GetOwner(), sfType::Actor);
        parentPtr->AddChild(objPtr);
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
        UActorComponent* componentPtr = sfObjectMap::Get<UActorComponent>(objPtr);
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

void UsfMissingComponent::OnComponentCreated()
{
    Super::OnComponentCreated();
    if (!ClassName.IsEmpty())
    {
        sfMissingObjectManager::Get().AddStandIn(this);
    }
}

void UsfMissingComponent::PostLoad()
{
    Super::PostLoad();
    if (!ClassName.IsEmpty())
    {
        sfMissingObjectManager::Get().AddStandIn(this);
    }
}

void UsfMissingComponent::BeginDestroy()
{
    sfMissingObjectManager::Get().RemoveStandIn(this);
    Super::BeginDestroy();
}

