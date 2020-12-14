// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ContentBrowserModule.h"

class FTestIceCmdsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	TSharedPtr<class FUICommandList> PluginCommandList;
	IConsoleCommand* DisplayTestCommand;
	IConsoleCommand* DisplayUserSpecifiedWindow;//TODO
public:
	void CommandAAction();
	void CommandBAction(FOnContentBrowserGetSelection GetSelectionDelegate);
	void DisplayWindow();
};
