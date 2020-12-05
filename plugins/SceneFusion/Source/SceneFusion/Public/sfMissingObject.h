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

#include "sfByteArray.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "sfMissingObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UsfMissingObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for missing object stand-ins.
 */
class IsfMissingObject
{
	GENERATED_BODY()
public:
    /**
     * @return  FString& name of the missing class or blueprint path.
     */
    virtual FString& MissingClass() = 0;

    /**
     * @return  TMap<FName, FsfByteArray>& Serialized property data for missing properties.
     */
    virtual TMap<FName, FsfByteArray>& SerializedProperties() = 0;

    /**
     * @return TMap<uint32, UObject*>& Maps sfObject ids to UObjects referenced in the serialized property data.
     */
    virtual TMap<uint32, UObject*>& References() = 0;

    /**
     * Replaces this object with with a new instance of the correct class if it is available.
     *
     * @return  bool - true if the missing object is successfully reloaded
     */
    virtual bool Reload() = 0;
	
};