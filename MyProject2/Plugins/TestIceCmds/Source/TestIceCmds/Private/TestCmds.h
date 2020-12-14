// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FTestCmds : public TCommands<FTestCmds>
{
public:
	FTestCmds();

	// TCommands<>�Ľӿڣ�ע������
	virtual void RegisterCommands() override;

public:
	//����
	TSharedPtr< FUICommandInfo > CommandA;
	TSharedPtr< FUICommandInfo > CommandB;
};