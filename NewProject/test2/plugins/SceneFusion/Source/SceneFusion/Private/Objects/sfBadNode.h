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

#include "../../Public/sfMissingObject.h"

#include <sfDictionaryProperty.h>

#include <CoreMinimal.h>
#include <K2Node.h>

#include "sfBadNode.generated.h"

using namespace KS::SceneFusion2;

 /**
  * Represents a node that is in a bad state.
  */
UCLASS()
class UsfBadNode : public UK2Node, public IsfMissingObject
{
    GENERATED_BODY()

    /**
     * Gets the node tile.
     *
     * @param   ENodeTitleType::Type titleType
     * @return  FText
     */
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override;

    /**
     * Returns whether this node is considered 'pure' by the compiler
     *
     * @return  bool
     */
    virtual bool IsNodePure() const;

private:
    FString m_warningMessage;

public:
    sfDictionaryProperty::SPtr PropertiesPtr;

    UPROPERTY()
    FString NodeTypeName;

    UPROPERTY(VisibleAnywhere)
    FString MissingClassName;

    UPROPERTY()
    TMap<FName, FsfByteArray> SerializedData;

    UPROPERTY()
    TMap<uint32, UObject*> ReferenceMap;

    /**
     * Shows the warning message on the node in the blueprint editor.
     */
    void ShowWarningMessage();

    /**
     * @return  FString& name of the missing class or function.
     */
    virtual FString& MissingClass() override;

    /**
     * @return  TMap<FName, FsfByteArray>& Serialized property data for missing properties.
     */
    virtual TMap<FName, FsfByteArray>& SerializedProperties() override;

    /**
     * @return TMap<uint32, UObject*>& Maps sfObject ids to UObjects referenced in the serialized property data.
     */
    virtual TMap<uint32, UObject*>& References() override;

    /**
     * Replaces this node with a new one using the data from this node's sfObject.
     *
     * @return  bool - true if the bad node is successfully reloaded
     */
    virtual bool Reload() override;

    /**
     * Called after the actor is loaded. Adds the node to the missing object manager.
     */
    virtual void PostLoad() override;

    /**
     * Called after the node is duplicated. Adds the node to the missing object manager.
     *
     * @param   bool bDuplicateForPie - true if the node was duplicated because we are about to play in editor, in
     *          which case this function does nothing.
     */
    virtual void PostDuplicate(bool bDuplicateForPie) override;

    /**
     * Called before destroying the object. Removes the node from the missing object manager.
     */
    virtual void BeginDestroy() override;
};