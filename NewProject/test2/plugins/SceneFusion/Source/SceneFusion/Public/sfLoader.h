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

#include "sfIStandInGenerator.h"
#include <sfProperty.h>
#include <sfObject.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>
#include <CoreMinimal.h>
#include <Framework/Application/IInputProcessor.h>
#include <Framework/Application/SlateApplication.h>
#include <Engine/AssetManager.h>

using namespace KS;
using namespace KS::SceneFusion2;

/**
 * Utility for loading assets from memory, or from disc when the user is idle. Loading from disc may trigger assets to
 * be baked which can block the main thread (and cannot be done from another thread) for several seconds and interrupt
 * the user, so we wait till the user is idle.
 */
class sfLoader : public IInputProcessor
{
public:
    /**
     * Stand-in generator.
     *
     * @param   const FString& path to missing asset to generate stand-in for.
     * @param   UObject* stand-in to generate data for.
     */
    typedef std::function<void(const FString&, UObject*)> StandInGenerator;

    /**
     * Delegate for on create asset.
     *
     * @param   UObject* asset that was created.
     */
    DECLARE_MULTICAST_DELEGATE_OneParam(OnCreateMissingAssetDelegate, UObject*);

    /**
     * Delegate for on missing asset.
     *
     * @param   const FString& path to missing asset.
     */
    DECLARE_MULTICAST_DELEGATE_OneParam(OnMissingAssetDelegate, const FString&);

    /**
     * Delegate for on removed missing asset.
     *
     * @param   const FString& path to asset that was found or no longer referenced.
     */
    DECLARE_MULTICAST_DELEGATE_OneParam(OnRemoveMissingAssetDelegate, const FString&);

    /**
     * Invoked when the loader creates a missing asset.
     */
    OnCreateMissingAssetDelegate OnCreateMissingAsset;

    /**
     * Invoked when an asset cannot be found.
     */
    OnMissingAssetDelegate OnMissingAsset;

    /**
     * Invoked when a missing asset is found or all levels referencing the asset were unloaded.
     */
    OnRemoveMissingAssetDelegate OnRemoveMissingAsset;

    /**
     * @return  sfLoader& singleton instance.
     */
    static sfLoader& Get();

    /**
     * Constructor
     */
    sfLoader();

    /**
     * Destructor
     */
    virtual ~sfLoader();

    /**
     * Initialization. Adds event handlers that listen for new assets.
     */
    void Initialize();

    /**
     * Starts monitoring user activity and loads assets when the user becomes idle.
     */
    void Start();

    /**
     * Stops monitoring user activity and stops loading.
     */
    void Stop();

    /**
     * Registers a generator to generate data for stand-ins of a given class.
     *
     * @param   UClass* classPtr to register generator for.
     * @param   TSharedPtr<sfIStandInGenerator> generatorPtr to register.
     */
    void RegisterStandInGenerator(UClass* classPtr, TSharedPtr<sfIStandInGenerator> generatorPtr);

    /**
     * Registers a generator to generate data for stand-ins of a given class.
     *
     * @param   UClass* classPtr to register generator for.
     * @param   StandInGenerator generator to register.
     */
    void RegisterStandInGenerator(UClass* classPtr, StandInGenerator generator);

    /**
     * Checks if the user is idle.
     *
     * @return  bool true if the user is idle.
     */
    bool IsUserIdle();

    /**
     * Loads the asset for a property when the user becomes idle.
     *
     * @param   sfProperty::SPtr propPtr to load asset for.
     */
    void LoadWhenIdle(sfProperty::SPtr propPtr);

    /**
     * Loads delayed assets referenced by an object or its component children.
     *
     * @param   sfObject::SPtr objPtr to load assets for.
     */
    void LoadAssetsFor(sfObject::SPtr objPtr);

    /**
     * Loads an asset. If the asset could not be found, creates a stand-in to represent the asset.
     *
     * @param   const FString& path of asset to load.
     * @param   const FString& className of asset to load.
     * @param   sfObject::SPtr contextPtr - sfObject containing the property we are loading the reference for. Used to
     *              determine which level to create a stand-in in if the asset cannot be loaded. Will not create a
     *              a stand-in if this is nullptr.
     * @param   bool silent - if true, will not log a warning if the asset cannot be loaded.
     * @return  UObject* asset or stand-in for the asset. nullptr if the asset was not found and a stand-in could not
     *              be created.
     */
    UObject* Load(const FString& path, const FString& className, sfObject::SPtr contextPtr, bool silent = false);

    /**
     * Checks if an object is a stand-in for a missing asset.
     *
     * @param   UObject* uobjPtr
     * @return  bool true if the object is a stand-in.
     */
    bool IsStandIn(UObject* uobjPtr);

    /**
     * Gets the class name and path of the asset a stand-in is representing.
     *
     * @param   UObject* standInPtr
     * @return  FString class name and path seperated by a ';'.
     */
    FString GetPathFromStandIn(UObject* standInPtr);

    /**
     * Increments the stand-in count for a stand-in's missing asset path, and broadcasts a missing asset event if this
     * is the first stand-in for this asset path.
     *
     * @param   UObject* standInPtr to increase count for.
     */
    void AddStandIn(UObject* standInPtr);

    /**
     * Increments the stand-in count for a stand-in's missing asset path, and broadcasts a missing asset event if this
     * is the first stand-in for this asset path.
     *
     * @param   const FString& path to increase count for.
     */
    void AddStandIn(const FString& path);

    /**
     * Decrements the stand-in count for a stand-in's missing asset path, and broadcasts a remove missing asset event
     * if the stand-in count becomes zero.
     *
     * @param   UObject* standInPtr to decrease count for.
     */
    void RemoveStandIn(UObject* standInPtr);

    /**
     * Decrements the stand-in count for a stand-in's missing asset path, and broadcasts a remove missing asset event
     * if the stand-in count becomes zero.
     *
     * @param   const FString& to decrease count for.
     */
    void RemoveStandIn(const FString& path);

    /**
     * Adds a property to the stand-in property map. When the stand-in is replaced with the missing asset, the property
     * will be reloaded so it references the asset instead of the stand-in. Asset reference properties do not need to
     * be passed to this function as they are detected and updated automatically, but more complex properties that
     * reference assets (such as serialized binary data which may contain asset references and other data) need to be
     * added to the map.
     *
     * @param   uint32_t pathId - string table id of the stand-in class and path seperated by a ';'.
     * @param   sfProperty::SPtr propertyPtr that references the stand-in.
     */
    void AddStandInReference(uint32_t pathId, sfProperty::SPtr propertyPtr);

    /**
     * Loads an asset from memory. If the asset was not found in memory, looks for a stand-in to return or returns
     * nullptr if not asset is found.
     *
     * @param   FString path to the asset.
     * @param   const FString& className of asset to load.
     * @param   sfObject::SPtr contextPtr - sfObject containing the property we are loading the reference for. Used to
     *              determine which level to look for a stand-in in if the asset cannot be loaded. Will not check for a
     *              stand-in if this is nullptr.
     * @return  UObject* loaded asset or nullptr.
     */
    UObject* LoadFromCache(FString path, const FString& className, sfObject::SPtr contextPtr);

    /**
     * Called every tick while the sfLoader is running. Loads assets if the user is idle and sets references to them,
     * and replaces stand-ins with their proper assets if they are available.
     *
     * @param   const float deltaTime
     * @param   FSlateApplication& slateApp
     * @param   TSharedRef<ICursor> cursor
     */
    virtual void Tick(const float deltaTime, FSlateApplication& slateApp, TSharedRef<ICursor> cursor) override;

    /**
     * Called when a mouse button is pressed.
     *
     * @param   FSlateApplication& slateApp
     * @param   const FPointerEvent& mouseEvent
     * @return  bool false to indicate the event was not handled.
     */
    virtual bool HandleMouseButtonDownEvent(FSlateApplication& slateApp, const FPointerEvent& mouseEvent) override;

    /**
     * Called when a mouse button is released.
     *
     * @param   FSlateApplication& slateApp
     * @param   const FPointerEvent& mouseEvent
     * @return  bool false to indicate the event was not handled.
     */
    virtual bool HandleMouseButtonUpEvent(FSlateApplication& slateApp, const FPointerEvent& mouseEvent) override;

    /**
     * Checks if an asset type should be created if it is missing.
     *
     * @param   UClass* classPtr
     * @return  bool true if the asset type should be created if it is missing.
     */
    bool IsCreatableAssetType(UClass* classPtr);

    /**
     * Checks if an asset was a missing asset we created when we tried to load it.
     *
     * @param   UObject* assetPtr
     * @return  bool true if the asset was created when we tried to load it.
     */
    bool WasCreatedOnLoad(UObject* assetPtr);

    /**
     * Registers an asset type to be created if it is missing when we try to load it.
     */
    template<typename T>
    void RegisterCreatableAssetType()
    {
        m_createTypes.Add(T::StaticClass());
    }

    /**
     * Unregisters a creatable asset type.
     */
    template<typename T>
    void UnregisterCreatableAssetType()
    {
        m_createTypes.Remove(T::StaticClass());
    }

private:
    static TSharedPtr<sfLoader> m_instancePtr;

    /**
     * sfIStandInGenerator implementation that wraps a delegate.
     */
    class Generator : public sfIStandInGenerator
    {
    public:
        /**
         * Constructor
         *
         * @param StandInGenerator generator
         */
        Generator(StandInGenerator generator) :
            m_generator{ generator }
        {

        }

        /**
         * Destructor
         */
        virtual ~Generator() {

        }

        /**
         * Calls the generator delegate.
         *
         * @param   const FString* path of missing asset.
         * @param   UObject* uobjPtr stand-in to generate data for.
         */
        virtual void Generate(const FString& path, UObject* uobjPtr) override
        {
            m_generator(path, uobjPtr);
        }

    private:
        StandInGenerator m_generator;
    };

    // Maps objects to a list of their properties that referenced assets to be loaded when the user is idle.
    std::unordered_map<sfObject::SPtr, std::vector<sfProperty::SPtr>> m_delayedAssets;
    // Maps classes to the path to their stand-in asset. If a class is not in the map, a stand-in is created using
    // NewObject.
    TMap<UClass*, FString> m_classToStandInPath;
    TMap<UClass*, TSharedPtr<sfIStandInGenerator>> m_standInGenerators;
    // Number of stand-ins for a missing asset path. A missing asset has one stand-in per level that references the
    // asset.
    TMap<FString, int> m_standInCounts;
    TSet<FString> m_transientStandInPaths;
    TSet<UObject*> m_transientStandIns;
    // Maps stand-in path string table ids to properties that contain references to the stand-in. Properties that are
    // just an asset reference aren't in this map as they are detected automatically, but more complex properties (such
    // as binary data containing asset references) need to be in this map so the asset reference can be updated when
    // the missing asset becomes available.
    std::unordered_map<uint32_t, std::vector<std::weak_ptr<sfProperty>>> m_standInPropertyMap;
    TArray<FString> m_standInsToReplace;
    // Will create assets of these types when loading if the asset does not exist
    TSet<UClass*> m_createTypes;
    TSet<UObject*> m_createdAssets;
    bool m_isMouseDown;
    bool m_overrideIdle;// if true, IsUserIdle() returns true even if the user isn't idle
    FDelegateHandle m_onNewAssetHandle;
    FDelegateHandle m_preUpdateReferencesHandle;

    /**
     * Gets the name for a stand-in from a path and class name.
     *
     * @param   const FString& path to the missing asset.
     * @param   const FString& className of the missing asset.
     * @param   FString& outName - set to the name of the stand-in for the missing asset.
     */
    void GetStandInName(const FString& path, const FString& className, FString& outName);

    /**
     * Gets the outer for a stand-in from a context sfObject. Returns the first level found by traversing the context
     * object and its ancestors, or returns the world if no level is found. Returns nullptr if the context is nullptr.
     *
     * @param   sfObject::SPtr contextPtr
     * @return  UObject* outer for the stand-in.
     */
    UObject* GetStandInOuter(sfObject::SPtr contextPtr);

    /**
     * Tries to find a stand-in for an asset. Returns nullptr if no stand-in can be found.
     *
     * @param   FString path to the asset.
     * @param   const FString& className of asset to get stand-in for.
     * @param   sfObject::SPtr contextPtr - sfObject containing the property we are getting the stand-in for. Used to
     *              determine which level to look for a stand-in in. Will not check for a stand-in if this is nullptr.
     * @return  UObject* stand-in or nullptr.
     */
    UObject* TryFindStandIn(FString path, const FString& className, sfObject::SPtr contextPtr);

    /**
     * Replaces references to stand-ins with the proper assets.
     */
    void ReplaceStandIns();

    /**
     * Checks if a property is a value property referencing one of the given stand-in paths.
     *
     * @param   sfProperty::SPtr propertyPtr to check.
     * @param   const TSet<FString>& paths to stand-in assets being replaced.
     * @return  bool true if the property is one of the stand-in asset paths.
     */
    bool ReferencesStandIn(sfProperty::SPtr propertyPtr, const TSet<FString>& paths);

    /**
     * Loads delayed assets and sets references to them.
     */
    void LoadDelayedAssets();

    /**
     * Loads the asset for a property and sets the reference to it.
     *
     * @param   sfProperty::SPtr propPtr
     */
    void LoadProperty(sfProperty::SPtr propPtr);

    /**
     * Called when a new asset is created. If the asset has a stand-in, adds the stand-in to a list to replaced with
     * the new asset.
     *
     * @param   const FAssetData& assetData
     */
    void OnNewAsset(const FAssetData& assetData);
};
