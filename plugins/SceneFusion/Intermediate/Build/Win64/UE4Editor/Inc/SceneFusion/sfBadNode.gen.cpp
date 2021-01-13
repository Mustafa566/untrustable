// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SceneFusion/Private/Objects/sfBadNode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodesfBadNode() {}
// Cross Module References
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfBadNode_NoRegister();
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfBadNode();
	BLUEPRINTGRAPH_API UClass* Z_Construct_UClass_UK2Node();
	UPackage* Z_Construct_UPackage__Script_SceneFusion();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	SCENEFUSION_API UScriptStruct* Z_Construct_UScriptStruct_FsfByteArray();
	SCENEFUSION_API UClass* Z_Construct_UClass_UsfMissingObject_NoRegister();
// End Cross Module References
	void UsfBadNode::StaticRegisterNativesUsfBadNode()
	{
	}
	UClass* Z_Construct_UClass_UsfBadNode_NoRegister()
	{
		return UsfBadNode::StaticClass();
	}
	struct Z_Construct_UClass_UsfBadNode_Statics
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
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MissingClassName_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_MissingClassName;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_NodeTypeName_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_NodeTypeName;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UsfBadNode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UK2Node,
		(UObject* (*)())Z_Construct_UPackage__Script_SceneFusion,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfBadNode_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n  * Represents a node that is in a bad state.\n  */" },
		{ "IncludePath", "Objects/sfBadNode.h" },
		{ "ModuleRelativePath", "Private/Objects/sfBadNode.h" },
		{ "ToolTip", "Represents a node that is in a bad state." },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfBadNode_Statics::NewProp_ReferenceMap_MetaData[] = {
		{ "ModuleRelativePath", "Private/Objects/sfBadNode.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_UsfBadNode_Statics::NewProp_ReferenceMap = { "ReferenceMap", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UsfBadNode, ReferenceMap), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UsfBadNode_Statics::NewProp_ReferenceMap_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UsfBadNode_Statics::NewProp_ReferenceMap_MetaData)) };
	const UE4CodeGen_Private::FUInt32PropertyParams Z_Construct_UClass_UsfBadNode_Statics::NewProp_ReferenceMap_Key_KeyProp = { "ReferenceMap_Key", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::UInt32, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UsfBadNode_Statics::NewProp_ReferenceMap_ValueProp = { "ReferenceMap", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfBadNode_Statics::NewProp_SerializedData_MetaData[] = {
		{ "ModuleRelativePath", "Private/Objects/sfBadNode.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_UsfBadNode_Statics::NewProp_SerializedData = { "SerializedData", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UsfBadNode, SerializedData), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UsfBadNode_Statics::NewProp_SerializedData_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UsfBadNode_Statics::NewProp_SerializedData_MetaData)) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UClass_UsfBadNode_Statics::NewProp_SerializedData_Key_KeyProp = { "SerializedData_Key", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UsfBadNode_Statics::NewProp_SerializedData_ValueProp = { "SerializedData", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UScriptStruct_FsfByteArray, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfBadNode_Statics::NewProp_MissingClassName_MetaData[] = {
		{ "Category", "Scene Fusion" },
		{ "ModuleRelativePath", "Private/Objects/sfBadNode.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UClass_UsfBadNode_Statics::NewProp_MissingClassName = { "MissingClassName", nullptr, (EPropertyFlags)0x0010000000020001, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UsfBadNode, MissingClassName), METADATA_PARAMS(Z_Construct_UClass_UsfBadNode_Statics::NewProp_MissingClassName_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UsfBadNode_Statics::NewProp_MissingClassName_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UsfBadNode_Statics::NewProp_NodeTypeName_MetaData[] = {
		{ "ModuleRelativePath", "Private/Objects/sfBadNode.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UClass_UsfBadNode_Statics::NewProp_NodeTypeName = { "NodeTypeName", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UsfBadNode, NodeTypeName), METADATA_PARAMS(Z_Construct_UClass_UsfBadNode_Statics::NewProp_NodeTypeName_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UsfBadNode_Statics::NewProp_NodeTypeName_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UsfBadNode_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfBadNode_Statics::NewProp_ReferenceMap,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfBadNode_Statics::NewProp_ReferenceMap_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfBadNode_Statics::NewProp_ReferenceMap_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfBadNode_Statics::NewProp_SerializedData,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfBadNode_Statics::NewProp_SerializedData_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfBadNode_Statics::NewProp_SerializedData_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfBadNode_Statics::NewProp_MissingClassName,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UsfBadNode_Statics::NewProp_NodeTypeName,
	};
		const UE4CodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_UsfBadNode_Statics::InterfaceParams[] = {
			{ Z_Construct_UClass_UsfMissingObject_NoRegister, (int32)VTABLE_OFFSET(UsfBadNode, IsfMissingObject), false },
		};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UsfBadNode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UsfBadNode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UsfBadNode_Statics::ClassParams = {
		&UsfBadNode::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UsfBadNode_Statics::PropPointers,
		InterfaceParams,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UsfBadNode_Statics::PropPointers),
		UE_ARRAY_COUNT(InterfaceParams),
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UsfBadNode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UsfBadNode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UsfBadNode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UsfBadNode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UsfBadNode, 1865369304);
	template<> SCENEFUSION_API UClass* StaticClass<UsfBadNode>()
	{
		return UsfBadNode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UsfBadNode(Z_Construct_UClass_UsfBadNode, &UsfBadNode::StaticClass, TEXT("/Script/SceneFusion"), TEXT("UsfBadNode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UsfBadNode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
