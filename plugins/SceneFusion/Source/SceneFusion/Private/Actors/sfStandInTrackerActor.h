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

#include <CoreMinimal.h>
#include "sfStandInTrackerActor.generated.h"

/**
 * Stores a weak pointer array of non-actor non-component asset stand-ins referenced in a level so we quickly know
 * which missing assets are referenced in a level when we load it.
 */
UCLASS()
class AsfStandInTrackerActor : public AActor
{
    GENERATED_BODY()

public:
    /**
     * Stand-ins referenced in the level.
     */
    UPROPERTY()
    TArray<TWeakObjectPtr<UObject>> StandIns;

    /**
     * Gets the stand-in tracker for a level.
     * 
     * @param   ULevel* levelPtr to get stand-in tracker for.
     * @param   bool create - if true, creates a stand-in tracker if non exists.
     * @return  AsfStandInTrackerActor*
     */
    static AsfStandInTrackerActor* Get(ULevel* levelPtr, bool create = true);

    /**
     * Replaces all stand-ins with assets that are available in all open levels.
     */
    static void ReloadAllStandIns();

    /**
     * Overrides IsSelectable to always return false.
     *
     * @return  bool
     */
    virtual bool IsSelectable() const override;

    /**
     * Overrides IsListedInSceneOutliner to always return false.
     *
     * @return  bool
     */
    virtual bool IsListedInSceneOutliner() const override final;

    /**
     * Called when the actor is loaded. Replaces stand-ins with available assets if not in a session.
     */
    virtual void PostLoad() override;

    /**
     * Called when the actor is destroyed.
     */
    virtual void BeginDestroy() override;

    /**
     * Adds a stand-in to the tracker's array of stand-ins.
     *
     * @param   FString path to missing asset the stand-in is for.
     * @param   UObject* standInPtr
     */
    void Add(const FString& path, UObject* standInPtr);

    /**
     * Checks if an object is in the tracker's array of stand-ins.
     *
     * @param   UObject* uobjPtr
     * @return  bool true if the object is in the array of stand-ins.
     */
    bool Contains(UObject* uobjPtr);

private:
    static TMap<ULevel*, AsfStandInTrackerActor*> m_trackerMap;

    // We need to store the missing asset paths separately because the StandIns array weak pointers are null when
    // BeginDestroy is called
    TSet<FString> m_missingPaths;
    FDelegateHandle m_preUpdateReferencesHandle;

    /**
     * Removes stand-ins from the array that have assets available. If outside a session, tells the sfReferenceFinder
     * to replace stand-in references with the correct asset.
     *
     * @param   bool true if replacement mappings were added to the sfReferenceFinder
     */
    bool ReloadStandIns();

};