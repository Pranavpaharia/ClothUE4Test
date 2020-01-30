// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
enum class EPathType : uint8;
struct FReturnedData;
#ifdef RUNTIMEMESHLOADER_LoaderBPFunctionLibrary_generated_h
#error "LoaderBPFunctionLibrary.generated.h already included, missing '#pragma once' in LoaderBPFunctionLibrary.h"
#endif
#define RUNTIMEMESHLOADER_LoaderBPFunctionLibrary_generated_h

#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_51_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FReturnedData_Statics; \
	RUNTIMEMESHLOADER_API static class UScriptStruct* StaticStruct();


template<> RUNTIMEMESHLOADER_API UScriptStruct* StaticStruct<struct FReturnedData>();

#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_19_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FMeshInfo_Statics; \
	RUNTIMEMESHLOADER_API static class UScriptStruct* StaticStruct();


template<> RUNTIMEMESHLOADER_API UScriptStruct* StaticStruct<struct FMeshInfo>();

#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_SPARSE_DATA
#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execLoadMesh) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_filepath); \
		P_GET_ENUM(EPathType,Z_Param_type); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(FReturnedData*)Z_Param__Result=ULoaderBPFunctionLibrary::LoadMesh(Z_Param_filepath,EPathType(Z_Param_type)); \
		P_NATIVE_END; \
	}


#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execLoadMesh) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_filepath); \
		P_GET_ENUM(EPathType,Z_Param_type); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(FReturnedData*)Z_Param__Result=ULoaderBPFunctionLibrary::LoadMesh(Z_Param_filepath,EPathType(Z_Param_type)); \
		P_NATIVE_END; \
	}


#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesULoaderBPFunctionLibrary(); \
	friend struct Z_Construct_UClass_ULoaderBPFunctionLibrary_Statics; \
public: \
	DECLARE_CLASS(ULoaderBPFunctionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RuntimeMeshLoader"), NO_API) \
	DECLARE_SERIALIZER(ULoaderBPFunctionLibrary)


#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_INCLASS \
private: \
	static void StaticRegisterNativesULoaderBPFunctionLibrary(); \
	friend struct Z_Construct_UClass_ULoaderBPFunctionLibrary_Statics; \
public: \
	DECLARE_CLASS(ULoaderBPFunctionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RuntimeMeshLoader"), NO_API) \
	DECLARE_SERIALIZER(ULoaderBPFunctionLibrary)


#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ULoaderBPFunctionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ULoaderBPFunctionLibrary) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ULoaderBPFunctionLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ULoaderBPFunctionLibrary); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ULoaderBPFunctionLibrary(ULoaderBPFunctionLibrary&&); \
	NO_API ULoaderBPFunctionLibrary(const ULoaderBPFunctionLibrary&); \
public:


#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ULoaderBPFunctionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ULoaderBPFunctionLibrary(ULoaderBPFunctionLibrary&&); \
	NO_API ULoaderBPFunctionLibrary(const ULoaderBPFunctionLibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ULoaderBPFunctionLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ULoaderBPFunctionLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ULoaderBPFunctionLibrary)


#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_PRIVATE_PROPERTY_OFFSET
#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_72_PROLOG
#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_PRIVATE_PROPERTY_OFFSET \
	MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_SPARSE_DATA \
	MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_RPC_WRAPPERS \
	MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_INCLASS \
	MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_PRIVATE_PROPERTY_OFFSET \
	MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_SPARSE_DATA \
	MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_RPC_WRAPPERS_NO_PURE_DECLS \
	MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_INCLASS_NO_PURE_DECLS \
	MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h_75_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> RUNTIMEMESHLOADER_API UClass* StaticClass<class ULoaderBPFunctionLibrary>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID MyProject2_Plugins_RuntimeMeshLoader_Source_RuntimeMeshLoader_Public_LoaderBPFunctionLibrary_h


#define FOREACH_ENUM_EPATHTYPE(op) \
	op(EPathType::Absolute) \
	op(EPathType::Relative) 

enum class EPathType : uint8;
template<> RUNTIMEMESHLOADER_API UEnum* StaticEnum<EPathType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
