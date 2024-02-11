#include "pch.h"
#include "Game.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

ref class App sealed : public IFrameworkView
{
	bool didCloseWindow ;
	CGame Game;
	
public:
	virtual void Initialize(CoreApplicationView^ appView)
	{
		didCloseWindow = false;
		appView->Activated += ref new TypedEventHandler<CoreApplicationView^,IActivatedEventArgs^>(this, &App::OnActivated);
		CoreApplication::Suspending += ref new Windows::Foundation::EventHandler<Windows::ApplicationModel::SuspendingEventArgs^>(this, &App::OnSuspending);
		CoreApplication::Resuming += ref new Windows::Foundation::EventHandler<Platform::Object^>(this, &App::OnResuming);
	}

	virtual void SetWindow(CoreWindow^ Window)
	{
		Window->PointerPressed += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(this, &App::PointerPressed);
		Window->PointerWheelChanged += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(this, &App::OnPointerWheelChanged);
		Window->KeyDown += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &App::OnKeyDown);
		Window->Closed += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::CoreWindowEventArgs^>(this, &App::OnClosed);
	}
	virtual void Load(String^ EntryPoint)
	{

	}
	virtual void Run()
	{
		Game.Initialize();
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		//run Process Events
		while (!didCloseWindow)
		{
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			Game.Update();
			Game.Render();
		}

		
	}
	virtual void Uninitialize()
	{

	}
	void OnActivated(CoreApplicationView^ sender, IActivatedEventArgs^ args)
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		Window->Activate();
			
	}

	//Input Event Handlers

	void PointerPressed(CoreWindow^ Window, PointerEventArgs^ Args)
	{
		MessageDialog Dialog("", "");
		Dialog.Content = "X : " + Args->CurrentPoint->Position.X.ToString() + " " + "Y : " + Args->CurrentPoint->Position.Y.ToString();
		Dialog.Title = "Notice";
		Dialog.ShowAsync();
	}
	void OnClosed(CoreWindow^ sender, CoreWindowEventArgs^ args )
	{
		didCloseWindow = true;
	}
	void OnKeyDown(CoreWindow^ Window, KeyEventArgs^ Args)
	{
		MessageDialog dialog("", "");

		if (Args->VirtualKey == VirtualKey::W)
		{
			dialog.Content = "Move forward";
			dialog.Title = "W Pressed";
			dialog.ShowAsync();

		}
		else if (Args->VirtualKey == VirtualKey::A)
		{
			dialog.Content = "Move Left";
			dialog.Title = "A Pressed";
			dialog.ShowAsync();
		}
		else if(Args->VirtualKey == VirtualKey::S)
		{
			dialog.Content = "Move Back";
			dialog.Title = "S Pressed";
			dialog.ShowAsync();
		}
		else if (Args->VirtualKey == VirtualKey::D)
		{
			dialog.Content = " Move Right";
			dialog.Title = "D pressed";
			dialog.ShowAsync();
		}
	}

	void OnPointerWheelChanged(CoreWindow^ Window, PointerEventArgs^ Args)
	{
		int wheel = Args->CurrentPoint->Properties->MouseWheelDelta;
		MessageDialog dialog("", "Mouse Wheel Event");
		dialog.Content = wheel.ToString();
		dialog.ShowAsync();
	}
	void OnSuspending(Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args)
	{

	}
	void OnResuming(Platform::Object^ sender, Platform::Object^ args)
	{

	}
};

ref class AppSource sealed : IFrameworkViewSource
{
public:

	virtual IFrameworkView^ CreateView()
	{
		return ref new App();

	}
};

int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new AppSource());
	return 0;
}

