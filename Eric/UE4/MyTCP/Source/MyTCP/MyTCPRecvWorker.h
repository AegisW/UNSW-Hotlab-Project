// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"

/**
 * 
 */
class AMyPCL;

class MYTCP_API MyTCPRecvWorker : public FRunnable
{
	/** Holds the network socket. */
	FSocket* Socket;

	/** Holds a pointer to the socket sub-system. */
	ISocketSubsystem* SocketSubsystem;

	/** Holds a flag indicating that the thread is stopping. */
	bool Stopping;

	/** Holds the thread object. */
	FRunnableThread* Thread;

	/** The receiver thread's name. */
	FString ThreadName;

	/** Holds the amount of time to wait for inbound packets. */
	FTimespan WaitTime;

	AMyPCL* Manager;

public:
	MyTCPRecvWorker(
		AMyPCL* manager,
		FSocket* InSocket,
		const FTimespan& InWaitTime,
		const TCHAR* InThreadName,
		const int32 PacketSize
	);

	~MyTCPRecvWorker();

	//FRunnable interface
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	//End of FRunnable interface

public:
	void Start();

	void ShutDown();

	void TCPConnectionListener();
	TArray<uint8> TCPSocketListener();

	FSocket* ConnectionSocket;

	int32 RecvMsgSize;

	int TotalPack;

	int PackSize;

	bool IsFinished = false;

	FString StringFromBinaryArray(TArray<uint8> BinaryArray);

	TArray<uint8> LastFrameData;

	TArray<uint8> TextureData;

	int ZeroCount(TArray<uint8> InData);

	struct MyDataToUpdate {
		TArray<uint8> z;
		TArray<uint8> r;
		TArray<uint8> g;
		TArray<uint8> b;
		TArray<uint8> last_z;
		TArray<uint8> last_r;
		TArray<uint8> last_g;
		TArray<uint8> last_b;
		bool useLastFrame;
	} DataToUpdate;

};
