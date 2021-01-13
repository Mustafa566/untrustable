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

#include "../../Public/Translators/sfBaseUObjectTranslator.h"

#include <sfSession.h>

#include <CoreMinimal.h>
#include <K2Node_CallFunction.h>

using namespace KS::SceneFusion2;

/**
 * Manages syncing of graphs.
 */
class sfGraphTranslator : public sfBaseUObjectTranslator
{
public:
    /**
     * Constructor
     */
    sfGraphTranslator();

    /**
     * Destructor
     */
    virtual ~sfGraphTranslator();

    /**
     * Initialization. Called after connecting to a session.
     */
    virtual void Initialize() override;

    /**
     * Deinitialization. Called after disconnecting from a session.
     */
    virtual void CleanUp() override;

    /**
     * Creates an sfObject for the given graph.
     *
     * @param   UEdGraph* graphPtr
     * @return  sfObject::SPtr
     */
    sfObject::SPtr CreateGraphObject(UEdGraph* graphPtr);

    /**
     * Called when a graph object is created.
     *
     * @param   sfObject::SPtr objPtr that was created.
     */
    void OnCreateGraph(sfObject::SPtr objPtr);

    /**
     * @return  bool true if both SyncBlueprint and SyncBlueprintGraph are enabled and
     * the object limit of blueprint type is not 0.
     */
    static bool ShouldSyncGraph();

protected:
    /**
     * Creates an sfObject for a uobject.
     *
     * @param   UObject* uobjPtr to create sfObject for.
     * @param   sfObject::SPtr* outObjPtr created for the uobject.
     * @return  bool true if the uobject was handled by this manager.
     */
    virtual bool Create(UObject* uobjPtr, sfObject::SPtr& outObjPtr) override;

    /**
     * Called when a graph is created by another user.
     *
     * @param   sfObject::SPtr objPtr that was created.
     * @param   int childIndex of new object. -1 if object is a root.
     */
    virtual void OnCreate(sfObject::SPtr objPtr, int childIndex) override;

    /**
     * Called when a graph is deleted by another user.
     *
     * @param   sfObject::SPtr objPtr that was deleted.
     */
    virtual void OnDelete(sfObject::SPtr objPtr) override;

    /**
     * Called when a uobject is modified. Sends node graph changes to server.
     *
     * @param   sfObject::SPtr objPtr for the uobject that was modified.
     * @param   UObject* uobjPtr that was modified.
     */
    virtual void OnUObjectModified(sfObject::SPtr objPtr, UObject* uobjPtr) override;

private:
    sfSession::SPtr m_sessionPtr;
    TMap<UEdGraph*, sfObject::SPtr> m_dirtyGraphs;
    FDelegateHandle m_tickHandle;

    /**
     * Updates the blueprint translator.
     *
     * @param   float deltaTime in seconds since the last tick.
     */
    void Tick(float deltaTime);
};
