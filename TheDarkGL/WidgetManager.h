#pragma once
#include "Widget.h"
#include <vector>

namespace TDEngine
{

	class WidgetManager
	{
	public:
		WidgetManager(void);
		~WidgetManager(void);

		std::vector<Widget*> ProceedEvent(sf::Event&);
		void AddWidget(Widget&);
		void DeleteWidget(Widget&);
		void DeleteWidget(std::string&);

		const std::vector<Widget*>& GetWidgetList() const
		{
			return Widgets;
		}
		Widget& GetWidget(std::string&);

	private:
		std::vector<Widget*> Widgets;
	};

}