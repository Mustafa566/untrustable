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
#include "Components/ActorComponent.h"
#include "sfMissingComponent.generated.h"


/**
 * Component class to use as a stand-in for missing non-scene component classes.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UsfMissingComponent : public UActorComponent, public IsfMissingObject
{
	GENERATED_BODY()

public:	
    // Name of the missing component class
    UPROPERTY(VisibleAnywhere, Category = "Scene Fusion")
    FString ClassName;

    UPROPERTY()
    TMap<FName, FsfByteArray> SerializedData;

    UPROPERTY()
    TMap<uint32, UObject*> ReferenceMap;
	
    /**
     * @return  FString& name of the missing component class or blueprint path.
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
     * Replaces this componet with a new one using the data from this component's sfObject.
     *
     * @return  bool - true if the missing component is successfully reloaded
     */
    virtual bool Reload() override;

    /**
     * Called when the component is created. Adds the component to the missing object manager.
     */
    virtual void OnComponentCreated() override;

    /**
     * Called after the component is loaded. Adds the component to the missing object manager.
     */
    virtual void PostLoad() override;

    /**
     * Called before destroying the object. Removes the component from the missing object manager.
     */
    virtual void BeginDestroy() override;


};
