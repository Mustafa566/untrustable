// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SceneFusion/Private/Actors/sfStandInTrackerActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodesfStandInTrackerActor() {}
// Cross Module References
	SCENEFUSION_API UClass* Z_Construct_UClass_AsfStandInTrackerActor_NoRegister();
	SCENEFUSION_API UClass* Z_Construct_UClass_AsfStandInTrackerActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_SceneFusion();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
// End Cross Module References
	void AsfStandInTrackerActor::StaticRegisterNativesAsfStandInTrackerActor()
	{
	}
	UClass* Z_Construct_UClass_AsfStandInTrackerActor_NoRegister()
	{
		return AsfStandInTrackerActor::StaticClass();
	}
	struct Z_Construct_UClass_AsfStandInTrackerActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_StandIns_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_StandIns;
		static const UE4CodeGen_Private::FWeakObjectPropertyParams NewProp_StandIns_Inner;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AsfStandInTrackerActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_SceneFusion,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AsfStandInTrackerActor_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * Stores a weak pointer array of non-actor non-component asset stand-ins referenced in a level so we quickly know\n * which missing assets are referenced in a level when we load it.\n */" },
		{ "IncludePath", "Actors/sfStandInTrackerActor.h" },
		{ "ModuleRelativePath", "Private/Actors/sfStandInTrackerActor.h" },
		{ "ToolTip", "Stores a weak pointer array of non-actor non-component asset stand-ins referenced in a level so we quickly know\nwhich missing assets are referenced in a level when we load it." },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AsfStandInTrackerActor_Statics::NewProp_StandIns_MetaData[] = {
		{ "Comment", "/**\n     * Stand-ins referenced in the level.\n     */" },
		{ "ModuleRelativePath", "Private/Actors/sfStandInTrackerActor.h" },
		{ "ToolTip", "Stand-ins referenced in the level." },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UClass_AsfStandInTrackerActor_Statics::NewProp_StandIns = { "StandIns", nullptr, (EPropertyFlags)0x0014000000000000, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AsfStandInTrackerActor, StandIns), METADATA_PARAMS(Z_Construct_UClass_AsfStandInTrackerActor_Statics::NewProp_StandIns_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AsfStandInTrackerActor_Statics::NewProp_StandIns_MetaData)) };
	const UE4CodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_AsfStandInTrackerActor_Statics::NewProp_StandIns_Inner = { "StandIns", nullptr, (EPropertyFlags)0x0004000000000000, UE4CodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AsfStandInTrackerActor_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfStandInTrackerActor_Statics::NewProp_StandIns,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfStandInTrackerActor_Statics::NewProp_StandIns_Inner,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AsfStandInTrackerActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AsfStandInTrackerActor>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AsfStandInTrackerActor_Statics::ClassParams = {
		&AsfStandInTrackerActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_AsfStandInTrackerActor_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_AsfStandInTrackerActor_Statics::PropPointers),
		0,
		0x008000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AsfStandInTrackerActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AsfStandInTrackerActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AsfStandInTrackerActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AsfStandInTrackerActor_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AsfStandInTrackerActor, 1812132275);
	template<> SCENEFUSION_API UClass* StaticClass<AsfStandInTrackerActor>()
	{
		return AsfStandInTrackerActor::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AsfStandInTrackerActor(Z_Construct_UClass_AsfStandInTrackerActor, &AsfStandInTrackerActor::StaticClass, TEXT("/Script/SceneFusion"), TEXT("AsfStandInTrackerActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AsfStandInTrackerActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
