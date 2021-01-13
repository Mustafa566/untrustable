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

#include <functional>
#include <vector>
#include <CoreMinimal.h>
#include <EdMode.h>
#include <EditorModeManager.h>

#include "sfDummyEdMode.generated.h"

/**
 * Part of a hack to prevent other editor modes from processing an event. We replace the other editor modes in the
 * protected Modes array we exposed with a dummy editor mode, and when the dummy editor mode processes an event,
 * it replaces itself with the mode that used to be there.
 */
UCLASS()
class UsfDummyEdMode : public UEdMode
{
    GENERATED_BODY()
public:
    /**
     * Array of modes we replaced with the dummy.
     */
    TArray<UEdMode*> Modes;

    /**
     * Do nothing.
     *
     * @param   FEditorViewportClient* viewportClientPtr
     * @param   float deltaTime
     */
    void Tick(FEditorViewportClient* viewportClientPtr, float deltaTime) override;

    /**
     * Called to handle click events. Replaces this dummy with the original editor mode.
     *
     * @param   FEditorViewportClient* viewportClientPtr
     * @param   HHitProxy* hitProxyPtr
     * @param   const FViewportClick& click
     * @return  bool false to allow the default event handling.
     */
    virtual bool HandleClick(
        FEditorViewportClient* viewportClientPtr,
        HHitProxy* hitProxyPtr,
        const FViewportClick& click);

    /**
     * Called to handle start tracking events. Replaces this dummy with the original editor mode.
     *
     * @param   FEditorViewportClient* viewportClientPtr
     * @param   FViewport* viewportPtr
     * @return  bool false to allow the default event handling.
     */
    virtual bool StartTracking(FEditorViewportClient* viewportClientPtr, FViewport* viewportPtr) override;

    /**
     * Called to handle keyboard events. Replaces this dummy with the original editor mode.
     *
     * @param   FEditorViewportClient* viewportClientPtr
     * @param   FViewport* viewportPtr
     * @param   FKey key
     * @param   EInputEvent ev
     * @return  bool false to allow the default event handling.
     */
    virtual bool InputKey(
        FEditorViewportClient* viewportClientPtr,
        FViewport* viewportPtr,
        FKey key,
        EInputEvent event) override;

    /**
     * Called to handle mouse move events. Replaces this dummy with the original editor mode.
     *
     * @param   FEditorViewportClient* viewportClientPtr
     * @param   FViewport* viewportPtr
     * @param   int x
     * @param   int y
     * @return  bool false to allow the default event handling.
     */
    virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int x, int y) override;

    /**
     * Called to handle captured mouse move events. Replaces this dummy with the original editor mode.
     *
     * @param   FEditorViewportClient* viewportClientPtr
     * @param   FViewport* viewportPtr
     * @param   int x
     * @param   int y
     * @return  bool false to allow the default event handling.
     */
    virtual bool CapturedMouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int x, int y) override;

    /**
     * Sets owner.
     *
     * @param   FEditorModeTools* owner
     */
    void SetOwner(FEditorModeTools* owner);

private:
    /**
     * Replaces this dummy in the protected array we exposed from FEditorModeTool with the last mode in our Modes
     * array. Removes the last mode from our Modes array.
     */
    void RestoreMode();
};