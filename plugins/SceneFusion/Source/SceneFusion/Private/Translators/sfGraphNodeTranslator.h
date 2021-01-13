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
#include "../Objects/sfBadNode.h"

#include <sfSession.h>

#include <CoreMinimal.h>
#include <K2Node_CallFunction.h>

using namespace KS::SceneFusion2;

/**
 * Manages syncing of uobjects.
 */
class sfGraphNodeTranslator : public sfBaseUObjectTranslator
{
    friend class sfBaseUObjectTranslator;
    friend class sfGraphTranslator;

public:
    /**
     * Constructor
     */
    sfGraphNodeTranslator();

    /**
     * Destructor
     */
    virtual ~sfGraphNodeTranslator();

    /**
     * Initialization. Called after connecting to a session.
     */
    virtual void Initialize() override;

    /**
     * Deinitialization. Called after disconnecting from a session.
     */
    virtual void CleanUp() override;

    /**
     * Checks and sends graph node changes to server.
     */
    void SendChanges(UEdGraph* graphPtr, sfObject::SPtr graphObjPtr);

    /**
     * Initializes the given node.
     *
     * @param
     */
    bool InitializeNode(
        UEdGraphNode*& inNodePtr,
        FString name,
        UClass* classPtr,
        UEdGraph* graphPtr,
        sfObject::SPtr objPtr,
        sfDictionaryProperty::SPtr propertiesPtr);

protected:
    /**
     * Creates an sfObject for a uobject.
     *
     * @param   UObject* uobjPtr to create sfObject for.
     * @param   sfObject::SPtr& outObjPtr created for the uobject.
     * @return  bool true if the uobject was handled by this manager.
     */
    virtual bool Create(UObject* uobjPtr, sfObject::SPtr& outObjPtr) override;

    /**
     * Creates an sfObject for the given UEdGraphNode.
     *
     * @param   UObject* uobjPtr to create sfObject for.
     * @param   sfObject::SPtr parentPtr - if not null, add the created sfObject as its child.
     * @return  sfObject::SPtr
     */
    sfObject::SPtr CreateNodeObject(UEdGraphNode* nodePtr, sfObject::SPtr parentPtr);

    /**
     * Called when a graph node is created by another user.
     *
     * @param   sfObject::SPtr objPtr that was created.
     * @param   int childIndex of new object. -1 if object is a root.
     */
    virtual void OnCreate(sfObject::SPtr objPtr, int childIndex) override;

    /**
     * Called when a graph node is deleted by another user.
     *
     * @param   sfObject::SPtr objPtr that was deleted.
     */
    virtual void OnDelete(sfObject::SPtr objPtr) override;

    /**
     * Called when a uobject is modified. Sends node changes to server.
     *
     * @param   sfObject::SPtr objPtr for the uobject that was modified.
     * @param   UObject* uobjPtr that was modified.
     */
    virtual void OnUObjectModified(sfObject::SPtr objPtr, UObject* uobjPtr) override;

    /**
     * Called when an object is modified by an undo or redo transaction.
     *
     * @param   sfObject::SPtr objPtr for the uobject that was modified. nullptr if the uobjPtr is not synced.
     * @param   UObject* uobjPtr that was modified.
     * @return  bool true if event was handled and need not be processed by other handlers.
     */
    virtual bool OnUndoRedo(sfObject::SPtr objPtr, UObject* uobjPtr) override;

private:
    sfSession::SPtr m_sessionPtr;
    TMap<UEdGraphNode*, sfObject::SPtr> m_dirtyGraphNodes;
    FDelegateHandle m_tickHandle;

    /**
     * Updates the graph node translator.
     *
     * @param   float deltaTime in seconds since the last tick.
     */
    void Tick(float deltaTime);

    /**
     * Called when a graph node property changes.
     *
     * @param   sfProperty::SPtr propertyPtr that changed.
     */
    virtual void OnPropertyChange(sfProperty::SPtr propertyPtr) override;

    /**
     * Called when a field is removed from a dictionary property.
     *
     * @param   sfDictionaryProperty::SPtr dictPtr the field was removed from.
     * @param   const sfName& name of removed field.
     */
    virtual void OnRemoveField(sfDictionaryProperty::SPtr dictPtr, const sfName& name) override;

    /**
     * Called when one or more elements are added to a list property.
     *
     * @param   sfListProperty::SPtr listPtr that elements were added to.
     * @param   int index elements were inserted at.
     * @param   int count - number of elements added.
     */
    virtual void OnListAdd(sfListProperty::SPtr listPtr, int index, int count) override;

    /**
     * Called when one or more elements are removed from a list property.
     *
     * @param   sfListProperty::SPtr listPtr that elements were removed from.
     * @param   int index elements were removed from.
     * @param   int count - number of elements removed.
     */
    virtual void OnListRemove(sfListProperty::SPtr listPtr, int index, int count) override;

    /**
     * Creates an sfProperty for the given graph pin.
     *
     * @param   UEdGraphPin* pinPtr.
     * @return  sfDictionaryProperty::SPtr.
     */
    sfDictionaryProperty::SPtr CreatePropertyForPin(UEdGraphPin* pinPtr);

    /**
     * Creates a LinkedTo property for the given graph pin.
     *
     * @param   UEdGraphPin* pinPtr.
     * @return  sfListProperty::SPtr.
     */
    sfListProperty::SPtr CreateLinkedToProperties(UEdGraphPin* pinPtr);

    /**
     * Initializes graph node pins.
     *
     * @param   UEdGraphNode* graphNodePtr.
     * @param   sfDictionaryProperty::SPtr pinProperties.
     * @return  bool - true if the given node's pins are successfully initialized.
     */
    bool InitializePins(UEdGraphNode* graphNodePtr, sfDictionaryProperty::SPtr pinProperties);

    /**
     * Initializes the given graph node pin.
     *
     * @param   UEdGraphNode* graphNodePtr.
     * @param   UEdGraphPin* pinPtr.
     * @param   sfDictionaryProperty::SPtr pinProperties.
     */
    void InitializePin(UEdGraphNode* graphNodePtr, UEdGraphPin* pinPtr, sfDictionaryProperty::SPtr pinProperties);

    /**
     * Sets the link for the given pin.
     *
     * @param   sfProperty::SPtr singleLinkToPropPtr - property of the link.
     * @param   UEdGraphPin* pinPtr - pin to set link for.
     * @param   bool notifyChange - if true, notify Unreal about the node change.
     * @return  UEdGraphNode*
     */
    UEdGraphNode* SetLink(sfProperty::SPtr singleLinkToPropPtr, UEdGraphPin* pinPtr, bool notifyChange);

    /**
     * Finds a node in the given graph by name.
     *
     * @param   UEdGraph* graphPtr.
     * @param   FName nodeName.
     * @return  UEdGraphNode*
     */
    UEdGraphNode* FindNode(UEdGraph* graphPtr, FName nodeName);

    /**
     * Sends a graph node link change to server.
     *
     * @param   UEdGraphNode* nodePtr.
     * @param   sfObject::SPtr objPtr.
     */
    void SendLinkChange(UEdGraphNode* nodePtr, sfObject::SPtr objPtr);

    /**
     * Gets the graph pin from the given property.
     *
     * @param   sfProperty::SPtr propertyPtr.
     * @param   UEdGraphPin*.
     */
    UEdGraphPin* GetPinForLinkProperty(sfProperty::SPtr propertyPtr);

    /**
     * Marks the given property's blueprint as modified.
     *
     * @param   sfProperty::SPtr propertyPtr.
     */
    void MarkBlueprintModified(sfProperty::SPtr propertyPtr);

    /**
     * Creates property for the given pin's default object.
     *
     * @param   UEdGraphPin* pinPtr
     * @return  sfProperty::SPtr
     */
    sfProperty::SPtr CreatePropertyForPinDefaultObject(UEdGraphPin* pinPtr);

    /**
     * Gets the pin default object from the given property.
     *
     * @param   sfProperty::SPtr propertyPtr.
     * @return  UObject*
     */
    UObject* GetDefaultObjectFromProperty(sfProperty::SPtr defaultObjectProp);

    /**
     * Finds pin from the given property.
     *
     * @param   sfProperty::SPtr propertyPtr.
     * @return  UEdGraphPin*
     */
    UEdGraphPin* FindPinFromProperty(sfProperty::SPtr pinProperty);
};
