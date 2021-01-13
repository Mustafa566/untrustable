#include "sfDummyEdMode.h"
#include "SceneFusionEdMode.h"

void UsfDummyEdMode::Tick(FEditorViewportClient* viewportClientPtr, float deltaTime)
{

}

bool UsfDummyEdMode::HandleClick(
    FEditorViewportClient* viewportClientPtr,
    HHitProxy* hitProxyPtr,
    const FViewportClick& click)
{
    RestoreMode();
    return false;
}

bool UsfDummyEdMode::StartTracking(FEditorViewportClient* viewportClientPtr, FViewport* viewportPtr)
{
    RestoreMode();
    return false;
}

bool UsfDummyEdMode::InputKey(
    FEditorViewportClient* viewportClientPtr,
    FViewport* viewportPtr,
    FKey key,
    EInputEvent event)
{
    RestoreMode();
    return false;
}

bool UsfDummyEdMode::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int x, int y)
{
    RestoreMode();
    return false;
}

bool UsfDummyEdMode::CapturedMouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int x, int y)
{
    RestoreMode();
    return false;
}

void UsfDummyEdMode::SetOwner(FEditorModeTools* owner)
{
    Owner = owner;
}

void UsfDummyEdMode::RestoreMode()
{
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 26
    // This is called when the dummy editor mode receives an event. Now that we've blocked the real mode from receiving
    // the event, we replace the dummy with the real mode.
    if (Modes.Num() <= 0)
    {
        return;
    }
    // Reinterpret to our hack class to expose the protected array of active editor modes.
    sfEditorModeToolsHack* hackPtr = static_cast<sfEditorModeToolsHack*>(&GLevelEditorModeTools());
    // Calculate the index of the mode that is processing this event, based on the number of modes left in our Modes
    // array.
    int index = hackPtr->GetModes().Num() - Modes.Num();
    if (index >= 0)
    {
        // Replace the dummy mode with the real mode, and remove the real mode from our Modes array.
        hackPtr->GetModes()[index] = Modes.Pop();
    }
#endif
}