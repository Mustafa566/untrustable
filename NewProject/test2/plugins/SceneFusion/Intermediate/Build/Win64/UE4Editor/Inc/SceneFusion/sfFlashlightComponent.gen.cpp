// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SceneFusion/Private/Components/sfFlashlightComponent.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodesfFlashlightComponent() {}
// Cross Module References
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfFlashlightComponent_NoRegister();
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfFlashlightComponent();
	ENGINE_API UClass* Z_Construct_UClass_USpotLightComponent();
	UPackage* Z_Construct_UPackage__Script_SceneFusion();
// End Cross Module References
	void UsfFlashlightComponent::StaticRegisterNativesUsfFlashlightComponent()
	{
	}
	UClass* Z_Construct_UClass_UsfFlashlightComponent_NoRegister()
	{
		return UsfFlashlightComponent::StaticClass();
	}
	struct Z_Construct_UClass_UsfFlashlightComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UsfFlashlightComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_USpotLightComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_SceneFusion,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfFlashlightComponent_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * Flashlight component that has no icon shown in the viewport.\n */" },
		{ "HideCategories", "Object Object LightShafts Object LightShafts Trigger Activation Components|Activation Physics Trigger Activation Components|Activation Physics Trigger PhysicsVolume" },
		{ "IncludePath", "Components/sfFlashlightComponent.h" },
		{ "ModuleRelativePath", "Private/Components/sfFlashlightComponent.h" },
		{ "ToolTip", "Flashlight component that has no icon shown in the viewport." },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UsfFlashlightComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UsfFlashlightComponent>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UsfFlashlightComponent_Statics::ClassParams = {
		&UsfFlashlightComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x00A010A4u,
		METADATA_PARAMS(Z_Construct_UClass_UsfFlashlightComponent_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UsfFlashlightComponent_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UsfFlashlightComponent()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UsfFlashlightComponent_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UsfFlashlightComponent, 3144179510);
	template<> SCENEFUSION_API UClass* StaticClass<UsfFlashlightComponent>()
	{
		return UsfFlashlightComponent::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UsfFlashlightComponent(Z_Construct_UClass_UsfFlashlightComponent, &UsfFlashlightComponent::StaticClass, TEXT("/Script/SceneFusion"), TEXT("UsfFlashlightComponent"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UsfFlashlightComponent);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
