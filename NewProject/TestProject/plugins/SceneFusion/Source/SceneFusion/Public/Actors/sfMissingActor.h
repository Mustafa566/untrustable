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

#include "../sfMissingObject.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "sfMissingActor.generated.h"

/**
 * Actor class to use as a stand-in for missing actor classes.
 */
UCLASS()
class AsfMissingActor : public AActor, public IsfMissingObject
{
	GENERATED_BODY()
	
public:	
    // Name of the missing actor class
    UPROPERTY(VisibleAnywhere)
    FString ClassName;

    // We have to set component creation method to instance or components will not save properly, so we store the real
    // creation method in this map.
    UPROPERTY()
    TMap<UActorComponent*, EComponentCreationMethod> ComponentCreationMethods;

    // Components with the flag RF_DefaultSubObject will not save properly, clear the flag and store the components
    // that should have it.
    UPROPERTY()
    TArray<UActorComponent*> DefaultSubComponents;

    UPROPERTY()
    TMap<FName, FsfByteArray> SerializedData;

    UPROPERTY()
    TMap<uint32, UObject*> ReferenceMap;

    /**
     * @return  FString& name of the missing actor class or blueprint path.
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
     * Replaces this actor with a new one using the data from this actor's sfObject.
     *
     * @return  bool - true if the missing actor is successfully reloaded
     */
    virtual bool Reload() override;

    /**
     * Called after the actor is loaded. Adds the actor to the missing object manager.
     */
    virtual void PostLoad() override;

    /**
     * Called after the actor is duplicated. Adds the actor to the missing object manager.
     *
     * @param   bool bDuplicateForPie - true if the actor was duplicated because we are about to play in editor, in
     *          which case this function does nothing.
     */
    virtual void PostDuplicate(bool bDuplicateForPie) override;
	
    /**
     * Called before destroying the object. Removes the actor from the missing object manager.
     */
    virtual void BeginDestroy() override;
};
