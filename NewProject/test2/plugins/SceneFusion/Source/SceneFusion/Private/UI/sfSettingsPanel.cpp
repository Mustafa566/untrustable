#include "sfSettingsPanel.h"
#include "sfUIStyles.h"
#include "../../Public/sfConfig.h"

#include <Widgets/Layout/SScrollBox.h>
#include <Widgets/Input/SHyperlink.h>

TSharedRef<SDockTab> sfSettingsPanel::CreateSettingsTab()
{
    return SNew(SDockTab)
        .Icon(sfUIStyles::Get().GetBrush("SceneFusion.TabIcon"))
        .TabRole(NomadTab)
        [
            SNew(SScrollBox)
            + SScrollBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Center).Padding(5, 10)
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Center).AutoHeight().Padding(2, 10, 2, 10)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Session Settings"))
                    .Font(sfUIStyles::GetDefaultFontStyle("Regular", 18))
                ]
                + SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Center).AutoHeight().Padding(2)
                [
                    SNew(SBorder)
                    .HAlign(HAlign_Fill)
                    .VAlign(VAlign_Center)
                    .Padding(FMargin(5, 5))
                    .BorderImage(new FSlateColorBrush(FLinearColor(FColor(0, 0, 0, 128))))
                    [
                        CreateSessionSettingsArea()
                    ]
                ]
            ]
        ];
}

TSharedRef<SVerticalBox> sfSettingsPanel::CreateSessionSettingsArea()
{
    return SNew(SVerticalBox)
        + SVerticalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Center).AutoHeight().Padding(2, 10, 2, 10)
        [
            SNew(STextBlock)
            .Text(FText::FromString("Synchronization Control"))
            .Font(sfUIStyles::GetDefaultFontStyle("Regular", 12))
        ]
        + SVerticalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Center).AutoHeight().Padding(2)
        [
            SNew(SCheckBox)
            .HAlign(HAlign_Left)
            .IsEnabled_Lambda([]() -> bool {
                return SceneFusion::Get().Service->Session() == nullptr;
            })
            .OnCheckStateChanged_Lambda([](ECheckBoxState newCheckedState){
                sfConfig::Get().SyncLandscape = newCheckedState == ECheckBoxState::Checked;
                sfConfig::Get().Save();
            })
            .IsChecked_Lambda([]()-> const ECheckBoxState {
                return sfConfig::Get().SyncLandscape ?
                    ECheckBoxState::Checked :
                    ECheckBoxState::Unchecked;
            })
            .ToolTipText(FText::FromString("Sync landscape creation, deletion, and modification to other users."))
            [
                SNew(STextBlock)
                .Text(FText::FromString("Landscape"))
            ]
        ]
        + SVerticalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Center).AutoHeight().Padding(2)
        [
            SNew(SCheckBox)
            .HAlign(HAlign_Left)
            .IsEnabled_Lambda([]() -> bool {
                return SceneFusion::Get().Service->Session() == nullptr;
            })
            .OnCheckStateChanged_Lambda([](ECheckBoxState newCheckedState){
                sfConfig::Get().SyncBlueprint = newCheckedState == ECheckBoxState::Checked;
                if (!sfConfig::Get().SyncBlueprint)
                {
                    sfConfig::Get().SyncBlueprintGraph = false;
                }
                sfConfig::Get().Save();
            })
            .IsChecked_Lambda([]()-> const ECheckBoxState {
                return sfConfig::Get().SyncBlueprint ?
                    ECheckBoxState::Checked :
                    ECheckBoxState::Unchecked;
            })
            .ToolTipText(FText::FromString("Sync blueprint's default actors and components to other users."))
            [
                SNew(STextBlock)
                .Text(FText::FromString("Blueprint (Experimental)"))
            ]
        ]
        + SVerticalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Center).AutoHeight().Padding(18, 2)
        [
            SNew(SCheckBox)
            .HAlign(HAlign_Left)
            .IsEnabled_Lambda([]() -> bool {
                return SceneFusion::Get().Service->Session() == nullptr && sfConfig::Get().SyncBlueprint;
            })
            .OnCheckStateChanged_Lambda([](ECheckBoxState newCheckedState){
                sfConfig::Get().SyncBlueprintGraph = newCheckedState == ECheckBoxState::Checked;
                sfConfig::Get().Save();
            })
            .IsChecked_Lambda([]()-> const ECheckBoxState {
                return sfConfig::Get().SyncBlueprintGraph ?
                    ECheckBoxState::Checked :
                    ECheckBoxState::Unchecked;
            })
            .ToolTipText(FText::FromString("Sync blueprint event graph, construction script, functions, macros, "
                "variables, event dispatchers and local variables changes to other users."))
            [
                SNew(STextBlock)
                .Text(FText::FromString("Blueprint Graphs (Experimental)"))
            ]
        ]
        + SVerticalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Center).AutoHeight().Padding(2)
        [
            SNew(SHyperlink)
            .Text(FText::FromString("More Info"))
            .OnNavigate(FSimpleDelegate::CreateLambda([]() {
                FString error = "";
                FPlatformProcess::LaunchURL(
                    *FString("https://docs.kinematicsoup.com/SceneFusion/unreal/configuration_options.html"),
                    nullptr,
                    &error);
            }))
        ];
}