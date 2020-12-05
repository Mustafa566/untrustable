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

#include "sfMissingObject.h"
#include <Runtime/Engine/Classes/Engine/AssetManager.h>
#include <sfObject.h>
#include <sfDictionaryProperty.h>

using namespace KS::SceneFusion2;

/**
 * Maps missing class/blueprint names to stand-in objects and replaces the stand-ins with the correct class/blueprint
 * when they become available.
 */
class sfMissingObjectManager
{
public:
    /**
     * @return  sfMissingObjectManager& singleton instance.
     */
    static sfMissingObjectManager& Get();

    /**
     * Delegate for on new stand-in.
     *
     * @param   const FString& path to missing asset.
     * @param   UObject* stand-in
     */
    DECLARE_MULTICAST_DELEGATE_OneParam(OnMissingAssetDelegate, const FString&);

    /**
     * Delegate for on remove missing asset.
     *
     * @param   const FString& path to missing asset that was found or no longer has stand-ins.
     */
    DECLARE_MULTICAST_DELEGATE_OneParam(OnRemoveMissingAssetDelegate, const FString&);

    /**
     * Invoked when an asset cannot be found. This is invoked once per asset path.
     */
    OnMissingAssetDelegate OnMissingAsset;

    /**
     * Invoked when a missing asset is found or all stand-ins for the asset are deleted.
     */
    OnRemoveMissingAssetDelegate OnRemoveMissingAsset;

    /**
     * Constructor
     */
    sfMissingObjectManager();

    /**
     * Initialization. Adds event handlers that listen for new assets.
     */
    void Initialize();

    /**
     * Adds a stand-in to the stand-in map.
     *
     * @param   IsfMissingObject* standInPtr to add.
     */
    void AddStandIn(IsfMissingObject* standInPtr);

    /**
     * Removes a stand-in from the stand-in map.
     *
     * @param   IsfMissingObject* standInPtr to remove.
     */
    void RemoveStandIn(IsfMissingObject* standInPtr);

    /**
     * Adds an sfObject for an asset that is missing. When the asset is found, the create event will be dispatched
     * again for the sfObject.
     *
     * @param   const FString& path to missing asset.
     * @param   sfObject::SPtr objPtr for the missing asset.
     */
    void AddMissingObject(const FString& path, sfObject::SPtr objPtr);

    /**
     * Clears the missing object list for missing assets.
     */
    void ClearMissingObjects();

    /**
     * Reloads stand-ins for a path after a short delay.
     *
     * @param   const FString& path to reload stand-ins for an actor or component.
     */
    void DelayedReload(const FString& path);

    /**
     * Serializes missing properties on a stand-in. If a property cannot be found on the stand-in, it is serialized and
     * added to the stand-in's serialized property map.
     *
     * @param   IsfMissingObject* standInPtr to serialize missing properties for.
     * @param   sfDictionaryProperty::SPtr propertiesPtr to iterate and serialize properties that are missing from the
     *          stand-in.
     */
    void SerializeProperties(IsfMissingObject* standInPtr, sfDictionaryProperty::SPtr propertiesPtr);

    /**
     * Serializes a property on a stand-in. If the property cannot be found on the stand-in, it is serialized and added
     * to the stand-in's serialized property map.
     *
     * @param   IsfMissingObject* standInPtr to serialize missing property for.
     * @param   sfProperty::SPtr propertyPtr - the 2nd top-level property containing this property will be serialized
     *          if it cannot be found on the stand-in.
     */
    void SerializeProperty(IsfMissingObject* standInPtr, sfProperty::SPtr propertyPtr);

    /**
     * Deserializes missing properties from a stand-in to a dictionary property.
     *
     * @param   IsfMissingObject* standInPtr to deserialize missing properties from.
     * @param   sfDictionaryProperty::SPtr propertiesPtr to deserialize to.
     */
    void DeserializeProperties(IsfMissingObject* standInPtr, sfDictionaryProperty::SPtr propertiesPtr);

    /**
     * @return  TMap<FString, TArray<IsfMissingObject*>>::TConstIterator iterator for the stand-ins for missing
     *          objects.
     */
    TMap<FString, TArray<IsfMissingObject*>>::TConstIterator Iterate();

private:
    // Maps missing class/blueprint names/paths to stand-in objects.
    TMap<FString, TArray<IsfMissingObject*>> m_standInMap;
    TMap<FString, sfObject::SPtr> m_missingObjects;
    FDelegateHandle m_onHotReloadHandle;
    FDelegateHandle m_onNewAssetHandle;
    FDelegateHandle m_preUpdateReferencesHandle;
    TSet<FString> m_newAssetPaths;

    /**
     * Replaces stand-in objects for assets that were newly added.
     */
    void ReloadObjects();

    /**
     * Adds referenced uobjects to a stand-in's reference map for a property and its subproperties.
     *
     * @param   IsfMissingObject* standInPtr to add references to.
     * @param   sfProperty::SPtr propertyPtr to get references from.
     */
    void AddReferences(IsfMissingObject* standInPtr, sfProperty::SPtr propertyPtr);

    /**
     * Adds a referenced uobject to a stand-in's reference map.
     *
     * @param   IsfMissingObject* standInPtr to add reference to.
     * @param   sfReferenceProperty::SPtr propertyPtr to get reference from.
     */
    void AddReference(IsfMissingObject* standInPtr, sfReferenceProperty::SPtr propertyPtr);

    /**
     * Called after game code is compiled. Checks if missing classes are now available and replaces stand-ins with
     * instances of the newly available classes.
     *
     * @param   bool automatic -  true if the recompile was triggered automatically.
     */
    void OnHotReload(bool automatic);

    /**
     * Called when a new asset is created. If the asset is a blueprint, replaces any stand-ins for that blueprint with
     * instances of the blueprint.
     *
     * @param   const FAssetData& assetData for the new asset.
     */
    void OnNewAsset(const FAssetData& assetData);
};