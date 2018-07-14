// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UTexture2D;
#ifdef MYTCP_MyPCL_generated_h
#error "MyPCL.generated.h already included, missing '#pragma once' in MyPCL.h"
#endif
#define MYTCP_MyPCL_generated_h

#define MyTCP_Source_MyTCP_MyPCL_h_15_RPC_WRAPPERS \
	virtual void OnTextureUpdated_Implementation(UTexture2D* PosTexture, UTexture2D* ColorTexture); \
 \
	DECLARE_FUNCTION(execOnTextureUpdated) \
	{ \
		P_GET_OBJECT(UTexture2D,Z_Param_PosTexture); \
		P_GET_OBJECT(UTexture2D,Z_Param_ColorTexture); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->OnTextureUpdated_Implementation(Z_Param_PosTexture,Z_Param_ColorTexture); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execStartTCPReceiver) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_SocketName); \
		P_GET_PROPERTY(UStrProperty,Z_Param_TheIP); \
		P_GET_PROPERTY(UIntProperty,Z_Param_ThePort); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->StartTCPReceiver(Z_Param_SocketName,Z_Param_TheIP,Z_Param_ThePort); \
		P_NATIVE_END; \
	}


#define MyTCP_Source_MyTCP_MyPCL_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual void OnTextureUpdated_Implementation(UTexture2D* PosTexture, UTexture2D* ColorTexture); \
 \
	DECLARE_FUNCTION(execOnTextureUpdated) \
	{ \
		P_GET_OBJECT(UTexture2D,Z_Param_PosTexture); \
		P_GET_OBJECT(UTexture2D,Z_Param_ColorTexture); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->OnTextureUpdated_Implementation(Z_Param_PosTexture,Z_Param_ColorTexture); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execStartTCPReceiver) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_SocketName); \
		P_GET_PROPERTY(UStrProperty,Z_Param_TheIP); \
		P_GET_PROPERTY(UIntProperty,Z_Param_ThePort); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->StartTCPReceiver(Z_Param_SocketName,Z_Param_TheIP,Z_Param_ThePort); \
		P_NATIVE_END; \
	}


#define MyTCP_Source_MyTCP_MyPCL_h_15_EVENT_PARMS \
	struct MyPCL_eventOnTextureUpdated_Parms \
	{ \
		UTexture2D* PosTexture; \
		UTexture2D* ColorTexture; \
	};


#define MyTCP_Source_MyTCP_MyPCL_h_15_CALLBACK_WRAPPERS
#define MyTCP_Source_MyTCP_MyPCL_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAMyPCL(); \
	friend MYTCP_API class UClass* Z_Construct_UClass_AMyPCL(); \
public: \
	DECLARE_CLASS(AMyPCL, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/MyTCP"), NO_API) \
	DECLARE_SERIALIZER(AMyPCL) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define MyTCP_Source_MyTCP_MyPCL_h_15_INCLASS \
private: \
	static void StaticRegisterNativesAMyPCL(); \
	friend MYTCP_API class UClass* Z_Construct_UClass_AMyPCL(); \
public: \
	DECLARE_CLASS(AMyPCL, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/MyTCP"), NO_API) \
	DECLARE_SERIALIZER(AMyPCL) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define MyTCP_Source_MyTCP_MyPCL_h_15_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AMyPCL(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AMyPCL) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMyPCL); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMyPCL); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AMyPCL(AMyPCL&&); \
	NO_API AMyPCL(const AMyPCL&); \
public:


#define MyTCP_Source_MyTCP_MyPCL_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AMyPCL(AMyPCL&&); \
	NO_API AMyPCL(const AMyPCL&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMyPCL); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMyPCL); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AMyPCL)


#define MyTCP_Source_MyTCP_MyPCL_h_15_PRIVATE_PROPERTY_OFFSET
#define MyTCP_Source_MyTCP_MyPCL_h_12_PROLOG \
	MyTCP_Source_MyTCP_MyPCL_h_15_EVENT_PARMS


#define MyTCP_Source_MyTCP_MyPCL_h_15_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MyTCP_Source_MyTCP_MyPCL_h_15_PRIVATE_PROPERTY_OFFSET \
	MyTCP_Source_MyTCP_MyPCL_h_15_RPC_WRAPPERS \
	MyTCP_Source_MyTCP_MyPCL_h_15_CALLBACK_WRAPPERS \
	MyTCP_Source_MyTCP_MyPCL_h_15_INCLASS \
	MyTCP_Source_MyTCP_MyPCL_h_15_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define MyTCP_Source_MyTCP_MyPCL_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MyTCP_Source_MyTCP_MyPCL_h_15_PRIVATE_PROPERTY_OFFSET \
	MyTCP_Source_MyTCP_MyPCL_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	MyTCP_Source_MyTCP_MyPCL_h_15_CALLBACK_WRAPPERS \
	MyTCP_Source_MyTCP_MyPCL_h_15_INCLASS_NO_PURE_DECLS \
	MyTCP_Source_MyTCP_MyPCL_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID MyTCP_Source_MyTCP_MyPCL_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
