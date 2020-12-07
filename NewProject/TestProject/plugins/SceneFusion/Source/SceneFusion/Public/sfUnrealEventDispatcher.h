/*************************************************************************
 *
 * KINEMATICOUP CONFIDENTIAL
 * __________________
 *
 *  Copyright (2017-2020) KinematicSoup Technologies Incorporated
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of KinematicSoup Technologies Incorporated and its
 * suppliers, if any.  The intellectual and technical concepts contained
 * herein are proprietary to KinematicSoup Technologies Incorporated
 * and its suppliers and may be covered by Canadian and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from KinematicSoup Technologies Incorporated.
 */
#pragma once

#include <CoreMinimal.h>
#include <Engine.h>
#include <EditorSupportDelegates.h>
#include <Engine/WorldComposition.h>
#if ENGINE_MAJOR_VERSION <= 4 && ENGINE_MINOR_VERSION <= 23
#include <Layers/ILayers.h>
#else
#include <Layers/LayersSubsystem.h>
#endif
#include <Misc/HotReloadInterface.h>
#include <Editor/Transbuffer.h>
#include <IAssetRegistry.h>

/**
 * Listens for and dispatches Unreal events. Allows all events to be enabled or disabled. Register with this class
 * instead of directly against the Unreal events to ensure you do not respond to events that were triggered by Scene
 * Fusion.
 */
class sfUnrealEventDispatcher
{
public:
    /**
     * @return  sfUnrealEventDispatcher& singleton instance.
     */
    static sfUnrealEventDispatcher& Get();

    /**
     * Invoked when an actor is added to the level.
     */
    UEngine::FLevelActorAddedEvent OnActorAdd;

    /**
     * Invoked when an actor is deleted.
     */
    UEngine::FLevelActorDeletedEvent OnActorDelete;

    /**
     * Invoked when an actor is attached to another actor.
     */
    UEngine::FLevelActorAttachedEvent OnActorAttach;

    /**
     * Invoked when an actor is detached from another actor.
     */
    UEngine::FLevelActorDetachedEvent OnActorDetach;

    /**
     * Invoked when an actor's folder changes.
     */
    UEngine::FLevelActorFolderChangedEvent OnActorFolderChange;

    /**
     * Invoked when an actor's label changes.
     */
    FCoreDelegates::FOnActorLabelChanged OnActorLabelChange;

    /**
     * Invoked when a uproperty changes.
     */
    FCoreUObjectDelegates::FOnObjectPropertyChanged OnPropertyChange;

    /**
     * Invoked when an object is applied to an actor.
     */
    FEditorDelegates::FOnApplyObjectToActor OnApplyObjectToActor;

    /**
     * Invoked when a level is added to the world.
     */
    FWorldDelegates::FOnLevelChanged OnLevelAdd;

    /**
     * Invoked before the editor cleanses an object (such as when unloading a level).
     */
    FEditorSupportDelegates::FPrepareToCleanseEditorObject OnPrepareToCleanseEditorObject;

    /**
     * Invoked when the world composition enabled setting changes.
     */
    UWorldComposition::FWorldCompositionChangedEvent OnWorldCompositionChange;

    /**
     * Invoked when a package is marked dirty.
     */
    UPackage::FOnPackageMarkedDirty OnPackageDirty;

    /**
     * Invoked when the level is marked dirty.
     */
    FSimpleMulticastDelegate OnLevelDirty;

    /**
     * Invoked when one or more layers are modified.
     */
#if ENGINE_MAJOR_VERSION <= 4 && ENGINE_MINOR_VERSION <= 23
    ILayers::FOnLayersChanged OnLayersChange;
#else
    ULayersSubsystem::FOnLayersChanged OnLayersChange;
#endif

    DECLARE_MULTICAST_DELEGATE_TwoParams(OnLevelTransformChangeDelegate, ULevel*, const FTransform&);

    /**
     * Invoked when a level's transform changes.
     */
    OnLevelTransformChangeDelegate OnLevelTransformChange;

    /**
     * Invoked when a new asset is created.
     */
    IAssetRegistry::FAssetAddedEvent OnAddAsset;

    /**
     * Invoked when an asset is deleted.
     */
    IAssetRegistry::FAssetRemovedEvent OnRemoveAsset;

    /**
     * Invoked when the editor camera moves.
     */
    FEditorDelegates::FOnEditorCameraMoved OnCameraMove;

    /**
     * Invoked when an object is modified.
     */
    FCoreUObjectDelegates::FOnObjectModified OnModify;

    /**
     * Invokes after a hot reload.
     */
    IHotReloadInterface::FHotReloadEvent OnHotReload;

    /**
     * Invoked when the user starts moving objects.
     */
    UEditorEngine::FOnBeginTransformObject OnMoveStart;

    /**
     * Invoked when the user stops moving objects.
     */
    UEditorEngine::FOnEndTransformObject OnMoveEnd;

    /**
     * Invoked when an actor is moved.
     */
    UEditorEngine::FOnActorMovedEvent OnActorMove;

    /**
     * Invoked after an undo operation.
     */
    UTransBuffer::FOnTransactorUndo OnUndo;

    /**
     * Invoked after a redo operation.
     */
    UTransBuffer::FOnTransactorRedo OnRedo;

    /**
     * Invoked before an undo or redo operation.
     */
    UTransBuffer::FOnTransactorBeforeRedoUndo BeforeUndoRedo;

    /**
     * Constructor
     */
    sfUnrealEventDispatcher();

    /**
     * Destructor
     */
    ~sfUnrealEventDispatcher();

    /**
     * @return bool true if the event dispatcher is running.
     */
    bool Running() const;

    /**
     * @return bool true if events are enabled and the dispatcher is running.
     */
    bool Enabled() const;

    /**
     * Starts listening for Unreal events and enables events.
     */
    void Start();

    /**
     * Stops listening for Unreal events.
     */
    void Stop();

    /**
     * Enables events. Does nothing if the event dispatcher is not running.
     */
    void Enable();

    /**
     * Disables events.
     */
    void Disable();

private:
    bool m_enabled;
    bool m_running;
    FDelegateHandle m_onActorAddedHandle;
    FDelegateHandle m_onActorDeletedHandle;
    FDelegateHandle m_onActorAttachedHandle;
    FDelegateHandle m_onActorDetachedHandle;
    FDelegateHandle m_onFolderChangeHandle;
    FDelegateHandle m_onLabelChangeHandle;
    FDelegateHandle m_onPropertyChangeHandle;
    FDelegateHandle m_onApplyObjectHandle;
    FDelegateHandle m_onAddLevelHandle;
    FDelegateHandle m_onPrepareToCleanseEditorObjectHandle;
    FDelegateHandle m_onWorldCompositionChangeHandle;
    FDelegateHandle m_onPackageDirtyHandle;
    FDelegateHandle m_onLevelDirtyHandle;
    FDelegateHandle m_onLayersChangeHandle;
    FDelegateHandle m_onAddAssetHandle;
    FDelegateHandle m_onRemoveAssetHandle;
    FDelegateHandle m_onCameraMoveHandle;
    FDelegateHandle m_onModifyHandle;
    FDelegateHandle m_onHotReloadHandle;
    FDelegateHandle m_onMoveStartHandle;
    FDelegateHandle m_onMoveEndHandle;
    FDelegateHandle m_onActorMoveHandle;
    FDelegateHandle m_onUndoHandle;
    FDelegateHandle m_onRedoHandle;
    FDelegateHandle m_beforeUndoRedoHandle;

    TMap<ULevel*, FDelegateHandle> m_levelTransformChangeHandles;

#if ENGINE_MAJOR_VERSION <= 4 && ENGINE_MINOR_VERSION <= 23
    /**
     * Gets Unreal's OnLayersChange event.
     *
     * @return  FLayers::FOnLayersChanged&
     */
    ILayers::FOnLayersChanged& GetOnLayersChange();
#else
    /**
     * Gets Unreal's OnLayersChange event.
     *
     * @return  ULayersSubsystem::FOnLayersChanged&
     */
    ULayersSubsystem::FOnLayersChanged& GetOnLayersChange();
#endif
};

