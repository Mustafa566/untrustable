// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SceneFusion/Public/Actors/sfMissingActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodesfMissingActor() {}
// Cross Module References
	SCENEFUSION_API UClass* Z_Construct_UClass_AsfMissingActor_NoRegister();
	SCENEFUSION_API UClass* Z_Construct_UClass_AsfMissingActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_SceneFusion();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	SCENEFUSION_API UScriptStruct* Z_Construct_UScriptStruct_FsfByteArray();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent_NoRegister();
	ENGINE_API UEnum* Z_Construct_UEnum_Engine_EComponentCreationMethod();
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfMissingObject_NoRegister();
// End Cross Module References
	void AsfMissingActor::StaticRegisterNativesAsfMissingActor()
	{
	}
	UClass* Z_Construct_UClass_AsfMissingActor_NoRegister()
	{
		return AsfMissingActor::StaticClass();
	}
	struct Z_Construct_UClass_AsfMissingActor_Statics
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
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DefaultSubComponents_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_DefaultSubComponents;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_DefaultSubComponents_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ComponentCreationMethods_MetaData[];
#endif
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_ComponentCreationMethods;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ComponentCreationMethods_Key_KeyProp;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_ComponentCreationMethods_ValueProp;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_ComponentCreationMethods_ValueProp_Underlying;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ClassName_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ClassName;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AsfMissingActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_SceneFusion,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AsfMissingActor_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * Actor class to use as a stand-in for missing actor classes.\n */" },
		{ "IncludePath", "Actors/sfMissingActor.h" },
		{ "ModuleRelativePath", "Public/Actors/sfMissingActor.h" },
		{ "ToolTip", "Actor class to use as a stand-in for missing actor classes." },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ReferenceMap_MetaData[] = {
		{ "ModuleRelativePath", "Public/Actors/sfMissingActor.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ReferenceMap = { "ReferenceMap", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AsfMissingActor, ReferenceMap), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ReferenceMap_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ReferenceMap_MetaData)) };
	const UE4CodeGen_Private::FUInt32PropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ReferenceMap_Key_KeyProp = { "ReferenceMap_Key", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::UInt32, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ReferenceMap_ValueProp = { "ReferenceMap", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AsfMissingActor_Statics::NewProp_SerializedData_MetaData[] = {
		{ "ModuleRelativePath", "Public/Actors/sfMissingActor.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_SerializedData = { "SerializedData", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AsfMissingActor, SerializedData), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_AsfMissingActor_Statics::NewProp_SerializedData_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AsfMissingActor_Statics::NewProp_SerializedData_MetaData)) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_SerializedData_Key_KeyProp = { "SerializedData_Key", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_SerializedData_ValueProp = { "SerializedData", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UScriptStruct_FsfByteArray, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AsfMissingActor_Statics::NewProp_DefaultSubComponents_MetaData[] = {
		{ "Comment", "// Components with the flag RF_DefaultSubObject will not save properly, clear the flag and store the components\n// that should have it.\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Actors/sfMissingActor.h" },
		{ "ToolTip", "Components with the flag RF_DefaultSubObject will not save properly, clear the flag and store the components\nthat should have it." },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_DefaultSubComponents = { "DefaultSubComponents", nullptr, (EPropertyFlags)0x0010008000000008, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AsfMissingActor, DefaultSubComponents), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_AsfMissingActor_Statics::NewProp_DefaultSubComponents_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AsfMissingActor_Statics::NewProp_DefaultSubComponents_MetaData)) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_DefaultSubComponents_Inner = { "DefaultSubComponents", nullptr, (EPropertyFlags)0x0000000000080008, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UClass_UActorComponent_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods_MetaData[] = {
		{ "Comment", "// We have to set component creation method to instance or components will not save properly, so we store the real\n// creation method in this map.\n" },
		{ "ModuleRelativePath", "Public/Actors/sfMissingActor.h" },
		{ "ToolTip", "We have to set component creation method to instance or components will not save properly, so we store the real\ncreation method in this map." },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods = { "ComponentCreationMethods", nullptr, (EPropertyFlags)0x0010008000000000, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AsfMissingActor, ComponentCreationMethods), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods_MetaData)) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods_Key_KeyProp = { "ComponentCreationMethods_Key", nullptr, (EPropertyFlags)0x0000000000080000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UClass_UActorComponent_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods_ValueProp = { "ComponentCreationMethods", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UEnum_Engine_EComponentCreationMethod, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods_ValueProp_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ClassName_MetaData[] = {
		{ "Category", "sfMissingActor" },
		{ "Comment", "// Name of the missing actor class\n" },
		{ "ModuleRelativePath", "Public/Actors/sfMissingActor.h" },
		{ "ToolTip", "Name of the missing actor class" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ClassName = { "ClassName", nullptr, (EPropertyFlags)0x0010000000020001, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AsfMissingActor, ClassName), METADATA_PARAMS(Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ClassName_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ClassName_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AsfMissingActor_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ReferenceMap,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ReferenceMap_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ReferenceMap_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_SerializedData,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_SerializedData_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_SerializedData_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_DefaultSubComponents,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_DefaultSubComponents_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ComponentCreationMethods_ValueProp_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AsfMissingActor_Statics::NewProp_ClassName,
	};
		const UE4CodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_AsfMissingActor_Statics::InterfaceParams[] = {
			{ Z_Construct_UClass_UsfMissingObject_NoRegister, (int32)VTABLE_OFFSET(AsfMissingActor, IsfMissingObject), false },
		};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AsfMissingActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AsfMissingActor>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AsfMissingActor_Statics::ClassParams = {
		&AsfMissingActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_AsfMissingActor_Statics::PropPointers,
		InterfaceParams,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_AsfMissingActor_Statics::PropPointers),
		UE_ARRAY_COUNT(InterfaceParams),
		0x008000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AsfMissingActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AsfMissingActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AsfMissingActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AsfMissingActor_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AsfMissingActor, 3149414919);
	template<> SCENEFUSION_API UClass* StaticClass<AsfMissingActor>()
	{
		return AsfMissingActor::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AsfMissingActor(Z_Construct_UClass_AsfMissingActor, &AsfMissingActor::StaticClass, TEXT("/Script/SceneFusion"), TEXT("AsfMissingActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AsfMissingActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
