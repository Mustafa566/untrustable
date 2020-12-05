#include "sfBadNode.h"
#include "../Translators/sfGraphNodeTranslator.h"
#include "../../Public/sfPropertyUtil.h"
#include "../../Public/sfMissingObjectManager.h"
#include "../../Public/sfUnrealUtils.h"

#include <K2Node_DynamicCast.h>
#include <K2Node_CallFunction.h>

FText UsfBadNode::GetNodeTitle(ENodeTitleType::Type titleType) const
{
    return FText::FromString("Bad Node");
}

bool UsfBadNode::IsNodePure() const
{
    return true;
}

void UsfBadNode::ShowWarningMessage()
{
    if (m_warningMessage.IsEmpty())
    {
        if (NodeTypeName.IsEmpty())
        {
            return;
        }

        if (PropertiesPtr == nullptr)
        {
            PropertiesPtr = sfDictionaryProperty::Create();
            sfMissingObjectManager::Get().DeserializeProperties(this, PropertiesPtr);
        }

        UClass* classPtr = sfUnrealUtils::LoadClass(NodeTypeName);
        if (classPtr == UK2Node_CallFunction::StaticClass())
        {
            FString functionName = sfPropertyUtil::ToString(
                PropertiesPtr->Get("FunctionReference")->AsDict()->Get("MemberName"));
            m_warningMessage = "The function '" + functionName + "' is missing.";
            MissingClassName = functionName;
        }
        else if (classPtr == UK2Node_DynamicCast::StaticClass())
        {
            FString className = sfPropertyUtil::ToString(PropertiesPtr->Get("TargetType"));
            m_warningMessage = "The class '" + className + "' is missing.";
            MissingClassName = className;
        }
        else
        {
            m_warningMessage = "Unknown error.";
        }
        m_warningMessage += " Please make sure your project is up to date.";
    }
    ErrorMsg = m_warningMessage;
    ErrorType = EMessageSeverity::Warning;
    bHasCompilerMessage = true;
}

FString& UsfBadNode::MissingClass()
{
    return MissingClassName;
}

TMap<FName, FsfByteArray>& UsfBadNode::SerializedProperties()
{
    return SerializedData;
}

TMap<uint32, UObject*>& UsfBadNode::References()
{
    return ReferenceMap;
}

bool UsfBadNode::Reload()
{
    TSharedPtr<sfGraphNodeTranslator> graphNodeTranslatorPtr =
        SceneFusion::Get().GetTranslator<sfGraphNodeTranslator>(sfType::UEdGraphNode);
    // Recreate node
    UEdGraphNode* nodePtr = Cast<UEdGraphNode>(this);
    UClass* classPtr = sfUnrealUtils::LoadClass(NodeTypeName);
    if (PropertiesPtr == nullptr)
    {
        PropertiesPtr = sfDictionaryProperty::Create();
        sfMissingObjectManager::Get().DeserializeProperties(this, PropertiesPtr);
    }
    if (!graphNodeTranslatorPtr->InitializeNode(
            nodePtr,
            GetName(),
            classPtr,
            GetGraph(),
            PropertiesPtr->GetContainerObject(),
            PropertiesPtr))
    {
        ShowWarningMessage();
        return false;
    }
    return true;
}

void UsfBadNode::PostLoad()
{
    Super::PostLoad();
    if (!MissingClassName.IsEmpty())
    {
        ShowWarningMessage();
        sfMissingObjectManager::Get().AddStandIn(this);
    }
}

void UsfBadNode::PostDuplicate(bool bDuplicateForPie)
{
    Super::PostDuplicate(bDuplicateForPie);
    if (!MissingClassName.IsEmpty() && !bDuplicateForPie)
    {
        sfMissingObjectManager::Get().AddStandIn(this);
    }
}

void UsfBadNode::BeginDestroy()
{
    sfMissingObjectManager::Get().RemoveStandIn(this);
    Super::BeginDestroy();
}