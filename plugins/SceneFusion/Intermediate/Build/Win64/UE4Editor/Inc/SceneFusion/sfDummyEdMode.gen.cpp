// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SceneFusion/Private/sfDummyEdMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodesfDummyEdMode() {}
// Cross Module References
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfDummyEdMode_NoRegister();
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfDummyEdMode();
	UNREALED_API UClass* Z_Construct_UClass_UEdMode();
	UPackage* Z_Construct_UPackage__Script_SceneFusion();
// End Cross Module References
	void UsfDummyEdMode::StaticRegisterNativesUsfDummyEdMode()
	{
	}
	UClass* Z_Construct_UClass_UsfDummyEdMode_NoRegister()
	{
		return UsfDummyEdMode::StaticClass();
	}
	struct Z_Construct_UClass_UsfDummyEdMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UsfDummyEdMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UEdMode,
		(UObject* (*)())Z_Construct_UPackage__Script_SceneFusion,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfDummyEdMode_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * Part of a hack to prevent other editor modes from processing an event. We replace the other editor modes in the\n * protected Modes array we exposed with a dummy editor mode, and when the dummy editor mode processes an event,\n * it replaces itself with the mode that used to be there.\n */" },
		{ "IncludePath", "sfDummyEdMode.h" },
		{ "ModuleRelativePath", "Private/sfDummyEdMode.h" },
		{ "ToolTip", "Part of a hack to prevent other editor modes from processing an event. We replace the other editor modes in the\nprotected Modes array we exposed with a dummy editor mode, and when the dummy editor mode processes an event,\nit replaces itself with the mode that used to be there." },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UsfDummyEdMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UsfDummyEdMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UsfDummyEdMode_Statics::ClassParams = {
		&UsfDummyEdMode::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UsfDummyEdMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UsfDummyEdMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UsfDummyEdMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UsfDummyEdMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UsfDummyEdMode, 2377372069);
	template<> SCENEFUSION_API UClass* StaticClass<UsfDummyEdMode>()
	{
		return UsfDummyEdMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UsfDummyEdMode(Z_Construct_UClass_UsfDummyEdMode, &UsfDummyEdMode::StaticClass, TEXT("/Script/SceneFusion"), TEXT("UsfDummyEdMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UsfDummyEdMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
