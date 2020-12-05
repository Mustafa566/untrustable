#include "../Public/sfUnrealEventDispatcher.h"
#include "../Public/sfUndoManager.h"
#include <Engine/AssetManager.h>
#include <Developer/HotReload/Public/IHotReload.h>

sfUnrealEventDispatcher& sfUnrealEventDispatcher::Get()
{
    static sfUnrealEventDispatcher dispatcher;
    return dispatcher;
}

sfUnrealEventDispatcher::sfUnrealEventDispatcher() :
    m_enabled{ false }
{

}

sfUnrealEventDispatcher::~sfUnrealEventDispatcher()
{

}

bool sfUnrealEventDispatcher::Running() const
{
    return m_running;
}

bool sfUnrealEventDispatcher::Enabled() const
{
    return m_enabled;
}

void sfUnrealEventDispatcher::Enable()
{
    if (m_running)
    {
        m_enabled = true;
    }
}

void sfUnrealEventDispatcher::Disable()
{
    m_enabled = false;
}

void sfUnrealEventDispatcher::Start()
{
    if (m_running)
    {
        return;
    }
    m_running = true;
    m_enabled = true;
    m_onActorAddedHandle = GEngine->OnLevelActorAdded().AddLambda([this](AActor* actorPtr)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnActorAdd.Broadcast(actorPtr);
            m_enabled = true;
        }
    });
    m_onActorDeletedHandle = GEngine->OnLevelActorDeleted().AddLambda([this](AActor* actorPtr)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnActorDelete.Broadcast(actorPtr);
            m_enabled = true;
        }
    });
    m_onActorAttachedHandle = GEngine->OnLevelActorAttached().AddLambda(
        [this](AActor* actorPtr, const AActor* parentPtr)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnActorAttach.Broadcast(actorPtr, parentPtr);
            m_enabled = true;
        }
    });
    m_onActorDetachedHandle = GEngine->OnLevelActorDetached().AddLambda(
        [this](AActor* actorPtr, const AActor* parentPtr)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnActorDetach.Broadcast(actorPtr, parentPtr);
            m_enabled = true;
        }
    });
    m_onFolderChangeHandle = GEngine->OnLevelActorFolderChanged().AddLambda(
        [this](const AActor* actorPtr, FName oldFolder)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnActorFolderChange.Broadcast(actorPtr, oldFolder);
            m_enabled = true;
        }
    });
    m_onLabelChangeHandle = FCoreDelegates::OnActorLabelChanged.AddLambda([this](AActor* actorPtr)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnActorLabelChange.Broadcast(actorPtr);
            m_enabled = true;
        }
    });
    m_onPropertyChangeHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.AddLambda(
        [this](UObject* uobjPtr, FPropertyChangedEvent& ev)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnPropertyChange.Broadcast(uobjPtr, ev);
            m_enabled = true;
        }
    });
    m_onApplyObjectHandle = FEditorDelegates::OnApplyObjectToActor.AddLambda(
        [this](UObject* uobjPtr, AActor* actorPtr)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnApplyObjectToActor.Broadcast(uobjPtr, actorPtr);
            m_enabled = true;
        }
    });
    m_onAddLevelHandle = FWorldDelegates::LevelAddedToWorld.AddLambda(
        [this](ULevel* levelPtr, UWorld* worldPtr)
    {
        if (!levelPtr->IsPersistentLevel())
        {
            m_levelTransformChangeHandles.Add(levelPtr, levelPtr->OnApplyLevelTransform.AddLambda(
                [this, levelPtr](const FTransform& transform)
            {
                if (m_enabled)
                {
                    m_enabled = false;
                    OnLevelTransformChange.Broadcast(levelPtr, transform);
                    m_enabled = true;
                }
            }));
        }
        if (m_enabled)
        {
            m_enabled = false;
            OnLevelAdd.Broadcast(levelPtr, worldPtr);
            m_enabled = true;
        }
    });
    m_onPrepareToCleanseEditorObjectHandle = FEditorSupportDelegates::PrepareToCleanseEditorObject.AddLambda(
        [this](UObject* uobjPtr)
    {
        ULevel* levelPtr = Cast<ULevel>(uobjPtr);
        FDelegateHandle handle;
        if (levelPtr != nullptr && m_levelTransformChangeHandles.RemoveAndCopyValue(levelPtr, handle))
        {
            levelPtr->OnApplyLevelTransform.Remove(handle);
        }
        if (m_enabled)
        {
            m_enabled = false;
            OnPrepareToCleanseEditorObject.Broadcast(uobjPtr);
            m_enabled = true;
        }
    });
    m_onWorldCompositionChangeHandle = UWorldComposition::WorldCompositionChangedEvent.AddLambda(
        [this](UWorld* worldPtr)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnWorldCompositionChange.Broadcast(worldPtr);
            m_enabled = true;
        }
    });
    m_onPackageDirtyHandle = UPackage::PackageMarkedDirtyEvent.AddLambda(
        [this](UPackage* packagePtr, bool wasDirty)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnPackageDirty.Broadcast(packagePtr, wasDirty);
            m_enabled = true;
        }
    });
    m_onLevelDirtyHandle = ULevel::LevelDirtiedEvent.AddLambda([this]()
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnLevelDirty.Broadcast();
            m_enabled = true;
        }
    });
    m_onLayersChangeHandle = GetOnLayersChange().AddLambda([this](
        const ELayersAction::Type action,
        const TWeakObjectPtr<ULayer>& changedLayer,
        const FName& changedProperty)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnLayersChange.Broadcast(action, changedLayer, changedProperty);
            m_enabled = true;
        }
    });
    m_onAddAssetHandle = UAssetManager::Get().GetAssetRegistry().OnAssetAdded().AddLambda(
        [this](const FAssetData& assetData)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnAddAsset.Broadcast(assetData);
            m_enabled = true;
        }
    });
    m_onRemoveAssetHandle = UAssetManager::Get().GetAssetRegistry().OnAssetRemoved().AddLambda(
        [this](const FAssetData& assetData)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnRemoveAsset.Broadcast(assetData);
            m_enabled = true;
        }
    });
    m_onCameraMoveHandle = FEditorDelegates::OnEditorCameraMoved.AddLambda([this](
        const FVector& location,
        const FRotator& rotation,
        ELevelViewportType viewportType,
        int viewIndex)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnCameraMove.Broadcast(location, rotation, viewportType, viewIndex);
            m_enabled = true;
        }
    });
    m_onModifyHandle = FCoreUObjectDelegates::OnObjectModified.AddLambda(
        [this](UObject* uobjPtr)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnModify.Broadcast(uobjPtr);
            m_enabled = true;
        }
    });
    m_onHotReloadHandle = IHotReloadModule::Get().OnHotReload().AddLambda(
        [this](bool automatic)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnHotReload.Broadcast(automatic);
            m_enabled = true;
        }
    });
    m_onMoveStartHandle = GEditor->OnBeginObjectMovement().AddLambda(
        [this](UObject& uobj)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnMoveStart.Broadcast(uobj);
            m_enabled = true;
        }
    });
    m_onMoveEndHandle = GEditor->OnEndObjectMovement().AddLambda(
        [this](UObject& uobj)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnMoveEnd.Broadcast(uobj);
            m_enabled = true;
        }
    });
    m_onActorMoveHandle = GEditor->OnActorMoved().AddLambda(
        [this](AActor* actorPtr)
    {
        if (m_enabled)
        {
            m_enabled = false;
            OnActorMove.Broadcast(actorPtr);
            m_enabled = true;
        }
    });

    UTransBuffer* undoBufferPtr = Cast<UTransBuffer>(GEditor->Trans);
    if (undoBufferPtr != nullptr)
    {
        m_onUndoHandle = undoBufferPtr->OnUndo().AddLambda(
            [this](sfUndoManager::TransactionContext context, bool success)
        {
            if (m_enabled)
            {
                m_enabled = false;
                OnUndo.Broadcast(context, success);
                m_enabled = true;
            }
        });
        m_onRedoHandle = undoBufferPtr->OnRedo().AddLambda(
            [this](sfUndoManager::TransactionContext context, bool success)
        {
            if (m_enabled)
            {
                m_enabled = false;
                OnRedo.Broadcast(context, success);
                m_enabled = true;
            }
        });
        m_beforeUndoRedoHandle = undoBufferPtr->OnBeforeRedoUndo().AddLambda(
            [this](sfUndoManager::TransactionContext context)
        {
            if (m_enabled)
            {
                m_enabled = false;
                BeforeUndoRedo.Broadcast(context);
                m_enabled = true;
            }
        });
    }

    for (FConstLevelIterator iter = GEditor->GetEditorWorldContext().World()->GetLevelIterator(); iter; ++iter)
    {
        ULevel* levelPtr = *iter;
        if (!levelPtr->IsPersistentLevel())
        {
            m_levelTransformChangeHandles.Add(levelPtr, levelPtr->OnApplyLevelTransform.AddLambda(
                [this, levelPtr](const FTransform& transform)
            {
                if (m_enabled)
                {
                    m_enabled = false;
                    OnLevelTransformChange.Broadcast(levelPtr, transform);
                    m_enabled = true;
                }
            }));
        }
    }
}

void sfUnrealEventDispatcher::Stop()
{
    if (!m_running)
    {
        return;
    }
    m_running = false;
    m_enabled = false;
    GEngine->OnLevelActorAdded().Remove(m_onActorAddedHandle);
    GEngine->OnLevelActorDeleted().Remove(m_onActorDeletedHandle);
    GEngine->OnLevelActorAttached().Remove(m_onActorAttachedHandle);
    GEngine->OnLevelActorDetached().Remove(m_onActorDetachedHandle);
    GEngine->OnLevelActorFolderChanged().Remove(m_onFolderChangeHandle);
    FCoreDelegates::OnActorLabelChanged.Remove(m_onLabelChangeHandle);
    FCoreUObjectDelegates::OnObjectPropertyChanged.Remove(m_onPropertyChangeHandle);
    FEditorDelegates::OnApplyObjectToActor.Remove(m_onApplyObjectHandle);
    FWorldDelegates::LevelAddedToWorld.Remove(m_onAddLevelHandle);
    FEditorSupportDelegates::PrepareToCleanseEditorObject.Remove(m_onPrepareToCleanseEditorObjectHandle);
    UWorldComposition::WorldCompositionChangedEvent.Remove(m_onWorldCompositionChangeHandle);
    UPackage::PackageMarkedDirtyEvent.Remove(m_onPackageDirtyHandle);
    ULevel::LevelDirtiedEvent.Remove(m_onLevelDirtyHandle);
    GetOnLayersChange().Remove(m_onLayersChangeHandle);
    UAssetManager::Get().GetAssetRegistry().OnAssetAdded().Remove(m_onAddAssetHandle);
    UAssetManager::Get().GetAssetRegistry().OnAssetRemoved().Remove(m_onRemoveAssetHandle);
    FEditorDelegates::OnEditorCameraMoved.Remove(m_onCameraMoveHandle);
    FCoreUObjectDelegates::OnObjectModified.Remove(m_onModifyHandle);
    IHotReloadModule::Get().OnHotReload().Remove(m_onHotReloadHandle);
    GEditor->OnBeginObjectMovement().Remove(m_onMoveStartHandle);
    GEditor->OnEndObjectMovement().Remove(m_onMoveEndHandle);
    GEditor->OnActorMoved().Remove(m_onActorMoveHandle);

    UTransBuffer* undoBufferPtr = Cast<UTransBuffer>(GEditor->Trans);
    if (undoBufferPtr != nullptr)
    {
        undoBufferPtr->OnUndo().Remove(m_onUndoHandle);
        undoBufferPtr->OnRedo().Remove(m_onRedoHandle);
        undoBufferPtr->OnBeforeRedoUndo().Remove(m_beforeUndoRedoHandle);
    }

    for (auto iter = m_levelTransformChangeHandles.CreateIterator(); iter; ++iter)
    {
        iter.Key()->OnApplyLevelTransform.Remove(iter.Value());
    }
    m_levelTransformChangeHandles.Empty();
}

#if ENGINE_MAJOR_VERSION <= 4 && ENGINE_MINOR_VERSION <= 23
ILayers::FOnLayersChanged& sfUnrealEventDispatcher::GetOnLayersChange()
{
    return GEditor->Layers->OnLayersChanged();
}
#else
ULayersSubsystem::FOnLayersChanged& sfUnrealEventDispatcher::GetOnLayersChange()
{
    return GEditor->GetEditorSubsystem<ULayersSubsystem>()->OnLayersChanged();
}
#endif