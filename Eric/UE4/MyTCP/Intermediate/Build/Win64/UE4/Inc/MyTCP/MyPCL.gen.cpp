// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "MyPCL.h"
PRAGMA_DISABLE_OPTIMIZATION
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMyPCL() {}
// Cross Module References
	MYTCP_API UFunction* Z_Construct_UFunction_AMyPCL_OnTextureUpdated();
	MYTCP_API UClass* Z_Construct_UClass_AMyPCL();
	ENGINE_API UClass* Z_Construct_UClass_UTexture2D_NoRegister();
	MYTCP_API UFunction* Z_Construct_UFunction_AMyPCL_StartTCPReceiver();
	MYTCP_API UClass* Z_Construct_UClass_AMyPCL_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_MyTCP();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FColor();
// End Cross Module References
	static FName NAME_AMyPCL_OnTextureUpdated = FName(TEXT("OnTextureUpdated"));
	void AMyPCL::OnTextureUpdated(UTexture2D* PosTexture, UTexture2D* ColorTexture)
	{
		MyPCL_eventOnTextureUpdated_Parms Parms;
		Parms.PosTexture=PosTexture;
		Parms.ColorTexture=ColorTexture;
		ProcessEvent(FindFunctionChecked(NAME_AMyPCL_OnTextureUpdated),&Parms);
	}
	void AMyPCL::StaticRegisterNativesAMyPCL()
	{
		UClass* Class = AMyPCL::StaticClass();
		static const TNameNativePtrPair<ANSICHAR> AnsiFuncs[] = {
			{ "OnTextureUpdated", (Native)&AMyPCL::execOnTextureUpdated },
			{ "StartTCPReceiver", (Native)&AMyPCL::execStartTCPReceiver },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, AnsiFuncs, ARRAY_COUNT(AnsiFuncs));
	}
	UFunction* Z_Construct_UFunction_AMyPCL_OnTextureUpdated()
	{
		UObject* Outer = Z_Construct_UClass_AMyPCL();
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("OnTextureUpdated"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), nullptr, (EFunctionFlags)0x08020C00, 65535, sizeof(MyPCL_eventOnTextureUpdated_Parms));
			UProperty* NewProp_ColorTexture = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("ColorTexture"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(ColorTexture, MyPCL_eventOnTextureUpdated_Parms), 0x0010000000000080, Z_Construct_UClass_UTexture2D_NoRegister());
			UProperty* NewProp_PosTexture = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("PosTexture"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(PosTexture, MyPCL_eventOnTextureUpdated_Parms), 0x0010000000000080, Z_Construct_UClass_UTexture2D_NoRegister());
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("MyPCL.h"));
#endif
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_AMyPCL_StartTCPReceiver()
	{
		struct MyPCL_eventStartTCPReceiver_Parms
		{
			FString SocketName;
			FString TheIP;
			int32 ThePort;
			bool ReturnValue;
		};
		UObject* Outer = Z_Construct_UClass_AMyPCL();
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("StartTCPReceiver"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), nullptr, (EFunctionFlags)0x04020401, 65535, sizeof(MyPCL_eventStartTCPReceiver_Parms));
			CPP_BOOL_PROPERTY_BITMASK_STRUCT(ReturnValue, MyPCL_eventStartTCPReceiver_Parms);
			UProperty* NewProp_ReturnValue = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("ReturnValue"), RF_Public|RF_Transient|RF_MarkAsNative) UBoolProperty(FObjectInitializer(), EC_CppProperty, CPP_BOOL_PROPERTY_OFFSET(ReturnValue, MyPCL_eventStartTCPReceiver_Parms), 0x0010000000000580, CPP_BOOL_PROPERTY_BITMASK(ReturnValue, MyPCL_eventStartTCPReceiver_Parms), sizeof(bool), true);
			UProperty* NewProp_ThePort = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("ThePort"), RF_Public|RF_Transient|RF_MarkAsNative) UIntProperty(CPP_PROPERTY_BASE(ThePort, MyPCL_eventStartTCPReceiver_Parms), 0x0010000000000082);
			UProperty* NewProp_TheIP = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("TheIP"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(CPP_PROPERTY_BASE(TheIP, MyPCL_eventStartTCPReceiver_Parms), 0x0010000000000080);
			UProperty* NewProp_SocketName = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("SocketName"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(CPP_PROPERTY_BASE(SocketName, MyPCL_eventStartTCPReceiver_Parms), 0x0010000000000080);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("Category"), TEXT("TCP Receiver"));
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("MyPCL.h"));
			MetaData->SetValue(NewProp_ThePort, TEXT("NativeConst"), TEXT(""));
			MetaData->SetValue(NewProp_TheIP, TEXT("NativeConst"), TEXT(""));
			MetaData->SetValue(NewProp_SocketName, TEXT("NativeConst"), TEXT(""));
#endif
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_AMyPCL_NoRegister()
	{
		return AMyPCL::StaticClass();
	}
	UClass* Z_Construct_UClass_AMyPCL()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_AActor();
			Z_Construct_UPackage__Script_MyTCP();
			OuterClass = AMyPCL::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= (EClassFlags)0x20900080u;

				OuterClass->LinkChild(Z_Construct_UFunction_AMyPCL_OnTextureUpdated());
				OuterClass->LinkChild(Z_Construct_UFunction_AMyPCL_StartTCPReceiver());

				UProperty* NewProp_ZeroRatio = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("ZeroRatio"), RF_Public|RF_Transient|RF_MarkAsNative) UFloatProperty(CPP_PROPERTY_BASE(ZeroRatio, AMyPCL), 0x0010000000000001);
				CPP_BOOL_PROPERTY_BITMASK_STRUCT(bHasRecvUpdate, AMyPCL);
				UProperty* NewProp_bHasRecvUpdate = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("bHasRecvUpdate"), RF_Public|RF_Transient|RF_MarkAsNative) UBoolProperty(FObjectInitializer(), EC_CppProperty, CPP_BOOL_PROPERTY_OFFSET(bHasRecvUpdate, AMyPCL), 0x0010000000020001, CPP_BOOL_PROPERTY_BITMASK(bHasRecvUpdate, AMyPCL), sizeof(bool), true);
				UProperty* NewProp_ColorTexture = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("ColorTexture"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(ColorTexture, AMyPCL), 0x0010000000000000, Z_Construct_UClass_UTexture2D_NoRegister());
				UProperty* NewProp_PosTexture = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("PosTexture"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(PosTexture, AMyPCL), 0x0010000000000000, Z_Construct_UClass_UTexture2D_NoRegister());
				UProperty* NewProp_ColorTextureData = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("ColorTextureData"), RF_Public|RF_Transient|RF_MarkAsNative) UArrayProperty(CPP_PROPERTY_BASE(ColorTextureData, AMyPCL), 0x0010000000000000);
				UProperty* NewProp_ColorTextureData_Inner = new(EC_InternalUseOnlyConstructor, NewProp_ColorTextureData, TEXT("ColorTextureData"), RF_Public|RF_Transient|RF_MarkAsNative) UStructProperty(FObjectInitializer(), EC_CppProperty, 0, 0x0000000000000000, Z_Construct_UScriptStruct_FColor());
				UProperty* NewProp_PosTextureData = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("PosTextureData"), RF_Public|RF_Transient|RF_MarkAsNative) UArrayProperty(CPP_PROPERTY_BASE(PosTextureData, AMyPCL), 0x0010000000000000);
				UProperty* NewProp_PosTextureData_Inner = new(EC_InternalUseOnlyConstructor, NewProp_PosTextureData, TEXT("PosTextureData"), RF_Public|RF_Transient|RF_MarkAsNative) UStructProperty(FObjectInitializer(), EC_CppProperty, 0, 0x0000000000000000, Z_Construct_UScriptStruct_FColor());
				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_AMyPCL_OnTextureUpdated(), "OnTextureUpdated"); // 4046320868
				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_AMyPCL_StartTCPReceiver(), "StartTCPReceiver"); // 1776405693
				static TCppClassTypeInfo<TCppClassTypeTraits<AMyPCL> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("MyPCL.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("MyPCL.h"));
				MetaData->SetValue(OuterClass, TEXT("ObjectInitializerConstructorDeclared"), TEXT(""));
				MetaData->SetValue(NewProp_ZeroRatio, TEXT("Category"), TEXT("Debug"));
				MetaData->SetValue(NewProp_ZeroRatio, TEXT("ModuleRelativePath"), TEXT("MyPCL.h"));
				MetaData->SetValue(NewProp_bHasRecvUpdate, TEXT("Category"), TEXT("Debug"));
				MetaData->SetValue(NewProp_bHasRecvUpdate, TEXT("ModuleRelativePath"), TEXT("MyPCL.h"));
				MetaData->SetValue(NewProp_ColorTexture, TEXT("ModuleRelativePath"), TEXT("MyPCL.h"));
				MetaData->SetValue(NewProp_ColorTexture, TEXT("ToolTip"), TEXT("Texture from last update."));
				MetaData->SetValue(NewProp_PosTexture, TEXT("ModuleRelativePath"), TEXT("MyPCL.h"));
				MetaData->SetValue(NewProp_PosTexture, TEXT("ToolTip"), TEXT("Our dynamically updated texture"));
				MetaData->SetValue(NewProp_ColorTextureData, TEXT("ModuleRelativePath"), TEXT("MyPCL.h"));
				MetaData->SetValue(NewProp_PosTextureData, TEXT("ModuleRelativePath"), TEXT("MyPCL.h"));
				MetaData->SetValue(NewProp_PosTextureData, TEXT("ToolTip"), TEXT("Our texture data"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(AMyPCL, 1408878211);
	static FCompiledInDefer Z_CompiledInDefer_UClass_AMyPCL(Z_Construct_UClass_AMyPCL, &AMyPCL::StaticClass, TEXT("/Script/MyTCP"), TEXT("AMyPCL"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AMyPCL);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
PRAGMA_ENABLE_OPTIMIZATION
