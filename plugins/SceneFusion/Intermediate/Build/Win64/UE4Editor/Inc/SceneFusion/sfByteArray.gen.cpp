// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SceneFusion/Public/sfByteArray.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodesfByteArray() {}
// Cross Module References
	SCENEFUSION_API UScriptStruct* Z_Construct_UScriptStruct_FsfByteArray();
	UPackage* Z_Construct_UPackage__Script_SceneFusion();
// End Cross Module References
class UScriptStruct* FsfByteArray::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern SCENEFUSION_API uint32 Get_Z_Construct_UScriptStruct_FsfByteArray_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FsfByteArray, Z_Construct_UPackage__Script_SceneFusion(), TEXT("sfByteArray"), sizeof(FsfByteArray), Get_Z_Construct_UScriptStruct_FsfByteArray_Hash());
	}
	return Singleton;
}
template<> SCENEFUSION_API UScriptStruct* StaticStruct<FsfByteArray>()
{
	return FsfByteArray::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FsfByteArray(FsfByteArray::StaticStruct, TEXT("/Script/SceneFusion"), TEXT("sfByteArray"), false, nullptr, nullptr);
static struct FScriptStruct_SceneFusion_StaticRegisterNativesFsfByteArray
{
	FScriptStruct_SceneFusion_StaticRegisterNativesFsfByteArray()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("sfByteArray")),new UScriptStruct::TCppStructOps<FsfByteArray>);
	}
} ScriptStruct_SceneFusion_StaticRegisterNativesFsfByteArray;
	struct Z_Construct_UScriptStruct_FsfByteArray_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Data_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Data;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Data_Inner;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FsfByteArray_Statics::Struct_MetaDataParams[] = {
		{ "Comment", "/**\n * A struct the wraps a byte array. Unreal doesn't allow nested array properties but does allow arrays of structs, so\n * we use this to create UPROPERTY containers of byte arrays.\n */" },
		{ "ModuleRelativePath", "Public/sfByteArray.h" },
		{ "ToolTip", "A struct the wraps a byte array. Unreal doesn't allow nested array properties but does allow arrays of structs, so\nwe use this to create UPROPERTY containers of byte arrays." },
	};
#endif
	void* Z_Construct_UScriptStruct_FsfByteArray_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FsfByteArray>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FsfByteArray_Statics::NewProp_Data_MetaData[] = {
		{ "ModuleRelativePath", "Public/sfByteArray.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FsfByteArray_Statics::NewProp_Data = { "Data", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FsfByteArray, Data), METADATA_PARAMS(Z_Construct_UScriptStruct_FsfByteArray_Statics::NewProp_Data_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FsfByteArray_Statics::NewProp_Data_MetaData)) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FsfByteArray_Statics::NewProp_Data_Inner = { "Data", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FsfByteArray_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FsfByteArray_Statics::NewProp_Data,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FsfByteArray_Statics::NewProp_Data_Inner,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FsfByteArray_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SceneFusion,
		nullptr,
		&NewStructOps,
		"sfByteArray",
		sizeof(FsfByteArray),
		alignof(FsfByteArray),
		Z_Construct_UScriptStruct_FsfByteArray_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FsfByteArray_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FsfByteArray_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FsfByteArray_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FsfByteArray()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FsfByteArray_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_SceneFusion();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("sfByteArray"), sizeof(FsfByteArray), Get_Z_Construct_UScriptStruct_FsfByteArray_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FsfByteArray_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FsfByteArray_Hash() { return 3451602243U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
