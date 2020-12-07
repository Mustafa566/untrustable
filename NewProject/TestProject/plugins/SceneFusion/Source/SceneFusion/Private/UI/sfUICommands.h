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
#include <Framework/Commands/Commands.h>

/**
 * Scene Fusion UI command list
 */
class sfUICommands : public TCommands<sfUICommands>
{
public:
    // Constructor
    sfUICommands();

    // TCommands<> interface
    virtual void RegisterCommands() override;

    // Commands
    TSharedPtr<FUICommandInfo> ToolBarClickPtr;
    TSharedPtr<FUICommandInfo> MissingAssetsPtr;
    TSharedPtr<FUICommandInfo> GettingStartedPtr;
    TSharedPtr<FUICommandInfo> ExperimentalFeaturesPtr;
    TSharedPtr<FUICommandInfo> WebConsolePtr;
    TSharedPtr<FUICommandInfo> DocumentPtr;
    TSharedPtr<FUICommandInfo> FeedbackPtr;
};