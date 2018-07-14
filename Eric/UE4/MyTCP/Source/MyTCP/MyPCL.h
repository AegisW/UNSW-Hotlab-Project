// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "RHI.h"
#include "Networking.h"
#include "MyTCPRecvWorker.h"
#include "MyPCL.generated.h"

UCLASS()
class MYTCP_API AMyPCL : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPCL(const FObjectInitializer& TheComp);
	virtual ~AMyPCL();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FSocket* ListenSocket;
	MyTCPRecvWorker* RecvThread;

	UFUNCTION(BlueprintCallable, Category = "TCP Receiver")
	bool StartTCPReceiver(
			const FString& SocketName,
			const FString& TheIP,
			const int32 ThePort,
			const int32 PacketSize
		);

	FSocket* CreateTCPConnectionListener(
		const FString& SocketName,
		const FString& TheIP,
		const int32 ThePort,
		const int32 ReceiverBufferSize
	);

	TArray<uint8> DataReceived;

	// Texture Update Part
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnTextureUpdated(UTexture2D* PosTexture, UTexture2D* ColorTexture);// , TArray<FColor> PosTextureData, TArray<FColor> ColorTextureData);

	void UpdateTextureRegions(
		UTexture2D* Texture,
		int32 MipIndex,
		uint32 NumRegions,
		FUpdateTextureRegion2D* Regions,
		uint32 SrcPitch,
		uint32 SrcBpp,
		uint8* SrcData,
		bool bFreeData);

	//The size of our textures
	uint32 TextureWidth = 512;
	uint32 TextureHeight = 512;
	uint32 TextureSize = 512;
	int ratio = 3;

	UFUNCTION(BlueprintCallable, Category = "Texture")
	TArray<FColor> GetPosTextureData();

	UFUNCTION(BlueprintCallable, Category = "Texture")
	TArray<FColor> GetColorTextureData();

	//Our texture data
	UPROPERTY()
	TArray<FColor> PosTextureData;

	UPROPERTY()
	TArray<FColor> ColorTextureData;

public:
	//void UpdateTexture();

	//Triggers the start of a new texture-update
	void StartTextureUpdate();

	//Our dynamically updated texture
	UPROPERTY()
	UTexture2D* PosTexture;

	//Texture from last update. 
	UPROPERTY()
	UTexture2D* ColorTexture;

	//Texture regions	
	FUpdateTextureRegion2D* textureRegions;

	UPROPERTY(VisibleAnywhere, category = "Debug")
	bool bHasRecvUpdate = false;

	UPROPERTY(EditAnywhere, category = "Debug")
	float ZeroRatio = 0.5;
	
	UPROPERTY(EditAnywhere, category = "Debug")
	float ColorZeroRatio = 0.9;
};
