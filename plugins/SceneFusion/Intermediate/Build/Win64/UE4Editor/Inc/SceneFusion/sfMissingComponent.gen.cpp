// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SceneFusion/Public/Components/sfMissingComponent.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodesfMissingComponent() {}
// Cross Module References
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfMissingComponent_NoRegister();
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfMissingComponent();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	UPackage* Z_Construct_UPackage__Script_SceneFusion();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	SCENEFUSION_API UScriptStruct* Z_Construct_UScriptStruct_FsfByteArray();
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfMissingObject_NoRegister();
// End Cross Module References
	void UsfMissingComponent::StaticRegisterNativesUsfMissingComponent()
	{
	}
	UClass* Z_Construct_UClass_UsfMissingComponent_NoRegister()
	{
		return UsfMissingComponent::StaticClass();
	}
	struct Z_Construct_UClass_UsfMissingComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReferenceMap_MetaData[];
#endif
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_ReferenceMap;
		static const UE4CodeGen_Private::FUInt32PropertyParams NewProp_ReferenceMap_Key_KeyProp;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReferenceMap_ValueProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SerializedData_MetaData[];
#endif
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_SerializedData;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_SerializedData_Key_KeyProp;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_SerializedData_ValueProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ClassName_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ClassName;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UsfMissingComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_SceneFusion,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfMissingComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "Comment", "/**\n * Component class to use as a stand-in for missing non-scene component classes.\n */" },
		{ "IncludePath", "Components/sfMissingComponent.h" },
		{ "ModuleRelativePath", "Public/Components/sfMissingComponent.h" },
		{ "ToolTip", "Component class to use as a stand-in for missing non-scene component classes." },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ReferenceMap_MetaData[] = {
		{ "ModuleRelativePath", "Public/Components/sfMissingComponent.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ReferenceMap = { "ReferenceMap", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UsfMissingComponent, ReferenceMap), METADATA_PARAMS(Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ReferenceMap_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ReferenceMap_MetaData)) };
	const UE4CodeGen_Private::FUInt32PropertyParams Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ReferenceMap_Key_KeyProp = { "ReferenceMap_Key", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::UInt32, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ReferenceMap_ValueProp = { "ReferenceMap", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_SerializedData_MetaData[] = {
		{ "ModuleRelativePath", "Public/Components/sfMissingComponent.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_SerializedData = { "SerializedData", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UsfMissingComponent, SerializedData), METADATA_PARAMS(Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_SerializedData_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_SerializedData_MetaData)) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_SerializedData_Key_KeyProp = { "SerializedData_Key", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_SerializedData_ValueProp = { "SerializedData", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UScriptStruct_FsfByteArray, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ClassName_MetaData[] = {
		{ "Category", "sfMissingComponent" },
		{ "Comment", "// Name of the missing component class\n" },
		{ "ModuleRelativePath", "Public/Components/sfMissingComponent.h" },
		{ "ToolTip", "Name of the missing component class" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ClassName = { "ClassName", nullptr, (EPropertyFlags)0x0010000000020001, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UsfMissingComponent, ClassName), METADATA_PARAMS(Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ClassName_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ClassName_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UsfMissingComponent_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ReferenceMap,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ReferenceMap_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ReferenceMap_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_SerializedData,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_SerializedData_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_SerializedData_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfMissingComponent_Statics::NewProp_ClassName,
	};
		const UE4CodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_UsfMissingComponent_Statics::InterfaceParams[] = {
			{ Z_Construct_UClass_UsfMissingObject_NoRegister, (int32)VTABLE_OFFSET(UsfMissingComponent, IsfMissingObject), false },
		};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UsfMissingComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UsfMissingComponent>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UsfMissingComponent_Statics::ClassParams = {
		&UsfMissingComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UsfMissingComponent_Statics::PropPointers,
		InterfaceParams,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UsfMissingComponent_Statics::PropPointers),
		UE_ARRAY_COUNT(InterfaceParams),
		0x00A000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UsfMissingComponent_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UsfMissingComponent_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UsfMissingComponent()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UsfMissingComponent_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UsfMissingComponent, 1531988613);
	template<> SCENEFUSION_API UClass* StaticClass<UsfMissingComponent>()
	{
		return UsfMissingComponent::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UsfMissingComponent(Z_Construct_UClass_UsfMissingComponent, &UsfMissingComponent::StaticClass, TEXT("/Script/SceneFusion"), TEXT("UsfMissingComponent"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UsfMissingComponent);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
