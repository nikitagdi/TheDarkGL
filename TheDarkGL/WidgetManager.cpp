#include "WidgetManager.h"
#include "Game.h"

namespace TDEngine
{

	WidgetManager::WidgetManager(void)
	{
	}


	WidgetManager::~WidgetManager(void)
	{
	}


	std::vector<Widget*> WidgetManager::ProceedEvent(sf::Event& event)
	{
		std::vector<Widget*> widgets;
		for(auto wid : Widgets)
		{
			if(wid->Collide(sf::Mouse::getPosition(*GameEngine.GraphicsEngine.Window)))
			{
				widgets.push_back(wid);
				wid->MouseEvent(event);
			}
		}
		return widgets;
	}


	void WidgetManager::AddWidget(Widget& widget)
	{
		Widgets.push_back(&widget);
	}


	void WidgetManager::DeleteWidget(Widget& widget)
	{
		for(auto it = Widgets.begin(); it != Widgets.end(); it++)
			if(*it == &widget)
				Widgets.erase(it);
	}


	void WidgetManager::DeleteWidget(std::string& widget)
	{
		for(auto it = Widgets.begin(); it != Widgets.end(); it++)
			if((*it)->GetName() == widget)
				Widgets.erase(it);
			else
				GameEngine.ReportError("Widget "+widget+" was not found");
	}


	Widget& WidgetManager::GetWidget(std::string& widget)
	{
		for(auto wid : Widgets)
			if(wid->GetName() == widget)
				return *wid;
			else
				GameEngine.ReportError("Widget "+widget+" was not found");
	}
}