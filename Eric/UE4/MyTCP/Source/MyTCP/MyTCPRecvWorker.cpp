// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTCPRecvWorker.h"
#include "MyPCL.h"
#include <Engine.h>
#include <string>

MyTCPRecvWorker::MyTCPRecvWorker(AMyPCL* manager, FSocket* InSocket, const FTimespan& InWaitTime, const TCHAR* InThreadName, const int32 PacketSize)
	: Manager(manager)
	, Socket(InSocket)
	, Stopping(false)
	, Thread(nullptr)
	, ThreadName(InThreadName)
	, WaitTime(InWaitTime)
	, PackSize(PacketSize)
{
	check(Socket != nullptr);

	SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	ConnectionSocket = NULL;

	RecvMsgSize = 0;
	//PackSize = (int)Manager->PacketSize;

	LastFrameData.Init(0, PackSize);

	DataToUpdate.z.Init(0, PackSize);
	DataToUpdate.r.Init(0, PackSize);
	DataToUpdate.g.Init(0, PackSize);
	DataToUpdate.b.Init(0, PackSize);
	DataToUpdate.last_z.Init(0, PackSize);
	DataToUpdate.last_r.Init(0, PackSize);
	DataToUpdate.last_g.Init(0, PackSize);
	DataToUpdate.last_b.Init(0, PackSize);
	DataToUpdate.useLastFrame = false;

	IsFinished = false;
}

MyTCPRecvWorker::~MyTCPRecvWorker()
{
	if (Thread != nullptr)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

void MyTCPRecvWorker::Stop()
{
	Stopping = true;
}

void MyTCPRecvWorker::Start()
{
	Thread = FRunnableThread::Create(this, *ThreadName, 0u, TPri_BelowNormal);
}

bool MyTCPRecvWorker::Init()
{
	if (Manager)
	{
		return true;
	}
	return false;
}

void MyTCPRecvWorker::ShutDown()
{
	Stop();
	Thread->WaitForCompletion();
}

uint32 MyTCPRecvWorker::Run()
{
	while (!Stopping)
	{
		if (!Socket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
		{
			continue;
		}

		TCPConnectionListener();
		if (ConnectionSocket)
		{
			TArray<uint8_t> Reader = TCPSocketListener();
			//const FString Msg = StringFromBinaryArray(Reader);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Message received: %s"), *Msg));
			
			if (!Manager->bHasRecvUpdate && Manager->PosTexture && Manager->ColorTexture)
			{
				int stride = PackSize;
				
				/*if (DataToUpdate.useLastFrame == true) {
					for (int i = 1; i < PackSize; i++) {
						//if (Reader[0] == 'x') Manager->PosTextureData[i-1].R = Reader[i];
						//if (Reader[0] == 'y') Manager->PosTextureData[i-1].G = Reader[i];
						Manager->PosTextureData[i - 1].B = DataToUpdate.last_z[i];
						Manager->ColorTextureData[i - 1].R = DataToUpdate.last_r[i];
						Manager->ColorTextureData[i - 1].G = DataToUpdate.last_g[i];						
						Manager->ColorTextureData[i - 1].B = DataToUpdate.last_b[i];
					}					
					Manager->bHasRecvUpdate = true;
					DataToUpdate.useLastFrame = false;
					IsFinished = false;
					//Set to Read Only
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("j: %d"), j));
				}
				else if (DataToUpdate.useLastFrame == false) {
					for (int i = 1; i < PackSize; i++) {
						//if (Reader[0] == 'x') Manager->PosTextureData[i-1].R = Reader[i];
						//if (Reader[0] == 'y') Manager->PosTextureData[i-1].G = Reader[i];
						Manager->PosTextureData[i - 1].B = DataToUpdate.z[i];
						Manager->ColorTextureData[i - 1].R = DataToUpdate.r[i];
						Manager->ColorTextureData[i - 1].G = DataToUpdate.g[i];
						Manager->ColorTextureData[i - 1].B = DataToUpdate.b[i];
					}
					Manager->bHasRecvUpdate = true;
					IsFinished = false;
					//Set to Read Only
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("j: %d"), j));
				}*/	
				for (int i = 1; i < PackSize; i++) {
					//if (Reader[0] == 'x') Manager->PosTextureData[i-1].R = Reader[i];
					//if (Reader[0] == 'y') Manager->PosTextureData[i-1].G = Reader[i];
					if (Reader[0] == 'z') Manager->PosTextureData[i - 1].B = Reader[i];
					if (Reader[0] == 'r') Manager->ColorTextureData[i - 1].R = Reader[i];
					if (Reader[0] == 'g') Manager->ColorTextureData[i - 1].G = Reader[i];
					if (Reader[0] == 'b') {
						Manager->ColorTextureData[i - 1].B = Reader[i];
						IsFinished = true;
					}
				}
				if(IsFinished)
				{
					Manager->bHasRecvUpdate = true; //Set to Read Only
					//DataToUpdate.useLastFrame = false;
					IsFinished = false;
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("j: %d"), j));
				}
			}
		}
	}

	return 0;
}

void MyTCPRecvWorker::TCPConnectionListener()
{
	if (!Socket) return;

	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending;

	if (Socket->HasPendingConnection(Pending) && Pending)
	{
		if (ConnectionSocket)
		{
			ConnectionSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
		}

		ConnectionSocket = Socket->Accept(*RemoteAddress, TEXT("TCP Received Socket Connection"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Socket Accepted! Waiting for data...")));
	}
}

/*void MyTCPRecvWorker::TCPSocketListener()
{
	uint32 Size = 0;
	TArray<uint8> ReceivedData;

	ReceivedData.Init(0, PackSize);
	if (ConnectionSocket->HasPendingData(Size)) {
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), RecvMsgSize);

		if (ReceivedData[0] == 'z') {
			DataToUpdate.z = ReceivedData;
			if (ZeroCount(DataToUpdate.z) > (PackSize * Manager->ZeroRatio)) DataToUpdate.useLastFrame = true;
		}
		else if (ReceivedData[0] == 'r') {
			DataToUpdate.r = ReceivedData;
			if (ZeroCount(DataToUpdate.r) > (PackSize * Manager->ColorZeroRatio)) DataToUpdate.useLastFrame = true;
		}
		else if (ReceivedData[0] == 'g') {
			DataToUpdate.g = ReceivedData;
			if (ZeroCount(DataToUpdate.g) > (PackSize * Manager->ColorZeroRatio)) DataToUpdate.useLastFrame = true;
		}
		else if (ReceivedData[0] == 'b') {
			DataToUpdate.b = ReceivedData;
			if (ZeroCount(DataToUpdate.b) > (PackSize * Manager->ColorZeroRatio)) DataToUpdate.useLastFrame = true;
			IsFinished = true;
		}
		if (IsFinished && DataToUpdate.useLastFrame == false) {
			DataToUpdate.last_z = DataToUpdate.z;
			DataToUpdate.last_r = DataToUpdate.r;
			DataToUpdate.last_g = DataToUpdate.g;
			DataToUpdate.last_b = DataToUpdate.b;
		}
	}
}*/

TArray<uint8> MyTCPRecvWorker::TCPSocketListener()
{
	uint32 Size = 0;
	TArray<uint8> ReceivedData;

	ReceivedData.Init(0, PackSize);
	if (ConnectionSocket->HasPendingData(Size)) {
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), RecvMsgSize);
		if (ZeroCount(ReceivedData) < PackSize*Manager->ZeroRatio) LastFrameData = ReceivedData;
		else ReceivedData = LastFrameData;
	}
	return ReceivedData;
}

FString MyTCPRecvWorker::StringFromBinaryArray(TArray<uint8> BinaryArray)
{

	//Create a string from a byte array!
	const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	return FString(cstr.c_str());
}

int MyTCPRecvWorker::ZeroCount(TArray<uint8> InData)
{
	int count = 0;

	for (int i = 0; i < InData.Num(); i++)
	{
		if (!InData[i]) count++;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Zeros: %d"), count));
	return count;
}
