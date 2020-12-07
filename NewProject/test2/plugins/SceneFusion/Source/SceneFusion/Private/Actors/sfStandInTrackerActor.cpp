#include "sfStandInTrackerActor.h"
#include "../../Public/sfLoader.h"
#include "../../Public/sfReferenceFinder.h"
#include "../../Public/SceneFusion.h"
#include "../../Public/Translators/sfActorTranslator.h"

TMap<ULevel*, AsfStandInTrackerActor*> AsfStandInTrackerActor::m_trackerMap;

AsfStandInTrackerActor* AsfStandInTrackerActor::Get(ULevel* levelPtr, bool create)
{
    AsfStandInTrackerActor* actorPtr = m_trackerMap.FindRef(levelPtr);
    if (actorPtr != nullptr || !create)
    {
        return actorPtr;
    }
    UWorld* worldPtr = GEditor->GetEditorWorldContext().World();
    FActorSpawnParameters spawnInfo;
    spawnInfo.OverrideLevel = levelPtr;
    actorPtr = worldPtr->SpawnActor<AsfStandInTrackerActor>(FVector::ZeroVector, FRotator::ZeroRotator, spawnInfo);
    if (actorPtr != nullptr)
    {
        m_trackerMap.Add(levelPtr, actorPtr);
    }
    return actorPtr;
}

void AsfStandInTrackerActor::ReloadAllStandIns()
{
    for (auto iter : m_trackerMap)
    {
        iter.Value->ReloadStandIns();
    }
}

bool AsfStandInTrackerActor::IsSelectable() const
{
    return false;
}

bool AsfStandInTrackerActor::IsListedInSceneOutliner() const
{
    return false;
}

void AsfStandInTrackerActor::PostLoad()
{
    Super::PostLoad();
    if (!m_trackerMap.Contains(GetLevel()))
    {
        m_trackerMap.Add(GetLevel(), this);
    }
    if (ReloadStandIns())
    {
        sfReferenceFinder::Get().DelayedUpdateReferences();
    }
    for (TWeakObjectPtr<UObject> weakPtr : StandIns)
    {
        UObject* standInPtr = weakPtr.Get();
        if (standInPtr != nullptr)
        {
            sfLoader::Get().AddStandIn(standInPtr);
            m_missingPaths.Add(sfLoader::Get().GetPathFromStandIn(standInPtr));
        }
    }
}

void AsfStandInTrackerActor::BeginDestroy()
{
    Super::BeginDestroy();
    for (const FString& path : m_missingPaths)
    {
        sfLoader::Get().RemoveStandIn(path);
    }
    sfReferenceFinder::Get().PreUpdateReferences.Remove(m_preUpdateReferencesHandle);
    if (m_trackerMap.FindRef(GetLevel()) == this)
    {
        m_trackerMap.Remove(GetLevel());
    }
}

void AsfStandInTrackerActor::Add(const FString& path, UObject* standInPtr)
{
    StandIns.Add(standInPtr);
    m_missingPaths.Add(path);
}

bool AsfStandInTrackerActor::Contains(UObject* uobjPtr)
{
    for (TWeakObjectPtr<UObject> weakPtr : StandIns)
    {
        if (weakPtr.Get() == uobjPtr)
        {
            return true;
        }
    }
    return false;
}

bool AsfStandInTrackerActor::ReloadStandIns()
{
    bool replace = false;
    for (int i = StandIns.Num() - 1; i >= 0; i--)
    {
        UObject* standInPtr = StandIns[i].Get();
        if (standInPtr != nullptr)
        {
            FString str = sfLoader::Get().GetPathFromStandIn(standInPtr);
            FString path, className;
            if (str.Split(";", &className, &path))
            {
                UObject* assetPtr = sfLoader::Get().Load(path, className, nullptr, true);
                if (assetPtr != nullptr)
                {
                    if (SceneFusion::Service->Session() == nullptr)
                    {
                        sfReferenceFinder::Get().AddReplacement(standInPtr, assetPtr);
                        replace = true;
                    }
                }
                else
                {
                    continue;
                }
            }
        }
        StandIns.RemoveAt(i);
    }
    if (StandIns.Num() == 0)
    {
        // Destroying the actor now if this was called from PostLoad will crash Unreal, so wait until we replace
        // references
        m_preUpdateReferencesHandle = sfReferenceFinder::Get().PreUpdateReferences.AddLambda([this]()
        {
            TSharedPtr<sfActorTranslator> actorTranslatorPtr = SceneFusion::Get().GetTranslator<sfActorTranslator>(
                sfType::Actor);
            if (actorTranslatorPtr.IsValid())
            {
                actorTranslatorPtr->DestroyActor(this);
            }
        });
    }
    return replace;
}