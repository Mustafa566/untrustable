#include "../Public/sfLoader.h"
#include "../Public/sfPropertyManager.h"
#include "../Public/sfObjectMap.h"
#include "../Public/sfConfig.h"
#include "../Public/sfUnrealUtils.h"
#include "../Public/Consts.h"
#include "../Public/SceneFusion.h"
#include "../Public/sfUnrealEventDispatcher.h"
#include "../Public/sfReferenceFinder.h"
#include "Actors/sfStandInTrackerActor.h"

#include <Editor.h>
#include <Log.h>
#include <sfPropertyIterator.h>
#include <sfConfig.h>
#include <sfStringProperty.h>
#include <Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h>
#include <Runtime/CoreUObject/Public/UObject/CoreRedirects.h>
#include <Rendering/SkeletalMeshRenderData.h>
#include <Engine/SkeletalMesh.h>
#include <Engine/TextureCube.h>
#include <Engine/TextureRenderTargetCube.h>
#include <../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h>
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
#include <Runtime/RawMesh/Public/RawMesh.h>
#else
#include <Developer/RawMesh/Public/RawMesh.h>
#endif

#define LOG_CHANNEL "sfLoader"

TSharedPtr<sfLoader> sfLoader::m_instancePtr = nullptr;

sfLoader& sfLoader::Get()
{
    if (!m_instancePtr.IsValid())
    {
        m_instancePtr = MakeShareable(new sfLoader);
    }
    return *m_instancePtr;
}

sfLoader::sfLoader() :
    m_isMouseDown{ false },
    m_overrideIdle{ false }
{
    m_classToStandInPath.Add(USkeletalMesh::StaticClass(), "/SceneFusion/StandIns/Skeletal");
    m_classToStandInPath.Add(UMaterial::StaticClass(), "/SceneFusion/StandIns/Material");
    m_classToStandInPath.Add(UTexture::StaticClass(), "/SceneFusion/StandIns/Question");
    m_classToStandInPath.Add(UPaperSprite::StaticClass(), "/SceneFusion/StandIns/Sprite");
    m_classToStandInPath.Add(UTextureCube::StaticClass(), "/Engine/EngineMaterials/DefaultCubemap");

    RegisterStandInGenerator(UTextureRenderTargetCube::StaticClass(), [](const FString& path, UObject* uobjPtr)
    {
        UTextureRenderTargetCube* targetPtr = Cast<UTextureRenderTargetCube>(uobjPtr);
        targetPtr->Init(1, EPixelFormat::PF_B8G8R8A8);
    });
}

sfLoader::~sfLoader() {

}

void sfLoader::Initialize()
{
    m_onNewAssetHandle = UAssetManager::Get().GetAssetRegistry().OnAssetAdded().AddRaw(this, &sfLoader::OnNewAsset);
    m_preUpdateReferencesHandle = sfReferenceFinder::Get().PreUpdateReferences.AddRaw(this,
        &sfLoader::ReplaceStandIns);
}

void sfLoader::Start()
{
    FSlateApplication::Get().RegisterInputPreProcessor(m_instancePtr);
}

void sfLoader::Stop()
{
    m_isMouseDown = false;
    m_delayedAssets.clear();
    m_createdAssets.Empty();
    FSlateApplication::Get().UnregisterInputPreProcessor(m_instancePtr);
    // Decrement stand-in counts for transient stand-ins since they don't save and cannot be replaced outside the
    // session
    for (const FString& path : m_transientStandInPaths)
    {
        RemoveStandIn(path);
    }
    m_transientStandInPaths.Empty();
    m_transientStandIns.Empty();
}

void sfLoader::RegisterStandInGenerator(UClass* classPtr, TSharedPtr<sfIStandInGenerator> generatorPtr)
{
    m_standInGenerators.Add(classPtr, generatorPtr);
}

void sfLoader::RegisterStandInGenerator(UClass* classPtr, sfLoader::StandInGenerator generator)
{
    m_standInGenerators.Add(classPtr, MakeShareable(new Generator(generator)));
}

bool sfLoader::IsUserIdle()
{
    return m_overrideIdle || (!m_isMouseDown &&
        FSlateApplication::Get().GetCurrentTime() - FSlateApplication::Get().GetLastUserInteractionTime() > 
        sfConfig::Get().IdleTime);
}

void sfLoader::LoadWhenIdle(sfProperty::SPtr propPtr)
{
    std::vector<sfProperty::SPtr>& properties = m_delayedAssets[propPtr->GetContainerObject()];
    if (std::find(properties.begin(), properties.end(), propPtr) == properties.end())
    {
        properties.emplace_back(propPtr);
    }
}

void sfLoader::LoadAssetsFor(sfObject::SPtr objPtr)
{
    auto iter = m_delayedAssets.find(objPtr);
    if (iter != m_delayedAssets.end())
    {
        m_overrideIdle = true;
        for (sfProperty::SPtr propPtr : iter->second)
        {
            LoadProperty(propPtr);
        }
        m_overrideIdle = false;
        m_delayedAssets.erase(iter);
    }
    for (sfObject::SPtr childPtr : objPtr->Children())
    {
        if (childPtr->Type() == sfType::Component)
        {
            LoadAssetsFor(childPtr);
        }
    }
}

UObject* sfLoader::Load(const FString& path, const FString& className, sfObject::SPtr contextPtr, bool silent)
{
    GIsEditorLoadingPackage = true;
    UObject* assetPtr = LoadObject<UObject>(nullptr, *path);
    GIsEditorLoadingPackage = false;
    if (assetPtr != nullptr)
    {
        return assetPtr;
    }

    FString name;
    GetStandInName(path, className, name);
    UObject* outerPtr = GetStandInOuter(contextPtr);
    // Try to find a stand-in
    if (outerPtr != nullptr)
    {
        assetPtr = StaticFindObjectFast(UObject::StaticClass(), outerPtr, *name);
        if (assetPtr != nullptr)
        {
            return assetPtr;
        }
    }

    UClass* classPtr = sfUnrealUtils::LoadClass(className);
    if (classPtr == nullptr)
    {
        return nullptr;
    }

    if (IsCreatableAssetType(classPtr))
    {
        KS::Log::Info(TCHAR_TO_UTF8(*("Creating " + className + " asset at '" + path + "'.")), LOG_CHANNEL);
        FString packageName;
        path.Split(".", &packageName, &name);
        UPackage* packagePtr = CreatePackage(nullptr, *packageName);
        if (packagePtr != nullptr)
        {
            assetPtr = NewObject<UObject>(packagePtr, classPtr, *name, RF_Public | RF_Transactional);
            if (assetPtr != nullptr)
            {
                m_createdAssets.Add(assetPtr);
                FAssetRegistryModule::AssetCreated(assetPtr);
                assetPtr->MarkPackageDirty();
                OnCreateMissingAsset.Broadcast(assetPtr);
                return assetPtr;
            }
        }
    }
    
    if (outerPtr == nullptr)
    {
        if (!silent)
        {
            KS::Log::Warning(TCHAR_TO_UTF8(*("Could not find " + className + " '" + path + "'.")), LOG_CHANNEL);
        }
        return nullptr;
    }
    if (!silent)
    {
        KS::Log::Warning(TCHAR_TO_UTF8(
            *("Could not find " + className + " '" + path + "'. Creating stand-in object.")), LOG_CHANNEL);
    }

    // Public makes copy/paste work.
    EObjectFlags flags = RF_Public;
    if (outerPtr == GEditor->GetEditorWorldContext().World())
    {
        // Don't save stand-ins that aren't in a level.
        flags |= RF_Transient;
    }
    UObject* standInPtr = nullptr;
    FString* pathPtr = m_classToStandInPath.Find(classPtr);
    if (pathPtr != nullptr)
    {
        UObject* standInAssetPtr = LoadObject<UObject>(nullptr, **pathPtr);
        if (standInAssetPtr != nullptr)
        {
            standInPtr = DuplicateObject(standInAssetPtr, GEditor->GetEditorWorldContext().World(), *name);
            standInPtr->SetFlags(flags);
            // RF_Standalone prevents the object from being garbage collected and causes an error when entering play
            // mode, so we remove it.
            standInPtr->ClearFlags(RF_Standalone);
        }
    }
    if (standInPtr == nullptr)
    {
        standInPtr = NewObject<UObject>(outerPtr, classPtr, *name, flags);
    }
    
    TSharedPtr<sfIStandInGenerator> generatorPtr;
    generatorPtr = m_standInGenerators.FindRef(classPtr);
    if (generatorPtr.IsValid())
    {
        generatorPtr->Generate(path, standInPtr);
    }

    // Add the stand-in to the stand-in tracker for the level it is in.
    ULevel* levelPtr = Cast<ULevel>(outerPtr);
    if (levelPtr != nullptr)
    {
        AsfStandInTrackerActor::Get(levelPtr)->Add(path, standInPtr);
    }
    else
    {
        m_transientStandInPaths.Add(path);
        m_transientStandIns.Add(standInPtr);
    }

    AddStandIn(path);
    return standInPtr;
}

bool sfLoader::IsStandIn(UObject* uobjPtr)
{
    if (uobjPtr == nullptr)
    {
        return false;
    }
    if (!uobjPtr->GetName().StartsWith("Missing_"))
    {
        return false;
    }
    ULevel* levelPtr = Cast<ULevel>(uobjPtr->GetOuter());
    if (levelPtr == nullptr)
    {
        return m_transientStandIns.Contains(uobjPtr);
    }
    AsfStandInTrackerActor* trackerPtr = AsfStandInTrackerActor::Get(levelPtr, false);
    return trackerPtr != nullptr && trackerPtr->Contains(uobjPtr);
}

FString sfLoader::GetPathFromStandIn(UObject* standInPtr)
{
    if (standInPtr == nullptr)
    {
        return "";
    }
    FString name = standInPtr->GetName();
    if (!name.StartsWith("Missing_"))
    {
        return "";
    }
    // Remove "Missing_"
    FString path = name.RightChop(8);
    // We had to replace ";" and "." with "+" earlier for Unreal deserialization to work. The first occurence is always
    // a ";" and the rest are "."
    int index;
    if (path.FindChar('+', index))
    {
        path[index] = ';';
        path.ReplaceInline(*FString("+"), *FString("."));
    }
    return path;
}

void sfLoader::AddStandIn(UObject* standInPtr)
{
    FString str = GetPathFromStandIn(standInPtr);
    if (str.IsEmpty())
    {
        return;
    }
    FString className, path;
    str.Split(";", &className, &path);
    if (!path.IsEmpty())
    {
        AddStandIn(path);
    }
}

void sfLoader::AddStandIn(const FString& path)
{
    int count = m_standInCounts.FindRef(path) + 1;
    m_standInCounts.Add(path, count);
    if (count == 1)
    {
        OnMissingAsset.Broadcast(path);
    }
}

void sfLoader::RemoveStandIn(UObject* standInPtr)
{
    FString path = GetPathFromStandIn(standInPtr);
    if (!path.IsEmpty())
    {
        RemoveStandIn(path);
    }
}

void sfLoader::RemoveStandIn(const FString& path)
{
    int count = m_standInCounts.FindRef(path);
    if (count == 0)
    {
        return;
    }
    count--;
    if (count == 0)
    {
        m_standInCounts.Remove(path);
        OnRemoveMissingAsset.Broadcast(path);
    }
    else
    {
        m_standInCounts[path] = count;
    }
}

void sfLoader::AddStandInReference(uint32_t pathId, sfProperty::SPtr propertyPtr)
{
    std::vector<std::weak_ptr<sfProperty>>& properties = m_standInPropertyMap[pathId];
    // Check if the property is already added and remove expired weak pointers
    bool found = false;
    for (auto iter = properties.begin(); iter != properties.end();)
    {
        if (iter->expired())
        {
            properties.erase(iter++);
        }
        else
        {
            if (iter->lock() == propertyPtr)
            {
                found = true;
            }
            ++iter;
        }
    }
    // Add the property if it wasn't found
    if (!found)
    {
        properties.emplace_back(std::weak_ptr<sfProperty>(propertyPtr));
    }
}

// This code is mostly copied from Unreal's ResolveName function and modified to not load the asset if it's not already
// in memory, and to fit our style.
UObject* sfLoader::LoadFromCache(FString path, const FString& className, sfObject::SPtr contextPtr)
{
    UObject* assetPtr = nullptr;

    // Strip off the object class.
    ConstructorHelpers::StripObjectClass(path);

    path = FPackageName::GetDelegateResolvedPackagePath(path);
    bool isSubobjectPath = false;
    int dotIndex = INDEX_NONE;

    // to make parsing the name easier, replace the subobject delimiter with an extra dot
    path.ReplaceInline(SUBOBJECT_DELIMITER, TEXT(".."), ESearchCase::CaseSensitive);
    while ((dotIndex = path.Find(TEXT("."), ESearchCase::CaseSensitive)) != INDEX_NONE)
    {
        FString partialName = path.Left(dotIndex);

        // if the next part of path ends in two dots, it indicates that the next object in the path name
        // is not a top-level object (i.e. it's a subobject).  e.g. SomePackage.SomeGroup.SomeObject..Subobject
        if (path.IsValidIndex(dotIndex + 1) && path[dotIndex + 1] == TEXT('.'))
        {
            path.RemoveAt(dotIndex, 1, false);
            isSubobjectPath = true;
        }

        FName* scriptPackageName = nullptr;
        if (!isSubobjectPath)
        {
            // In case this is a short script package name, convert to long name before passing to
            // CreatePackage/FindObject.
            scriptPackageName = FPackageName::FindScriptPackageName(*partialName);
            if (scriptPackageName)
            {
                partialName = scriptPackageName->ToString();
            }
        }

        // Process any package redirects before calling CreatePackage/FindObject
        {
            const FCoreRedirectObjectName newPackageName = FCoreRedirects::GetRedirectedName(
                ECoreRedirectFlags::Type_Package, FCoreRedirectObjectName(NAME_None, NAME_None, *partialName));
            partialName = newPackageName.PackageName.ToString();
        }

        // Only long package names are allowed so don't even attempt to create one because whatever the name represents
        // it's not a valid package name anyway.

        if (isSubobjectPath)
        {
            UObject* newPackage = FindObject<UPackage>(assetPtr, *partialName);
            if (!newPackage)
            {
                newPackage = FindObject<UObject>(assetPtr == NULL ? ANY_PACKAGE : assetPtr, *partialName);
                if (!newPackage)
                {
                    return TryFindStandIn(path, className, contextPtr);
                }
            }
            assetPtr = newPackage;
        }
        else if (!FPackageName::IsShortPackageName(partialName))
        {
            // Try to find the package in memory
            assetPtr = StaticFindObjectFast(UPackage::StaticClass(), assetPtr, *partialName);
            if (!assetPtr)
            {
                return TryFindStandIn(path, className, contextPtr);
            }
        }
        path.RemoveAt(0, dotIndex + 1, false);
    }

    if (assetPtr == nullptr)
    {
        return TryFindStandIn(path, className, contextPtr);
    }
    assetPtr = StaticFindObjectFast(UObject::StaticClass(), assetPtr, *path);
    if (assetPtr && assetPtr->HasAnyFlags(RF_NeedLoad | RF_NeedPostLoad | RF_NeedPostLoadSubobjects | RF_WillBeLoaded))
    {
        // Object needs loading
        return TryFindStandIn(path, className, contextPtr);
    }
    return assetPtr == nullptr ? TryFindStandIn(path, className, contextPtr) : assetPtr;
}

UObject* sfLoader::TryFindStandIn(FString path, const FString& className, sfObject::SPtr contextPtr)
{
    UObject* outerPtr = GetStandInOuter(contextPtr);
    if (outerPtr == nullptr)
    {
        return nullptr;
    }
    FString name;
    GetStandInName(path, className, name);
    // Try to find a stand-in
    return StaticFindObjectFast(UObject::StaticClass(), outerPtr, *name);
}

void sfLoader::GetStandInName(const FString& path, const FString& className, FString& outName)
{
    // Names containing '.'s don't deserialize properly so replace "." with "+"
    // All delimiters were chosen because they work with soft pointer copy/paste. If you change anything, make sure
    // copy/paste still works with all pointer types.
    outName = "Missing_" + className + "+" + path.Replace(*FString("."), *FString("+"));
}

UObject* sfLoader::GetStandInOuter(sfObject::SPtr contextPtr)
{
    if (contextPtr == nullptr)
    {
        return nullptr;
    }
    // Get the outer for the stand-in by finding a level from the context object or its ancestors.
    UObject* outerPtr = nullptr;
    while (contextPtr != nullptr)
    {
        if (contextPtr->Type() == sfType::Level)
        {
            outerPtr = sfObjectMap::GetUObject(contextPtr);
            break;
        }
        contextPtr = contextPtr->Parent();
    }
    // If no level was found, use the world as the outer.
    if (outerPtr == nullptr)
    {
        outerPtr = GEditor->GetEditorWorldContext().World();
    }
    return outerPtr;
}

void sfLoader::LoadProperty(sfProperty::SPtr propPtr)
{
    sfObject::SPtr objPtr = propPtr->GetContainerObject();
    if (objPtr == nullptr)
    {
        // This happens if the object was deleted or the property was changed before we loaded it.
        return;
    }
    UObject* uobjPtr = sfObjectMap::GetUObject(objPtr);
    if (uobjPtr == nullptr)
    {
        return;
    }
    sfUPropertyInstance uprop = sfPropertyManager::Get().FindUProperty(uobjPtr, propPtr);
    if (uprop.IsValid())
    {
        // This will load the asset
        sfPropertyManager::Get().SetValue(uobjPtr, uprop, propPtr);
    }
}

void sfLoader::OnNewAsset(const FAssetData& assetData)
{
    FString path = assetData.ObjectPath.ToString();
    if (m_standInCounts.Remove(path) == 0)
    {
        return;
    }
    KS::Log::Debug("New asset found for stand-in '" + std::string(TCHAR_TO_UTF8(*path)) + "'.", LOG_CHANNEL);
    FString className = assetData.AssetClass.ToString();
    m_standInsToReplace.Add(className + ";" + path);
    Load(path, className, nullptr);// Load the asset so we find it in the cache when we replace the stand-in
    OnRemoveMissingAsset.Broadcast(path);
    sfReferenceFinder::Get().DelayedUpdateReferences();
}

void sfLoader::ReplaceStandIns()
{
    if (m_standInsToReplace.Num() == 0)
    {
        return;
    }
    if (SceneFusion::Service->Session() == nullptr)
    {
        AsfStandInTrackerActor::ReloadAllStandIns();
        m_standInsToReplace.Empty();
        return;
    }
    TSet<FString> paths;
    std::unordered_set<sfProperty::SPtr> customReferences;
    for (const FString& path : m_standInsToReplace)
    {
        uint32_t pathId = SceneFusion::Service->Session()->GetStringTableId(TCHAR_TO_UTF8(*path));
        paths.Add(path);
        // Build set of properties referencing this stand-in from the property map
        auto iter = m_standInPropertyMap.find(pathId);
        if (iter != m_standInPropertyMap.end())
        {
            for (std::weak_ptr<sfProperty> propPtr : iter->second)
            {
                if (!propPtr.expired())
                {
                    customReferences.emplace(propPtr.lock());
                }
            }
            m_standInPropertyMap.erase(iter);
        }
    }
    int count = 0;
    for (auto iter = sfObjectMap::Begin(); iter != sfObjectMap::End(); ++iter)
    {
        for (auto propIter = iter->first->Property()->Iterate(); propIter.Value() != nullptr; propIter.Next())
        {
            sfProperty::SPtr propPtr = propIter.Value();
            bool isReferenceOnly = ReferencesStandIn(propPtr, paths);
            if (isReferenceOnly || customReferences.find(propPtr) != customReferences.end())
            {
                // Replace the stand-in by triggering a property change event for the property
                if (!isReferenceOnly || 
                    (propPtr->Key().IsValid() && propPtr->Key()->size() > 0 && (*propPtr->Key())[0] == '#'))
                {
                    // If the property starts with '#' or is in the custom references set it is one of our custom
                    // properties. Call the object event dispatcher to run the appropriate property change handler.
                    SceneFusion::ObjectEventDispatcher->OnPropertyChange(propPtr);
                }
                else
                {
                    // It's a uproperty. Let the property manager handle the event and increase the count if successful.
                    sfUPropertyInstance upropInstance = sfPropertyManager::Get().FindUProperty(iter->second, propPtr);
                    if (sfPropertyManager::Get().SetValue(iter->second, upropInstance, propPtr))
                    {
                        count++;
                    }
                }
            }
        }
    }
    m_standInsToReplace.Empty();
    KS::Log::Info("Replaced " + std::to_string(count) + " stand-in reference(s).", LOG_CHANNEL);
}

bool sfLoader::ReferencesStandIn(sfProperty::SPtr propertyPtr, const TSet<FString>& paths)
{
    // Check if the property is a uint value contained in the path ids set.
    if (propertyPtr->Type() == sfProperty::STRING)
    {
        sfStringProperty::SPtr strPtr = propertyPtr->AsString();
        FString path = UTF8_TO_TCHAR(strPtr->GetString()->c_str());
        return paths.Contains(path);
    }
    return false;
}

void sfLoader::LoadDelayedAssets()
{
    if (m_delayedAssets.size() == 0)
    {
        return;
    }
    for (auto iter : m_delayedAssets)
    {
        for (sfProperty::SPtr propPtr : iter.second)
        {
            LoadProperty(propPtr);
        }
    }
    m_delayedAssets.clear();
}

bool sfLoader::IsCreatableAssetType(UClass* classPtr)
{
    while (classPtr != nullptr)
    {
        if (m_createTypes.Contains(classPtr))
        {
            return true;
        }
        classPtr = classPtr->GetSuperClass();
    }
    return false;
}

bool sfLoader::WasCreatedOnLoad(UObject* assetPtr)
{
    return m_createdAssets.Contains(assetPtr);
}

void sfLoader::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
    if (IsUserIdle())
    {
        LoadDelayedAssets();
    }
}

bool sfLoader::HandleMouseButtonDownEvent(FSlateApplication& slateApp, const FPointerEvent& mouseEvent)
{
    if (mouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        m_isMouseDown = true;
    }
    return false;
}

bool sfLoader::HandleMouseButtonUpEvent(FSlateApplication& slateApp, const FPointerEvent& mouseEvent)
{
    if (mouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        m_isMouseDown = false;
    }
    return false;
}

#undef LOG_CHANNEL