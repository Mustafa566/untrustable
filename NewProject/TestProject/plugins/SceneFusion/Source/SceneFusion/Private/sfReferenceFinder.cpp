#include "../Public/sfReferenceFinder.h"
#include "../Public/sfPropertyManager.h"
#include "../Public/SceneFusion.h"
#include "../Public/sfActorUtil.h"
#include "../Public/Translators/sfActorTranslator.h"
#include <LevelUtils.h>

sfReferenceFinder& sfReferenceFinder::Get()
{
    static sfReferenceFinder instance;
    return instance;
}

sfReferenceFinder::sfReferenceFinder() :
    m_updateTimer{ 0.0f }
{
    m_typeHandlers[sfUnrealUtils::GetClassTypeHash(UnrealObjectProperty::StaticClass())] = TypeHandler(
        [this](const sfUPropertyInstance& upropInstance, UObject* uobjPtr)
    {
        UnrealObjectProperty* tPtr = CastUnrealProperty<UnrealObjectProperty>(upropInstance.Property());
        return tPtr->GetObjectPropertyValue(upropInstance.Data());
    },
        [this](const sfUPropertyInstance& upropInstance, UObject* referencePtr)
    {
        UnrealObjectProperty* tPtr = CastUnrealProperty<UnrealObjectProperty>(upropInstance.Property());
        tPtr->SetObjectPropertyValue(upropInstance.Data(), referencePtr);
    });

    m_typeHandlers[sfUnrealUtils::GetClassTypeHash(UnrealSoftObjectProperty::StaticClass())] = TypeHandler(
        [this](const sfUPropertyInstance& upropInstance, UObject* uobjPtr)
    {
        FSoftObjectPtr& softObjPtr = *(FSoftObjectPtr*)upropInstance.Data();
        return softObjPtr.Get();
    },
        [this](const sfUPropertyInstance& upropInstance, UObject* referencePtr)
    {
        FSoftObjectPtr& softObjPtr = *(FSoftObjectPtr*)upropInstance.Data();
        softObjPtr = FSoftObjectPath(referencePtr);
    });

    m_typeHandlers[sfUnrealUtils::GetClassTypeHash(UnrealLazyObjectProperty::StaticClass())] = TypeHandler(
        [this](const sfUPropertyInstance& upropInstance, UObject* uobjPtr)
    {
        FLazyObjectPtr& lazyObjPtr = *(FLazyObjectPtr*)upropInstance.Data();
        return lazyObjPtr.Get();
    },
        [this](const sfUPropertyInstance& upropInstance, UObject* referencePtr)
    {
        FLazyObjectPtr& lazyObjPtr = *(FLazyObjectPtr*)upropInstance.Data();
        lazyObjPtr = referencePtr;
    });

    m_typeHandlers[sfUnrealUtils::GetClassTypeHash(UnrealWeakObjectProperty::StaticClass())] = TypeHandler(
        [this](const sfUPropertyInstance& upropInstance, UObject* uobjPtr)
    {
        FWeakObjectPtr& weakObjPtr = *(FWeakObjectPtr*)upropInstance.Data();
        return weakObjPtr.Get();
    },
        [this](const sfUPropertyInstance& upropInstance, UObject* referencePtr)
    {
        FWeakObjectPtr& weakObjPtr = *(FWeakObjectPtr*)upropInstance.Data();
        weakObjPtr = referencePtr;
    });

    // We update sup-property references in the getter for container properties.
    m_typeHandlers[sfUnrealUtils::GetClassTypeHash(UnrealArrayProperty::StaticClass())] = TypeHandler(
        [this](const sfUPropertyInstance& upropInstance, UObject* uobjPtr)
    {
        UnrealArrayProperty* tPtr = CastUnrealProperty<UnrealArrayProperty>(upropInstance.Property());
        auto iter = m_typeHandlers.find(sfUnrealUtils::GetClassTypeHash(tPtr->Inner->GetClass()));
        if (iter == m_typeHandlers.end())
        {
            return nullptr;
        }
        FScriptArrayHelper array(tPtr, upropInstance.Data());
        for (int i = 0; i < array.Num(); i++)
        {
            sfUPropertyInstance subpropInstance = sfUPropertyInstance(tPtr->Inner, (void*)array.GetRawPtr(i));
            UpdateProperty(subpropInstance, uobjPtr);
        }
        return nullptr;
    });

    m_typeHandlers[sfUnrealUtils::GetClassTypeHash(UnrealMapProperty::StaticClass())] = TypeHandler(
        [this](const sfUPropertyInstance& upropInstance, UObject* uobjPtr)
    {
        UnrealMapProperty* tPtr = CastUnrealProperty<UnrealMapProperty>(upropInstance.Property());
        auto keyIter = m_typeHandlers.find(sfUnrealUtils::GetClassTypeHash(tPtr->KeyProp->GetClass()));
        auto valueIter = m_typeHandlers.find(sfUnrealUtils::GetClassTypeHash(tPtr->ValueProp->GetClass()));
        if (keyIter == m_typeHandlers.end() && valueIter == m_typeHandlers.end())
        {
            return nullptr;
        }
        FScriptMapHelper map(tPtr, upropInstance.Data());
        for (int i = 0; i < map.GetMaxIndex(); i++)
        {
            if (map.IsValidIndex(i))
            {
                sfUPropertyInstance subpropInstance = sfUPropertyInstance(tPtr->KeyProp, (void*)map.GetKeyPtr(i));
                UpdateProperty(subpropInstance, uobjPtr);
                subpropInstance = sfUPropertyInstance(tPtr->ValueProp, (void*)map.GetValuePtr(i));
                UpdateProperty(subpropInstance, uobjPtr);
            }
        }
        return nullptr;
    });

    m_typeHandlers[sfUnrealUtils::GetClassTypeHash(UnrealSetProperty::StaticClass())] = TypeHandler(
        [this](const sfUPropertyInstance& upropInstance, UObject* uobjPtr)
    {
        UnrealSetProperty* tPtr = CastUnrealProperty<UnrealSetProperty>(upropInstance.Property());
        auto iter = m_typeHandlers.find(sfUnrealUtils::GetClassTypeHash(tPtr->ElementProp->GetClass()));
        if (iter == m_typeHandlers.end())
        {
            return nullptr;
        }
        FScriptSetHelper set(tPtr, upropInstance.Data());
        for (int i = 0; i < set.GetMaxIndex(); i++)
        {
            if (set.IsValidIndex(i))
            {
                sfUPropertyInstance subpropInstance = sfUPropertyInstance(tPtr->ElementProp, (void*)set.GetElementPtr(i));
                UpdateProperty(subpropInstance, uobjPtr);
            }
        }
        return nullptr;
    });

    m_typeHandlers[sfUnrealUtils::GetClassTypeHash(UnrealStructProperty::StaticClass())] = TypeHandler(
        [this](const sfUPropertyInstance& upropInstance, UObject* uobjPtr)
    {
        UnrealStructProperty* tPtr = CastUnrealProperty<UnrealStructProperty>(upropInstance.Property());
        UnrealProperty* subPropPtr = tPtr->Struct->PropertyLink;
        while (subPropPtr)
        {
            auto iter = m_typeHandlers.find(sfUnrealUtils::GetClassTypeHash(subPropPtr->GetClass()));
            if (iter != m_typeHandlers.end())
            {
                for (int i = 0; i < subPropPtr->ArrayDim; i++)
                {
                    sfUPropertyInstance subpropInstance = sfUPropertyInstance(subPropPtr,
                        subPropPtr->ContainerPtrToValuePtr<void>(upropInstance.Data(), i));
                    UpdateProperty(subpropInstance, uobjPtr);
                }
            }
            subPropPtr = subPropPtr->PropertyLinkNext;
        }
        return nullptr;
    });
}

void sfReferenceFinder::AddReplacement(UObject* oldPtr, UObject* newPtr)
{
    m_replacementMap.Add(oldPtr, newPtr);
}

void sfReferenceFinder::DelayedUpdateReferences()
{
    if (m_updateTimer <= 0.0f)
    {
        m_tickHandle = FTicker::GetCoreTicker().AddTicker(
            FTickerDelegate::CreateRaw(this, &sfReferenceFinder::Tick), 1.0f / 60.0f);
    }
    // Wait .1 seconds to see if more assets are created that we can swap in all at once.
    m_updateTimer = 0.1f;
}

bool sfReferenceFinder::Tick(float deltaTime)
{
    m_updateTimer -= deltaTime;
    if (m_updateTimer > 0.0f)
    {
        return true;
    }
    PreUpdateReferences.Broadcast();
    UpdateAllReferences();
    return false;
}

void sfReferenceFinder::UpdateAllReferences()
{
    if (m_replacementMap.Num() == 0)
    {
        return;
    } 
    for (FConstLevelIterator iter = GEditor->GetEditorWorldContext().World()->GetLevelIterator(); iter; ++iter)
    {
        UpdateReferencesInLevel(*iter);
    }
    TSet<AActor*> toReselect;
    TSharedPtr<sfActorTranslator> translatorPtr = SceneFusion::Get().GetTranslator<sfActorTranslator>(sfType::Actor);
    for (auto iter : m_replacementMap)
    {
        AActor* actorPtr = Cast<AActor>(iter.Key);
        if (actorPtr != nullptr)
        {
            translatorPtr->DestroyActor(actorPtr);
            continue;
        }
        UActorComponent* componentPtr = Cast<UActorComponent>(iter.Key);
        if (componentPtr != nullptr)
        {
            toReselect.Add(componentPtr->GetOwner());
            componentPtr->DestroyComponent();
        }
    }
    m_replacementMap.Empty();

    // Reselect selected actors with replaced components to refresh the details panel
    for (AActor* actorPtr : toReselect)
    {
        sfActorUtil::Reselect(actorPtr);
    }

    for (AActor* actorPtr : m_changedActors)
    {
        sfPropertyManager::Get().BroadcastChangeEvent(actorPtr, nullptr);
    }
    m_changedActors.Empty();
    SceneFusion::RedrawActiveViewport();
}

void sfReferenceFinder::UpdateReferencesInLevel(ULevel* levelPtr)
{
    if (levelPtr->IsPersistentLevel())
    {
        AWorldSettings* worldSettingsPtr = GEditor->GetEditorWorldContext().World()->GetWorldSettings();
        UpdateReferences(worldSettingsPtr);
    }
    else
    {
        ULevelStreaming* streamingLevelPtr = FLevelUtils::FindStreamingLevel(levelPtr);
        if (streamingLevelPtr != nullptr)
        {
            UpdateReferences(streamingLevelPtr);
        }
    }
    for (AActor* actorPtr : levelPtr->Actors)
    {
        UpdateReferencesInActor(actorPtr);
    }
}

void sfReferenceFinder::UpdateReferencesInActor(AActor* actorPtr)
{
    if (m_replacementMap.Contains(actorPtr))
    {
        return;
    }
    TSharedPtr<sfActorTranslator> translatorPtr = SceneFusion::Get().GetTranslator<sfActorTranslator>(sfType::Actor);
    if (!translatorPtr->IsSyncable(actorPtr))
    {
        return;
    }
    UpdateReferences(actorPtr);
    for (UActorComponent* componentPtr : actorPtr->GetComponents())
    {
        UpdateReferences(componentPtr);
    }
}

void sfReferenceFinder::UpdateReferences(UObject* uobjPtr)
{
    bool alreadyAdded = false;
    m_exploredObjects.Add(uobjPtr, &alreadyAdded);
    if (alreadyAdded)
    {
        // Don't explore this object if we've already explored it to prevent recursion
        return;
    }
    for (TFieldIterator<UnrealProperty> iter(uobjPtr->GetClass()); iter; ++iter)
    {
        if (!sfPropertyManager::Get().IsSyncable(uobjPtr, *iter))
        {
            continue;
        }
        UnrealProperty* upropPtr = *iter;
        auto handlerIter = m_typeHandlers.find(sfUnrealUtils::GetClassTypeHash(upropPtr->GetClass()));
        if (handlerIter == m_typeHandlers.end())
        {
            continue;
        }
        for (int i = 0; i < upropPtr->ArrayDim; i++)
        {
            sfUPropertyInstance upropInstance = sfUPropertyInstance(upropPtr,
                upropPtr->ContainerPtrToValuePtr<void>(uobjPtr, i));
            UpdateProperty(upropInstance, uobjPtr);
        }
    } 
}

void sfReferenceFinder::UpdateProperty(sfUPropertyInstance& upropInstance, UObject* uobjPtr)
{
    auto handlerIter = m_typeHandlers.find(sfUnrealUtils::GetClassTypeHash(upropInstance.Property()->GetClass()));
    if (handlerIter == m_typeHandlers.end())
    {
        return;
    }
    UObject* referencePtr = handlerIter->second.Get(upropInstance, uobjPtr);
    if (referencePtr == nullptr || referencePtr->IsPendingKill())
    {
        return;
    }
    UObject* replacementPtr = m_replacementMap.FindRef(referencePtr);
    if (replacementPtr != nullptr)
    {
        handlerIter->second.Set(upropInstance, replacementPtr);
        referencePtr = replacementPtr;
        sfPropertyManager::Get().BroadcastChangeEvent(uobjPtr, upropInstance.Property());
        AActor* actorPtr = Cast<AActor>(uobjPtr->GetOuter());
        if (actorPtr != nullptr)
        {
            m_changedActors.Add(actorPtr);
        }
    }
    if (referencePtr->GetTypedOuter<ULevel>() != nullptr)
    {
        UpdateReferences(referencePtr);
    }
}