// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPCL.h"


// Sets default values
AMyPCL::AMyPCL(const FObjectInitializer& TheComp)
	: Super(TheComp)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ListenSocket = NULL;
	RecvThread = NULL;

	textureRegions = new FUpdateTextureRegion2D(0, 0, 0, 0, TextureWidth, TextureHeight);
}

AMyPCL::~AMyPCL()
{
	if (RecvThread) RecvThread->ShutDown();
}

// Called when the game starts or when spawned
void AMyPCL::BeginPlay()
{
	Super::BeginPlay();
	
	AMyPCL::StartTextureUpdate();
}

// Called every frame
void AMyPCL::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PosTexture && ColorTexture && bHasRecvUpdate) {
		ColorTexture->UpdateResource();
		UpdateTextureRegions(ColorTexture, (int32)0, (uint32)1, textureRegions, (uint32)(4 * TextureWidth), (uint32)4, (uint8*)ColorTextureData.GetData(), false);
		PosTexture->UpdateResource();
		UpdateTextureRegions(PosTexture, (int32)0, (uint32)1, textureRegions, (uint32)(4 * TextureWidth), (uint32)4, (uint8*)PosTextureData.GetData(), false);
		bHasRecvUpdate = false;
		//Trigger the blueprint update
		OnTextureUpdated(PosTexture, ColorTexture);// , PosTextureData, ColorTextureData);
	}
}

void AMyPCL::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	RecvThread->ShutDown();
	delete RecvThread;
	RecvThread = nullptr;

	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}
}

bool AMyPCL::StartTCPReceiver(
	const FString& SocketName,
	const FString& TheIP,
	const int32 ThePort,
	const int32 PacketSize
)
{
	ListenSocket = CreateTCPConnectionListener(SocketName, TheIP, ThePort, PacketSize);

	if (!ListenSocket) return false;

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(8);

	RecvThread = new MyTCPRecvWorker(this, ListenSocket, ThreadWaitTime, TEXT("TestSocket"), PacketSize);
	RecvThread->Start();

	return true;
}

FSocket* AMyPCL::CreateTCPConnectionListener(const FString& SocketName, const FString& TheIP, const int32 ThePort, const int32 ReceiverBufferSize)
{
	FIPv4Address Addr;
	FIPv4Address::Parse(TheIP, Addr);

	FIPv4Endpoint Endpoint(Addr, ThePort);

	ListenSocket = FTcpSocketBuilder(*SocketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(8);

	int32 NewSize;
	ListenSocket->SetReceiveBufferSize(ReceiverBufferSize, NewSize);

	return ListenSocket;
}

/**
* Texture update part
*/
void AMyPCL::StartTextureUpdate() {
	if (!PosTexture && !ColorTexture) {
		PosTexture = UTexture2D::CreateTransient(TextureWidth, TextureHeight);
		ColorTexture = UTexture2D::CreateTransient(TextureWidth, TextureHeight);
		int arraySize = TextureWidth * TextureHeight;
		PosTextureData.Init(FColor(0, 0, 0, 255), arraySize);
		ColorTextureData.Init(FColor(0, 0, 0, 255), arraySize);
		DataReceived.Init(0, arraySize);
	}
}

void AMyPCL::UpdateTextureRegions(UTexture2D* MyTexture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{

	if (MyTexture && (PosTexture->Resource || ColorTexture->Resource))
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)MyTexture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}
		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}

void AMyPCL::OnTextureUpdated_Implementation(UTexture2D* PosTexture, UTexture2D* ColorTexture) {// , TArray<FColor> PosTextureData, TArray<FColor> ColorTextureData) {
	//Handle in blueprint
}

TArray<FColor> AMyPCL::GetPosTextureData()
{
	return PosTextureData;
}

TArray<FColor> AMyPCL::GetColorTextureData()
{
	return ColorTextureData;
}
