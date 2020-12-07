#include "sfGraphNodeTranslator.h"
#include "sfGraphTranslator.h"
#include "../../Public/sfObjectMap.h"
#include "../../Public/Consts.h"
#include "../../Public/sfUnrealUtils.h"
#include "../../Public/sfPropertyUtil.h"
#include "../../Public/sfPropertyManager.h"
#include "../../Public/SceneFusion.h"
#include "../../Public/sfUnrealEventDispatcher.h"
#include "../../Public/Objects/sfReferenceTracker.h"
#include "../../Public/sfConfig.h"
#include "../../Public/sfMissingObjectManager.h"
#include "../Translators/sfUObjectTranslator.h"

#include <Kismet2/BlueprintEditorUtils.h>
#include <K2Node_FunctionEntry.h>
#include <K2Node_CustomEvent.h>
#include <K2Node_AssignDelegate.h>
#include <KismetCompiler.h>
#include <Logging/TokenizedMessage.h>


#define DEFAULT_FLAGS (RF_Transactional | RF_DefaultSubObject | RF_WasLoaded | RF_LoadCompleted)
#define LOG_CHANNEL "sfGraphNodeTranslator"

sfGraphNodeTranslator::sfGraphNodeTranslator()
{
    sfPropertyManager& propertyManager = sfPropertyManager::Get();
    propertyManager.AddPropertyToForceSyncList("BPVariableDescription", "VarType");
}

sfGraphNodeTranslator::~sfGraphNodeTranslator()
{

}

void sfGraphNodeTranslator::Initialize()
{
    if (!sfGraphTranslator::ShouldSyncGraph())
    {
        return;
    }
    m_sessionPtr = SceneFusion::Service->Session();
    m_tickHandle = SceneFusion::Get().OnTick.AddRaw(this, &sfGraphNodeTranslator::Tick);
}

void sfGraphNodeTranslator::CleanUp()
{
    if (!sfGraphTranslator::ShouldSyncGraph())
    {
        return;
    }
    m_dirtyGraphNodes.Empty();
    SceneFusion::Get().OnTick.Remove(m_tickHandle);
}

void sfGraphNodeTranslator::Tick(float deltaTime)
{
    if (!sfGraphTranslator::ShouldSyncGraph())
    {
        return;
    }

    for (auto iter : m_dirtyGraphNodes)
    {
        if (iter.Value != nullptr)
        {
            if (iter.Key->IsPendingKill())
            {
                m_sessionPtr->Delete(iter.Value);
                continue;
            }

            sfPropertyManager::Get().SendPropertyChanges(iter.Key, iter.Value->Property()->AsDict());
            SendLinkChange(iter.Key, iter.Value);
        }
    }
    m_dirtyGraphNodes.Empty();
}

bool sfGraphNodeTranslator::Create(UObject* uobjPtr, sfObject::SPtr& outObjPtr)
{
    outObjPtr = CreateNodeObject(Cast<UEdGraphNode>(uobjPtr), nullptr);
    return outObjPtr != nullptr;
}

sfObject::SPtr sfGraphNodeTranslator::CreateNodeObject(UEdGraphNode* nodePtr, sfObject::SPtr parentPtr)
{
    if (nodePtr == nullptr || nodePtr->IsPendingKill())
    {
        return nullptr;
    }

    // If this is a custom event node, find or create sfObject for the assign delegate node first.
    UK2Node_CustomEvent* eventNodePtr = Cast<UK2Node_CustomEvent>(nodePtr);
    if (eventNodePtr != nullptr)
    {
        // Find the assign delegate node
        UEdGraphPin* outDelegatePinPtr = eventNodePtr->FindPinChecked(UK2Node_CustomEvent::DelegateOutputName);
        for (UEdGraphPin* toPinPtr : outDelegatePinPtr->LinkedTo)
        {
            UEdGraphNode* toNodePtr = toPinPtr->GetOwningNode();
            if (!sfObjectMap::Contains(toNodePtr))
            {
                CreateNodeObject(toNodePtr, parentPtr);
            }
        }
    }

    sfObject::SPtr objPtr = sfObjectMap::GetOrCreateSFObject(nodePtr, sfType::UEdGraphNode);
    if (objPtr->IsSyncing())
    {
        return nullptr;
    }
    sfDictionaryProperty::SPtr propertiesPtr = objPtr->Property()->AsDict();

    propertiesPtr->Set(sfProp::Name, sfPropertyUtil::FromString(nodePtr->GetName()));
    FString className = sfUnrealUtils::ClassToFString(nodePtr->GetClass());
    propertiesPtr->Set(sfProp::Class, sfPropertyUtil::FromString(className));
    sfPropertyManager::Get().CreateProperties(nodePtr, propertiesPtr);

    if (nodePtr != nullptr)
    {
        // Create properties for pins
        sfDictionaryProperty::SPtr pinsProperty = sfDictionaryProperty::Create();
        for (UEdGraphPin* pinPtr : nodePtr->Pins)
        {
            pinsProperty->Set(sfUnrealUtils::FToStdString(pinPtr->PinName.ToString()), CreatePropertyForPin(pinPtr));
        }
        propertiesPtr->Set(sfProp::Pins, pinsProperty);
    }
    if (parentPtr != nullptr)
    {
        if (parentPtr->IsSyncing())
        {
            m_sessionPtr->Create(objPtr, parentPtr, parentPtr->Children().size());
        }
        else
        {
            parentPtr->AddChild(objPtr);
        }
    }
    return objPtr;
}

sfDictionaryProperty::SPtr sfGraphNodeTranslator::CreatePropertyForPin(UEdGraphPin* pinPtr)
{
    sfDictionaryProperty::SPtr pinProperties = sfDictionaryProperty::Create();

    // Direction
    pinProperties->Set(sfProp::Direction, sfValueProperty::Create((uint8_t)pinPtr->Direction));

    // LinkedTo
    pinProperties->Set(sfProp::LinkedTo, CreateLinkedToProperties(pinPtr));

    return pinProperties;
}

sfListProperty::SPtr sfGraphNodeTranslator::CreateLinkedToProperties(UEdGraphPin* pinPtr)
{
    sfListProperty::SPtr linkedToPropPtr = sfListProperty::Create();
    for (UEdGraphPin* toPinPtr : pinPtr->LinkedTo)
    {
        FString nodeName = toPinPtr->GetOwningNode()->GetName();
        FString pinName = toPinPtr->PinName.ToString();
        linkedToPropPtr->Add(sfPropertyUtil::FromString(nodeName + "." + pinName));
    }
    return linkedToPropPtr;
}

void sfGraphNodeTranslator::OnCreate(sfObject::SPtr objPtr, int childIndex)
{
    sfObject::SPtr graphObjPtr = objPtr->Parent();
    if (graphObjPtr == nullptr || graphObjPtr->Type() != sfType::UEdGraph)
    {
        KS::Log::Warning("Graph node object cannot be created without a graph parent.", LOG_CHANNEL);
        return;
    }
    UEdGraph* graphPtr = sfObjectMap::Get<UEdGraph>(graphObjPtr);
    if (graphPtr == nullptr)
    {
        return;
    }

    // Find or create graph node
    sfDictionaryProperty::SPtr propertiesPtr = objPtr->Property()->AsDict();
    FString className = sfPropertyUtil::ToString(propertiesPtr->Get(sfProp::Class));
    UClass* classPtr = sfUnrealUtils::LoadClass(className);
    FString name = sfPropertyUtil::ToString(propertiesPtr->Get(sfProp::Name));
    UEdGraphNode* nodePtr = FindNode(graphPtr, *name);
    //If it is an event node, try to find its assign delegate node's custom event node
    if (nodePtr == nullptr && classPtr == UK2Node_CustomEvent::StaticClass())
    {
        // Get pins property
        sfDictionaryProperty::SPtr pinsProperty = propertiesPtr->Get(sfProp::Pins)->AsDict();
        sfProperty::SPtr delegateOutPinProperty;
        if (pinsProperty->TryGet(
            sfUnrealUtils::FToStdString(UK2Node_CustomEvent::DelegateOutputName.ToString()),
            delegateOutPinProperty))
        {
            //Get linked to property
            sfProperty::SPtr propPtr;
            if (delegateOutPinProperty->AsDict()->TryGet(sfProp::LinkedTo, propPtr))
            {
                TSet<UEdGraphNode*> nodeList;
                sfListProperty::SPtr linkedToPropPtr = propPtr->AsList();
                for (sfProperty::SPtr toPinPropPtr : *linkedToPropPtr)
                {
                    FString pinPath = sfPropertyUtil::ToString(toPinPropPtr);
                    FString nodeName;
                    FString pinName;
                    pinPath.Split(".", &nodeName, &pinName);
                    // Get linked assign delegate node
                    UEdGraphNode* linkedToNodePtr = FindNode(graphPtr, *nodeName);
                    if (linkedToNodePtr != nullptr)
                    {
                        UK2Node_AssignDelegate* assignDelegateNodePtr = Cast<UK2Node_AssignDelegate>(linkedToNodePtr);
                        if (assignDelegateNodePtr != nullptr)
                        {
                            //Get custom event for delegate
                            UEdGraphPin* inDelegatePinPtr = assignDelegateNodePtr->GetDelegatePin();
                            for (UEdGraphPin* toPinPtr : inDelegatePinPtr->LinkedTo)
                            {
                                UEdGraphNode* owningNodePtr = toPinPtr->GetOwningNode();
                                if (owningNodePtr->IsA<UK2Node_CustomEvent>())
                                {
                                    nodePtr = owningNodePtr;
                                    break;
                                }
                            }
                            if (nodePtr != nullptr)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    if (!InitializeNode(nodePtr, name, classPtr, graphPtr, objPtr, propertiesPtr))
    {
        UsfBadNode* badNodePtr = Cast<UsfBadNode>(nodePtr);

        // Serialize all properties
        for (auto iter = propertiesPtr->begin(); iter != propertiesPtr->end(); ++iter)
        {
            std::vector<uint8_t> output;
            sfProperty::Serialize(iter->second, output);
            FName propertyName = FName(UTF8_TO_TCHAR(iter->first->c_str()));
            FsfByteArray& data = badNodePtr->SerializedProperties().FindOrAdd(propertyName);
            data.Data = TArray<uint8>((uint8*)output.data(), output.size());
        }

        sfMissingObjectManager::Get().AddStandIn(badNodePtr);
        KS::Log::Warning("Failed to initialize node " + sfUnrealUtils::FToStdString(name), LOG_CHANNEL);
    }

    // Initialize children
    for (sfObject::SPtr childPtr : objPtr->Children())
    {
        if (childPtr->Type() == sfType::UObject)
        {
            TSharedPtr<sfUObjectTranslator> uobjectTranslatorPtr
                = SceneFusion::Get().GetTranslator<sfUObjectTranslator>(sfType::UObject);
            if (uobjectTranslatorPtr.IsValid())
            {
                uobjectTranslatorPtr->InitializeUObject(nodePtr, childPtr);
            }
        }
        else if (childPtr->Type() == sfType::UEdGraph)
        {
            TSharedPtr<sfGraphTranslator> graphTranslatorPtr = SceneFusion::Get().GetTranslator<sfGraphTranslator>(
                sfType::UEdGraph);
            if (graphTranslatorPtr.IsValid())
            {
                graphTranslatorPtr->OnCreateGraph(childPtr);
            }
        }
        else
        {
            KS::Log::Error("Graph node has unexpected child type " + *childPtr->Type(), LOG_CHANNEL);
        }
    }
}

bool sfGraphNodeTranslator::InitializeNode(
    UEdGraphNode*& inNodePtr,
    FString name,
    UClass* classPtr,
    UEdGraph* graphPtr,
    sfObject::SPtr objPtr,
    sfDictionaryProperty::SPtr propertiesPtr)
{
    bool createNode = inNodePtr == nullptr || inNodePtr->IsA<UsfBadNode>();
    UEdGraphNode* nodePtr = nullptr;
    if (createNode)
    {
        UObject* uobjPtr = StaticFindObjectFast(nullptr, graphPtr, FName(*name));
        if (uobjPtr != nullptr)
        {
            sfUnrealUtils::Rename(uobjPtr, name + " (deleted)");
        }
        nodePtr = NewObject<UEdGraphNode>(graphPtr, classPtr, *name);
        nodePtr->CreateNewGuid();
        nodePtr->SetFlags(RF_Transactional);
    }
    else
    {
        nodePtr = inNodePtr;
    }

    sfPropertyManager::Get().ApplyProperties(nodePtr, propertiesPtr);

    if (createNode)
    {
        nodePtr->AllocateDefaultPins();
        nodePtr->PostPlacedNewNode();
        graphPtr->Modify();
        graphPtr->AddNode(nodePtr, /*bFromUI =*/false, /*bSelectNewNode =*/false);
        nodePtr->ReconstructNode();
    }

    // Initialize pins
    UBlueprint* blueprintPtr = FBlueprintEditorUtils::FindBlueprintForNode(nodePtr);
    if (!InitializePins(nodePtr, propertiesPtr->Get(sfProp::Pins)->AsDict()))
    {
        FBlueprintEditorUtils::RemoveNode(blueprintPtr, nodePtr);
        sfUnrealUtils::Rename(nodePtr, name + " (deleted)");

        if (inNodePtr != nullptr && inNodePtr->IsA<UsfBadNode>())
        {
            inNodePtr->Rename(*name);
            return false;
        }

        // Create the stand-in node
        nodePtr = NewObject<UEdGraphNode>(graphPtr, UsfBadNode::StaticClass(), *name);
        nodePtr->CreateNewGuid();
        nodePtr->SetFlags(RF_Transactional);
        nodePtr->AllocateDefaultPins();
        nodePtr->PostPlacedNewNode();
        graphPtr->Modify();
        graphPtr->AddNode(nodePtr, /*bFromUI =*/false, /*bSelectNewNode =*/false);
        nodePtr->ReconstructNode();

        sfObjectMap::Add(objPtr, nodePtr);
        sfPropertyManager::Get().ApplyProperties(nodePtr, propertiesPtr);

        UsfBadNode* badNodePtr = Cast<UsfBadNode>(nodePtr);
        badNodePtr->PropertiesPtr = propertiesPtr;
        badNodePtr->NodeTypeName = sfUnrealUtils::ClassToFString(classPtr);
        badNodePtr->ShowWarningMessage();
        inNodePtr = nodePtr;
        return false;
    }

    if (createNode && inNodePtr != nullptr)
    {
        FBlueprintEditorUtils::RemoveNode(blueprintPtr, inNodePtr);
    }
    inNodePtr = nodePtr;
    sfObjectMap::Add(objPtr, nodePtr);
    return true;
}

bool sfGraphNodeTranslator::InitializePins(UEdGraphNode* graphNodePtr, sfDictionaryProperty::SPtr pinProperties)
{
    bool success = graphNodePtr->Pins.Num() == pinProperties->Size();
    sfProperty::SPtr pinProperty;
    for (UEdGraphPin* pinPtr : graphNodePtr->Pins)
    {
        if (pinProperties->TryGet(sfUnrealUtils::FToStdString(pinPtr->PinName.ToString()), pinProperty))
        {
            InitializePin(graphNodePtr, pinPtr, pinProperty->AsDict());
        }
        else
        {
            success = false;
        }
    }
    return success;
}

void sfGraphNodeTranslator::InitializePin(
    UEdGraphNode* graphNodePtr,
    UEdGraphPin* pinPtr,
    sfDictionaryProperty::SPtr pinProperties)
{
    sfProperty::SPtr propPtr;
    // Direction
    if (pinProperties->TryGet(sfProp::Direction, propPtr))
    {
        EEdGraphPinDirection direction = (EEdGraphPinDirection)propPtr->AsValue()->GetValue().GetByte();
        pinPtr->Direction = direction;
    }

    // LinkedTo
    if (pinProperties->TryGet(sfProp::LinkedTo, propPtr))
    {
        TSet<UEdGraphNode*> nodeList;
        sfListProperty::SPtr linkedToPropPtr = propPtr->AsList();
        for (sfProperty::SPtr toPinPropPtr : *linkedToPropPtr)
        {
            UEdGraphNode* linkedToNodePtr = SetLink(toPinPropPtr, pinPtr, false);
            if (linkedToNodePtr != nullptr)
            {
                nodeList.Add(pinPtr->GetOwningNode());
                nodeList.Add(linkedToNodePtr);
            }
        }

        // Send all nodes that received a new pin connection a notification
        for (auto iter = nodeList.CreateConstIterator(); iter; ++iter)
        {
            (*iter)->NodeConnectionListChanged();
        }
    }
}

UEdGraphNode* sfGraphNodeTranslator::SetLink(sfProperty::SPtr toPinPropPtr, UEdGraphPin* pinPtr, bool notifyChange)
{
    if (pinPtr == nullptr)
    {
        return nullptr;
    }

    FString pinPath = sfPropertyUtil::ToString(toPinPropPtr);
    FString nodeName;
    FString pinName;
    pinPath.Split(".", &nodeName, &pinName);
    UEdGraphNode* linkedToNodePtr = FindNode(pinPtr->GetOwningNode()->GetGraph(), *nodeName);
    if (linkedToNodePtr == nullptr)
    {
        return nullptr;
    }
    for (UEdGraphPin* pPtr : linkedToNodePtr->Pins)
    {
        if (pPtr->PinName.ToString() == pinName)
        {
            linkedToNodePtr->GetSchema()->TryCreateConnection(pinPtr, pPtr);
            if (notifyChange)
            {
                pinPtr->GetOwningNode()->NodeConnectionListChanged();
                linkedToNodePtr->NodeConnectionListChanged();
            }
            return linkedToNodePtr;
        }
    }
    return nullptr;
}

void sfGraphNodeTranslator::OnDelete(sfObject::SPtr objPtr)
{
    UEdGraphNode* nodePtr = sfObjectMap::Get<UEdGraphNode>(objPtr);
    if (nodePtr != nullptr)
    {
        UBlueprint* blueprintPtr = FBlueprintEditorUtils::FindBlueprintForNode(nodePtr);
        if (blueprintPtr != nullptr)
        {
            FBlueprintEditorUtils::RemoveNode(blueprintPtr, nodePtr);
        }
    }
}

void sfGraphNodeTranslator::OnUObjectModified(sfObject::SPtr objPtr, UObject* uobjPtr)
{
    if (!sfGraphTranslator::ShouldSyncGraph())
    {
        return;
    }

    UEdGraphNode* graphNodePtr = Cast<UEdGraphNode>(uobjPtr);
    if (graphNodePtr != nullptr && !graphNodePtr->IsA<UsfBadNode>())
    {
        m_dirtyGraphNodes.Add(graphNodePtr, objPtr);
    }
}

bool sfGraphNodeTranslator::OnUndoRedo(sfObject::SPtr objPtr, UObject* uobjPtr)
{
    if (objPtr == nullptr)
    {
        return false;
    }
    UEdGraphNode* graphNodePtr = Cast<UEdGraphNode>(uobjPtr);
    if (graphNodePtr == nullptr)
    {
        return false;
    }
    if (!objPtr->IsSyncing() || uobjPtr->IsPendingKill())
    {
        return true;
    }
    sfDictionaryProperty::SPtr propertiesPtr = objPtr->Property()->AsDict();
    sfPropertyManager::Get().SendPropertyChanges(uobjPtr, propertiesPtr);
    return true;
}

void sfGraphNodeTranslator::SendChanges(UEdGraph* graphPtr, sfObject::SPtr graphObjPtr)
{
    TSet<UEdGraphNode*> newNodes(graphPtr->Nodes);
    TMap<UEdGraphNode*, sfObject::SPtr> nodesToCheck;
    auto childIter = graphObjPtr->Children().begin();
    while (childIter != graphObjPtr->Children().end())
    {
        sfObject::SPtr childObjPtr = *childIter++;
        // Send node changes including pin link changes
        UEdGraphNode* nodePtr = sfObjectMap::Get<UEdGraphNode>(childObjPtr);

        if (nodePtr == nullptr || nodePtr->IsPendingKill() || !newNodes.Contains(nodePtr))
        {
            m_sessionPtr->Delete(childObjPtr);
        }
        else
        {
            nodesToCheck.Add(nodePtr, childObjPtr);

            // If node got renamed because of paste or duplicate, rename it back to its old name.
            // Also rename the new node.
            FString name = sfPropertyUtil::ToString(childObjPtr->Property()->AsDict()->Get(sfProp::Name));
            if (name != nodePtr->GetName())
            {
                UObject* duplicatedNodePtr = FindObject<UObject>(graphPtr, *name);
                if (duplicatedNodePtr != nullptr && !sfObjectMap::Contains(duplicatedNodePtr))
                {
                    duplicatedNodePtr->Rename(nullptr, nullptr, REN_DontCreateRedirectors);
                    nodePtr->Rename(*name, nullptr, REN_DontCreateRedirectors);
                }
            }
        }

        if (nodePtr != nullptr)
        {
            newNodes.Remove(nodePtr);
        }
    }

    // Upload new nodes first
    for (UEdGraphNode* nodePtr : newNodes)
    {
        CreateNodeObject(nodePtr, graphObjPtr);
    }

    // Send node property changes
    for (auto iter : nodesToCheck)
    {
        if (iter.Key->IsA<UsfBadNode>())
        {
            // Skip bad nodes
            continue;
        }
        sfPropertyManager::Get().SendPropertyChanges(iter.Key, iter.Value->Property()->AsDict());
        SendLinkChange(iter.Key, iter.Value);
        m_dirtyGraphNodes.Remove(iter.Key);
    }
}

void sfGraphNodeTranslator::SendLinkChange(UEdGraphNode* nodePtr, sfObject::SPtr objPtr)
{
    // Send pin link change
    sfDictionaryProperty::SPtr propertiesPtr = objPtr->Property()->AsDict();
    if (nodePtr->Pins.Num() == 0)
    {
        propertiesPtr->Remove(sfProp::Pins);
        return;
    }
    sfProperty::SPtr propPtr;
    sfDictionaryProperty::SPtr pinsProperty;
    if (!propertiesPtr->TryGet(sfProp::Pins, propPtr))
    {
        pinsProperty = sfDictionaryProperty::Create();
    }
    else
    {
        pinsProperty = propPtr->AsDict();
    }
    for (UEdGraphPin* pinPtr : nodePtr->Pins)
    {
        sfDictionaryProperty::SPtr pinProperties;
        std::string pinName = sfUnrealUtils::FToStdString(pinPtr->PinName.ToString());
        if (!pinsProperty->TryGet(pinName, propPtr))
        {
            pinProperties = sfDictionaryProperty::Create();
            pinsProperty->Set(pinName, pinProperties);
        }
        else
        {
            pinProperties = propPtr->AsDict();
        }

        sfListProperty::SPtr linkedToPropPtr;
        if (!pinProperties->TryGet(sfProp::LinkedTo, propPtr))
        {
            linkedToPropPtr = sfListProperty::Create();
            pinProperties->Set(sfProp::LinkedTo, linkedToPropPtr);
        }
        else
        {
            linkedToPropPtr = propPtr->AsList();
        }

        sfPropertyManager::Get().CopyList(linkedToPropPtr, CreateLinkedToProperties(pinPtr));
    }
}

void sfGraphNodeTranslator::OnPropertyChange(sfProperty::SPtr propertyPtr)
{
    std::string propertyPath = propertyPtr->GetPath();
    std::string pinsPropertyPath(*sfProp::Pins + ".");
    if (propertyPath.compare(0, pinsPropertyPath.length(), pinsPropertyPath) == 0)
    {
        SetLink(propertyPtr, GetPinForLinkProperty(propertyPtr), true);
    }
    else
    {
        sfBaseUObjectTranslator::OnPropertyChange(propertyPtr);
    }
    MarkBlueprintModified(propertyPtr);
}

void sfGraphNodeTranslator::OnListAdd(sfListProperty::SPtr listPtr, int index, int count)
{
    sfProperty::SPtr propertyPtr = listPtr->Get(index);
    std::string pinsPropertyPath(*sfProp::Pins + ".");
    if (propertyPtr->GetPath().compare(0, pinsPropertyPath.length(), pinsPropertyPath) == 0)
    {
        SetLink(propertyPtr, GetPinForLinkProperty(propertyPtr), true);
    }
    else
    {
        sfBaseUObjectTranslator::OnListAdd(listPtr, index, count);
    }
    MarkBlueprintModified(listPtr);
}

void sfGraphNodeTranslator::OnListRemove(sfListProperty::SPtr listPtr, int index, int count)
{
    std::string pinsPropertyPath(*sfProp::Pins + ".");
    if (listPtr->GetPath().compare(0, pinsPropertyPath.length(), pinsPropertyPath) == 0)
    {
        UEdGraphPin* pinPtr = GetPinForLinkProperty(listPtr);
        if (pinPtr != nullptr && pinPtr->LinkedTo.Num() > index)
        {
            pinPtr->GetSchema()->BreakSinglePinLink(pinPtr, pinPtr->LinkedTo[index]);
        }
    }
    else
    {
        sfBaseUObjectTranslator::OnListRemove(listPtr, index, count);
    }
    MarkBlueprintModified(listPtr);
}

UEdGraphPin* sfGraphNodeTranslator::GetPinForLinkProperty(sfProperty::SPtr propertyPtr)
{
    std::string pinsPropertyPath(*sfProp::Pins + ".");
    if (propertyPtr->GetPath().compare(0, pinsPropertyPath.length(), pinsPropertyPath) == 0)
    {
        sfObject::SPtr nodeObjPtr = propertyPtr->GetContainerObject();
        UEdGraphNode* nodePtr = sfObjectMap::Get<UEdGraphNode>(nodeObjPtr);
        if (nodePtr != nullptr)
        {
            while (propertyPtr->GetParentProperty()->Key() != sfProp::Pins)
            {
                propertyPtr = propertyPtr->GetParentProperty();
            }
            return nodePtr->FindPin(FName(propertyPtr->Key()->c_str()));
        };
    }
    return nullptr;
}

UEdGraphNode* sfGraphNodeTranslator::FindNode(UEdGraph* graphPtr, FName nodeName)
{
    for (UEdGraphNode* nodePtr : graphPtr->Nodes)
    {
        if (nodePtr->GetFName() == nodeName)
        {
            return nodePtr;
        }
    }
    return nullptr;
}

void sfGraphNodeTranslator::MarkBlueprintModified(sfProperty::SPtr propertyPtr)
{
    sfObject::SPtr nodeObjPtr = propertyPtr->GetContainerObject();
    UK2Node_FunctionEntry* nodePtr = sfObjectMap::Get<UK2Node_FunctionEntry>(nodeObjPtr);
    if (nodePtr != nullptr)
    {
        UBlueprint* ownerBlueprintPtr = nodePtr->GetBlueprint();
        FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(ownerBlueprintPtr);
    }
}

#undef LOG_CHANNEL
#undef DEFAULT_FLAGS