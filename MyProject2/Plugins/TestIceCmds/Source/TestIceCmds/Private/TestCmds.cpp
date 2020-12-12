// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCmds.h"

#define LOCTEXT_NAMESPACE "FTestIceCmdsModule"

FTestCmds::FTestCmds() : TCommands<FTestCmds>(
	"TestYaksueCommands",
	NSLOCTEXT("Contexts", "TestIceCmds", "TestIce Plugin"),
	NAME_None,
	FName(*FString("todo")))
{
}

void FTestCmds::RegisterCommands()
{
	UI_COMMAND(CommandA, "TestIceCommandA", "Execute TestIce CommandA", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Shift | EModifierKey::Alt, EKeys::Z));
	UI_COMMAND(CommandB, "TestIceCommandB", "Execute TestIce CommmandB", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Shift | EModifierKey::Alt, EKeys::X));
}

#undef LOCTEXT_NAMESPACE
