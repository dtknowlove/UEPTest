// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestIceCmds.h"
#include "TestCmds.h"

#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Misc/MessageDialog.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE "FTestIceCmdsModule"

void FTestIceCmdsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FTestCmds::Register();

	PluginCommandList = MakeShareable(new FUICommandList);
	//为命令映射操作
	PluginCommandList->MapAction(FTestCmds::Get().CommandA, FExecuteAction::CreateRaw(this, &FTestIceCmdsModule::CommandAAction), FCanExecuteAction());

	//获得内容浏览器模块
	FContentBrowserModule& ContentBrowserModule = FModuleManager::GetModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FContentBrowserCommandExtender>& CBCommandExtenderDelegates = ContentBrowserModule.GetAllContentBrowserCommandExtenders();
	CBCommandExtenderDelegates.Add(FContentBrowserCommandExtender::CreateLambda([this](TSharedRef<FUICommandList> CommandList, FOnContentBrowserGetSelection GetSelectionDelegate)
		{
			CommandList->MapAction(FTestCmds::Get().CommandB, FExecuteAction::CreateLambda([this, GetSelectionDelegate]
				{
					CommandBAction(GetSelectionDelegate);
				}
			));
		}
	));
	
	//获得关卡编辑器模块
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	//将CommandA添加入关卡编辑器的GlobalLevelEditorActions中，这样可以触发快捷键
	TSharedPtr<FUICommandList> LevelEditorCommandList = LevelEditorModule.GetGlobalLevelEditorActions();
	LevelEditorCommandList->MapAction(FTestCmds::Get().CommandA, FExecuteAction::CreateRaw(this, &FTestIceCmdsModule::CommandAAction), FCanExecuteAction());

	//扩展关卡编辑器的菜单
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommandList, FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& Builder) 
		{
			Builder.AddMenuEntry(FTestCmds::Get().CommandA);
		}
	));
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	//扩展关卡编辑器的工具栏
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender());
	ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommandList, FToolBarExtensionDelegate::CreateLambda([](FToolBarBuilder& Builder)
		{
			Builder.AddToolBarButton(FTestCmds::Get().CommandA);
		}
	));
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

	//添加运行时命令行
	DisplayTestCommand = IConsoleManager::Get().RegisterConsoleCommand(TEXT("IceCmd_PrintStr"), TEXT("IceCmd"), FConsoleCommandDelegate::CreateRaw(this,&FTestIceCmdsModule::DisplayWindow), ECVF_Default);
	
}

void FTestIceCmdsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FTestCmds::Unregister();

	if (DisplayTestCommand)
	{
		IConsoleManager::Get().UnregisterConsoleObject(DisplayTestCommand);
		DisplayTestCommand = nullptr;
	}
}


void FTestIceCmdsModule::CommandAAction()
{
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Execute CommandA!"));
}


void FTestIceCmdsModule::CommandBAction(FOnContentBrowserGetSelection GetSelectionDelegate)
{
	TArray<FAssetData> SelectedAssets;
	TArray<FString> SelectedPaths;
	if (GetSelectionDelegate.IsBound())
		GetSelectionDelegate.Execute(SelectedAssets, SelectedPaths);

	FString Message = "Execute CommandB:";
	for (auto ad:SelectedAssets)
	{
		Message += ad.GetAsset()->GetName() + " ";
	}
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Message));
}

void FTestIceCmdsModule::DisplayWindow()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sucessed print by command"));

	FString WindowTitle = FString(TEXT("Test Command Window"));
	TSharedRef<SWindow> CookbookWindow = SNew(SWindow)
		.Title(FText::FromString(WindowTitle))
		.ClientSize(FVector2D(800,400))
		.SupportsMaximize(false)
		.SupportsMinimize(false);
	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked< IMainFrameModule>(TEXT("MainFrame"));
	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(CookbookWindow, MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(CookbookWindow);
	}

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTestIceCmdsModule, TestIceCmds)