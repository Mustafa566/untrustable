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
#include <Engine.h>
#include <unordered_map>
#include "sfUPropertyInstance.h"

/**
 * Finds and updates references to stand-ins.
 */
class sfReferenceFinder
{
public:
    /**
     * @return  sfReferenceFinder& singleton instance.
     */
    static sfReferenceFinder& Get();

    /**
     * Handler for pre update references.
     */
    DECLARE_MULTICAST_DELEGATE(PreUpdateReferencesDelegate);

    /**
     * Invoked before updating references.
     */
    PreUpdateReferencesDelegate PreUpdateReferences;

    /**
     * Constructor
     */
    sfReferenceFinder();

    /**
     * Adds a mapping to replace references to one object to another when UpdateAllReferences is called.
     *
     * @param   UObject* oldObjPtr - references to this object will be updated to reference the new object.
     * @param   UObject* newObjPtr to changes references to.
     */
    void AddReplacement(UObject* oldObjPtr, UObject* newObjPtr);

    /**
     * Updates all references to assets in the replacement map after a short delay.
     */
    void DelayedUpdateReferences();

private:
    /**
     * Holds delegates for getting and setting a uobject on a uproperty.
     */
    struct TypeHandler
    {
    public:
        /**
         * Gets a uobject from a property reference.
         *
         * @param   const sfUPropertyInstance& property to get reference from.
         * @param   UObject* uobject containing the property.
         * @return  UObject* uobject referenced by the property.
         */
        typedef std::function<UObject*(const sfUPropertyInstance&, UObject*)> Getter;

        /**
         * Sets a uobject reference property.
         *
         * @param   const sfUPropertyInstance& property to set reference on.
         * @param   UObject* uobject to set reference to.
         */
        typedef std::function<void(const sfUPropertyInstance&, UObject*)> Setter;

        /**
         * Getter
         */
        Getter Get;

        /**
         * Setter
         */
        Setter Set;

        /**
         * Constructor
         */
        TypeHandler() :
            Get{ nullptr },
            Set{ nullptr }
        {

        }

        /**
         * Constructor
         *
         * @param   Getter getter
         * @param   Setter setter
         */
        TypeHandler(Getter getter, Setter setter) :
            Get{ getter },
            Set{ setter }
        {

        }

        /**
         * Constructor
         *
         * @param   Getter getter
         */
        TypeHandler(Getter getter) :
            Get{ getter },
            Set{ nullptr }
        {

        }
    };

    // References to keys will be updated to reference the value.
    TMap<UObject*, UObject*> m_replacementMap;
    TSet<UObject*> m_exploredObjects;
    TSet<AActor*> m_changedActors;
    std::unordered_map<TypeHash, TypeHandler> m_typeHandlers;
    FDelegateHandle m_tickHandle;
    float m_updateTimer;

    /**
     * Updates references if the replaceTimer reaches 0.
     *
     * @param   float deltaTime in seconds since the last tick.
     * @return  bool true if this should be called again next tick.
     */
    bool Tick(float deltaTime);

    /**
     * Crawls all syncable properties of all open levels, their actors, components, and referenced objects, and updates
     * any references to objects in the replacement map.
     */
    void UpdateAllReferences();

    /**
     * Updates references in a level, its actors, components, and referenced objects.
     *
     * @param   ULevel* levelPtr to update references in.
     */
    void UpdateReferencesInLevel(ULevel* levelPtr);

    /**
     * Updates references in an actor, its components and referenced objects.
     *
     * @param   AActor* actorPtr to update references in.
     */
    void UpdateReferencesInActor(AActor* actorPtr);

    /**
     * Updates references in an object, and objects referenced by that object.
     *
     * @param   UObject* uobjPtr to update references in.
     */
    void UpdateReferences(UObject* uobjPtr);

    /**
     * Updates a property if it references or contains references to objects in the replacement map.
     *
     * @param sfUPropertyInstance& upropInstance to update references in.
     * @param UObject* uobjPtr containing the property.
     */
    void UpdateProperty(sfUPropertyInstance& upropInstance, UObject* uobjPtr);
};