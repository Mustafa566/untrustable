#include "sfGraphTranslator.h"
#include "../../Public/sfObjectMap.h"
#include "../../Public/Consts.h"
#include "../../Public/sfUnrealUtils.h"
#include "../../Public/sfPropertyUtil.h"
#include "../../Public/sfPropertyManager.h"
#include "../../Public/SceneFusion.h"
#include "../../Public/Objects/sfReferenceTracker.h"
#include "../../Public/sfConfig.h"

#include <Kismet2/BlueprintEditorUtils.h>


#define DEFAULT_FLAGS (RF_Transactional | RF_DefaultSubObject | RF_WasLoaded | RF_LoadCompleted)
#define LOG_CHANNEL "sfGraphTranslator"

sfGraphTranslator::sfGraphTranslator()
{

}

sfGraphTranslator::~sfGraphTranslator()
{

}

void sfGraphTranslator::Initialize()
{
    if (!ShouldSyncGraph())
    {
        return;
    }
    m_sessionPtr = SceneFusion::Service->Session();
    m_tickHandle = SceneFusion::Get().OnTick.AddRaw(this, &sfGraphTranslator::Tick);
}

void sfGraphTranslator::CleanUp()
{
    if (!ShouldSyncGraph())
    {
        return;
    }
    m_dirtyGraphs.Empty();
    SceneFusion::Get().OnTick.Remove(m_tickHandle);
}

void sfGraphTranslator::Tick(float deltaTime)
{
    if (!ShouldSyncGraph())
    {
        return;
    }

    for (auto iter : m_dirtyGraphs)
    {
        if (iter.Value != nullptr)
        {
            if (iter.Key->IsPendingKill())
            {
                m_sessionPtr->Delete(iter.Value);
                continue;
            }

            sfPropertyManager::Get().SendPropertyChanges(iter.Key, iter.Value->Property()->AsDict());
            TSharedPtr<sfGraphNodeTranslator> graphNodeTranslator
                = SceneFusion::Get().GetTranslator<sfGraphNodeTranslator>(sfType::UEdGraphNode);
            if (graphNodeTranslator.IsValid())
            {
                graphNodeTranslator->SendChanges(iter.Key, iter.Value);
            }
        }
    }
    m_dirtyGraphs.Empty();
}

bool sfGraphTranslator::Create(UObject* uobjPtr, sfObject::SPtr& outObjPtr)
{
    UEdGraph* graphPtr = Cast<UEdGraph>(uobjPtr);
    if (graphPtr == nullptr)
    {
        return false;
    }
    outObjPtr = CreateGraphObject(graphPtr);
    return outObjPtr != nullptr;
}

void sfGraphTranslator::OnCreate(sfObject::SPtr objPtr, int childIndex)
{
    if (objPtr->Type() == sfType::UEdGraph)
    {
        OnCreateGraph(objPtr);
    }
}

void sfGraphTranslator::OnDelete(sfObject::SPtr objPtr)
{

}

void sfGraphTranslator::OnUObjectModified(sfObject::SPtr objPtr, UObject* uobjPtr)
{
    if (!ShouldSyncGraph())
    {
        return;
    }

    UEdGraph* modifiedGraphPtr = Cast<UEdGraph>(uobjPtr);
    if (modifiedGraphPtr != nullptr)
    {
        m_dirtyGraphs.Add(modifiedGraphPtr, objPtr);
        return;
    }

    UBlueprint* blueprintPtr = Cast<UBlueprint>(uobjPtr);
    if (blueprintPtr != nullptr)
    {
        TArray<UEdGraph*> allGraphs;
        blueprintPtr->GetAllGraphs(allGraphs);
        for (UEdGraph* graphPtr : allGraphs)
        {
            // Don't create subgraph
            if (graphPtr->GetOuter() != blueprintPtr)
            {
                continue;
            }

            sfObject::SPtr graphObjPtr = sfObjectMap::GetSFObject(graphPtr);
            if (graphObjPtr != nullptr)
            {
                m_dirtyGraphs.Add(graphPtr, graphObjPtr);
            }
        }
    }
}

sfObject::SPtr sfGraphTranslator::CreateGraphObject(UEdGraph* graphPtr)
{
    // Create function graph object
    sfObject::SPtr objPtr = sfObjectMap::GetOrCreateSFObject(graphPtr, sfType::UEdGraph);
    if (objPtr->IsSyncing())
    {
        return nullptr;
    }
    sfDictionaryProperty::SPtr propertiesPtr = objPtr->Property()->AsDict();
    if (!propertiesPtr->HasKey(sfProp::Name)) // Not already initialized
    {
        propertiesPtr->Set(sfProp::Name, sfPropertyUtil::FromString(graphPtr->GetName()));;

        // Create nodes
        for (UEdGraphNode* nodePtr : graphPtr->Nodes)
        {
            TSharedPtr<sfGraphNodeTranslator> graphNodeTranslator
                = SceneFusion::Get().GetTranslator<sfGraphNodeTranslator>(sfType::UEdGraphNode);
            if (graphNodeTranslator.IsValid())
            {
                sfObject::SPtr nodeObjPtr = graphNodeTranslator->CreateNodeObject(nodePtr, objPtr);
            }
        }
    }

    // Find or create parent
    sfObject::SPtr parentPtr = sfObjectMap::GetSFObject(graphPtr->GetOuter());
    if (parentPtr == nullptr)
    {
        parentPtr = SceneFusion::ObjectEventDispatcher->Create(graphPtr->GetOuter());
        if (parentPtr == nullptr)
        {
            return objPtr;
        }
    }
    if (parentPtr->IsSyncing())
    {
        m_sessionPtr->Create(objPtr, parentPtr, parentPtr->Children().size());
    }
    else
    {
        parentPtr->AddChild(objPtr);
    }

    return objPtr;
}

void sfGraphTranslator::OnCreateGraph(sfObject::SPtr objPtr)
{
    sfObject::SPtr rootPtr = objPtr;
    while (rootPtr->Parent() != nullptr)
    {
        rootPtr = rootPtr->Parent();
    }
    UBlueprint* blueprintPtr = sfObjectMap::Get<UBlueprint>(rootPtr);
    if (blueprintPtr == nullptr)
    {
        return;
    }

    // Find or create graph
    sfDictionaryProperty::SPtr propertiesPtr = objPtr->Property()->AsDict();
    FString name = sfPropertyUtil::ToString(propertiesPtr->Get(sfProp::Name));
    TArray<UEdGraph*> allGraphs;
    blueprintPtr->GetAllGraphs(allGraphs);
    UEdGraph* graphPtr = nullptr;
    for (UEdGraph* tPtr : allGraphs)
    {
        if (tPtr->GetName() == name)
        {
            graphPtr = tPtr;
        }
    }
    if (graphPtr == nullptr)
    {
        graphPtr = FBlueprintEditorUtils::CreateNewGraph(
            blueprintPtr, FName(*name), UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
    }
    if (graphPtr == nullptr)
    {
        return;
    }
    sfObjectMap::Add(objPtr, graphPtr);

    for (sfObject::SPtr childPtr : objPtr->Children())
    {
        TSharedPtr<sfGraphNodeTranslator> graphNodeTranslator
            = SceneFusion::Get().GetTranslator<sfGraphNodeTranslator>(sfType::UEdGraphNode);
        if (graphNodeTranslator.IsValid())
        {
            graphNodeTranslator->OnCreate(childPtr, 0);
        }
    }
}

bool sfGraphTranslator::ShouldSyncGraph()
{
    return sfConfig::Get().SyncBlueprint &&
        SceneFusion::Get().Service->Session()->GetObjectLimit(sfType::Blueprint) != 0 &&
        sfConfig::Get().SyncBlueprintGraph;
}

#undef LOG_CHANNEL
#undef DEFAULT_FLAGS