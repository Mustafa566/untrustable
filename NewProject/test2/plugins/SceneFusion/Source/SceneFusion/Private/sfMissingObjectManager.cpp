#include "../Public/sfMissingObjectManager.h"
#include "../Public/sfUnrealUtils.h"
#include "../Public/SceneFusion.h"
#include "../Public/sfReferenceFinder.h"
#include "../Public/sfWriter.h"
#include "../Public/sfUPropertyTypes.h"
#include "../Public/sfObjectMap.h"
#include "../Public/sfLoader.h"
#include <sfPropertyIterator.h>
#include <Developer/HotReload/Public/IHotReload.h>
#include <vector>

sfMissingObjectManager::sfMissingObjectManager()
{

}

sfMissingObjectManager& sfMissingObjectManager::Get()
{
    static sfMissingObjectManager instance;
    return instance;
}

void sfMissingObjectManager::Initialize()
{
    m_onHotReloadHandle = IHotReloadModule::Get().OnHotReload().AddRaw(this, &sfMissingObjectManager::OnHotReload);
    m_onNewAssetHandle = UAssetManager::Get().GetAssetRegistry().OnAssetAdded().AddRaw(this,
        &sfMissingObjectManager::OnNewAsset);
    m_preUpdateReferencesHandle = sfReferenceFinder::Get().PreUpdateReferences.AddRaw(this,
        &sfMissingObjectManager::ReloadObjects);
}

void sfMissingObjectManager::ReloadObjects()
{
    for (FString& path : m_newAssetPaths)
    {
        TArray<IsfMissingObject*> standIns;
        if (!m_standInMap.RemoveAndCopyValue(path, standIns))
        {
            continue;
        }
        for (IsfMissingObject* standInPtr : standIns)
        {
            standInPtr->Reload();
        }
        OnRemoveMissingAsset.Broadcast(path);
    }
    m_newAssetPaths.Empty();
}

void sfMissingObjectManager::AddStandIn(IsfMissingObject* standInPtr)
{
    TArray<IsfMissingObject*>& standIns = m_standInMap.FindOrAdd(standInPtr->MissingClass());
    standIns.AddUnique(standInPtr);
    if (SceneFusion::Service->Session() == nullptr &&
        sfUnrealUtils::LoadClass(standInPtr->MissingClass(), true) != nullptr)
    {
        DelayedReload(standInPtr->MissingClass());
    }
    else if (standIns.Num() == 1)
    {
        OnMissingAsset.Broadcast(standInPtr->MissingClass());
    }
}

void sfMissingObjectManager::RemoveStandIn(IsfMissingObject* standInPtr)
{
    TArray<IsfMissingObject*>* standInsPtr = m_standInMap.Find(standInPtr->MissingClass());
    if (standInsPtr != nullptr)
    {
        standInsPtr->Remove(standInPtr);
        if (standInsPtr->Num() == 0)
        {
            m_standInMap.Remove(standInPtr->MissingClass());
            OnRemoveMissingAsset.Broadcast(standInPtr->MissingClass());
        }
    }
}

void sfMissingObjectManager::AddMissingObject(const FString& path, sfObject::SPtr objPtr)
{
    m_missingObjects.Add(path, objPtr);
}

void sfMissingObjectManager::ClearMissingObjects()
{
    m_missingObjects.Empty();
}

void sfMissingObjectManager::DelayedReload(const FString& path)
{
    m_newAssetPaths.Add(path);
    sfReferenceFinder::Get().DelayedUpdateReferences();
}

void sfMissingObjectManager::SerializeProperties(
    IsfMissingObject* standInPtr,
    sfDictionaryProperty::SPtr propertiesPtr)
{
    UObject* uobjPtr = Cast<UObject>(standInPtr);
    if (uobjPtr == nullptr)
    {
        return;
    }
    for (auto iter = propertiesPtr->begin(); iter != propertiesPtr->end(); ++iter)
    {
        if (iter->first->size() > 0 && iter->first->at(0) == '#')
        {
            continue;
        }
        FName name = FName(UTF8_TO_TCHAR(iter->first->c_str()));
        UnrealProperty* upropPtr = uobjPtr->GetClass()->FindPropertyByName(name);
        if (upropPtr == nullptr)
        {
            std::vector<uint8_t> output;
            sfProperty::Serialize(iter->second, output);
            FsfByteArray& data = standInPtr->SerializedProperties().FindOrAdd(name);
            data.Data = TArray<uint8>((uint8*)output.data(), output.size());
            // Add mapping of sfObject id to UObject* for uobjects referenced by properties that could not be found on
            // the stand-in, so when we deserialize we can match the id with the uobject.
            AddReferences(standInPtr, iter->second);
        }
    }
}

void sfMissingObjectManager::SerializeProperty(IsfMissingObject* standInPtr, sfProperty::SPtr propertyPtr)
{
    UObject* uobjPtr = Cast<UObject>(standInPtr);
    if (uobjPtr == nullptr)
    {
        return;
    }

    // Get property at depth 1
    int depth = propertyPtr->GetDepth();
    sfProperty::SPtr currentPtr = propertyPtr;
    while (depth > 1)
    {
        currentPtr = currentPtr->GetParentProperty();
        depth--;
    }
    FName name = FName(UTF8_TO_TCHAR(currentPtr->Key()->c_str()));
    UnrealProperty* upropPtr = uobjPtr->GetClass()->FindPropertyByName(name);
    if (upropPtr == nullptr)
    {
        std::vector<uint8_t> output;
        sfProperty::Serialize(currentPtr, output);
        FsfByteArray& data = standInPtr->SerializedProperties().FindOrAdd(name);
        data.Data = TArray<uint8>((uint8*)output.data(), output.size());
        // Add mapping of sfObject id to UObject* for uobjects referenced by properties that could not be found on
        // the stand-in, so when we deserialize we can match the id with the uobject.
        AddReferences(standInPtr, currentPtr);
    }
}

void sfMissingObjectManager::DeserializeProperties(
    IsfMissingObject* standInPtr,
    sfDictionaryProperty::SPtr propertiesPtr)
{
    if (SceneFusion::Get().Service->Session() == nullptr)
    {
        // Create mapping of ids to uobjects so when we deserializes references we can match the id to the uobject.
        for (auto iter : standInPtr->References())
        {
            sfObjectMap::Add((uint32_t)iter.Key, iter.Value);
        }
    }
    for (auto iter : standInPtr->SerializedProperties())
    {
        sfName name = TCHAR_TO_UTF8(*iter.Key.ToString());
        std::vector<uint8_t> input;
        input.resize(iter.Value.Data.Num());
        uint8_t* dataPtr = (uint8_t*)iter.Value.Data.GetData();
        input.assign(dataPtr, dataPtr + iter.Value.Data.Num());
        sfProperty::SPtr propPtr = sfProperty::Deserialize(input);
        if (propPtr != nullptr)
        {
            propertiesPtr->Set(name, propPtr);
        }
    }
}

void sfMissingObjectManager::AddReferences(IsfMissingObject* standInPtr, sfProperty::SPtr propertyPtr)
{
    if (propertyPtr->Type() == sfProperty::Types::REFERENCE)
    {
        AddReference(standInPtr, propertyPtr->AsReference());
    }
    else
    {
        sfPropertyIterator propIter = propertyPtr->Iterate();
        while (propIter.Next())
        {
            if (propIter.Value()->Type() == sfProperty::Types::REFERENCE)
            {
                AddReference(standInPtr, propIter.Value()->AsReference());
            }
        }
    }
}

void sfMissingObjectManager::AddReference(IsfMissingObject* standInPtr, sfReferenceProperty::SPtr propertyPtr)
{
    uint32_t objId = propertyPtr->GetObjectId();
    sfObject::SPtr objPtr = SceneFusion::Service->Session()->GetObject(objId);
    UObject* referencePtr = sfObjectMap::GetUObject(objPtr);
    if (referencePtr != nullptr)
    {
        standInPtr->References().Add((uint32)objId, referencePtr);
    }
}

TMap<FString, TArray<IsfMissingObject*>>::TConstIterator sfMissingObjectManager::Iterate()
{
    return m_standInMap.CreateConstIterator();
}

void sfMissingObjectManager::OnHotReload(bool automatic)
{
    // Iterate using a copy of the keys so modifications are safe while iterating
    TArray<FString> keys;
    m_standInMap.GetKeys(keys);
    TArray<FString> keysToRemove;
    for (FString& key : keys)
    {
        TArray<IsfMissingObject*>& standIns = m_standInMap[key];
        TArray<IsfMissingObject*> standInsToRemove;
        for (IsfMissingObject* standInPtr : standIns)
        {
            if (standInPtr->Reload())
            {
                standInsToRemove.Add(standInPtr);
            }
        }
        for (IsfMissingObject* standInPtr : standInsToRemove)
        {
            standIns.Remove(standInPtr);
        }
        if (standIns.Num() == 0)
        {
            keysToRemove.Add(key);
        }
    }
    for (FString& key : keysToRemove)
    {
        m_standInMap.Remove(key);
        OnRemoveMissingAsset.Broadcast(key);
    }
    sfReferenceFinder::Get().DelayedUpdateReferences();
}

void sfMissingObjectManager::OnNewAsset(const FAssetData& assetData)
{
    FString path = assetData.ObjectPath.ToString();
    int index = path.Find(".");
    if (index >= 0)
    {
        path = path.Left(index);
    }

    // Redispatch the create event for the object for this asset
    sfObject::SPtr objPtr;
    if (m_missingObjects.RemoveAndCopyValue(path, objPtr))
    {
        SceneFusion::ObjectEventDispatcher->OnCreate(objPtr, 0);
    }

    if (m_standInMap.Contains(path))
    {
        DelayedReload(path);
    }
}