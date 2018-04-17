#include "EventManager.h"
#include "Game.h"
//#include <type.h>
#include <algorithm>

extern TDEngine::GameManager TDEngine::GameEngine;

namespace TDEngine
{

	EventReceiver::~EventReceiver()
	{
		GameEngine.EventEngine.Unsubscribe(this);
	}

	//All events have to be defined at constructor, or registered with void RegisterEvent()
	EventManager::EventManager()
	{
		RegisterEvent("event_changeColor", std::vector<TYPE>(1,CODE));  // CODE is pointer to int (*)()
		RegisterEvent("event_changeTile", std::vector<TYPE>(1,STRING));
		RegisterEvent("event_buttonPressed", std::vector<TYPE>(1,INT)); // INT is sf::Keyboard::Key
		RegisterEvent("event_buttonReleased", std::vector<TYPE>(1, INT)); // INT is sf::Keyboard::Key
		RegisterEvent("event_mouseWheelTurned", std::vector<TYPE>(1, INT)); // INT is delta value for mouse wheel
		RegisterEvent("event_Update", std::vector<TYPE>(1, FLOAT)); // FLOAT is time from last update
		RegisterEvent("event_updateEntity", std::vector<TYPE>(1,INT));  // No params for this event
	}


	void EventManager::RegisterEvent(std::string name, std::vector<TYPE>& params)
	{
		Event event;
		event.name = name;
		for(auto param : params)
			event.Parameters.push_back(std::pair<TYPE,void*>(param, 0));
		Events.push_back(event);
		Subscribers[event.name];
	}

	bool EventManager::DoesEventExist(std::string name)
	{
		for(auto ev : Events)
			if(ev.name == name)
				return true;
			return false;
	}

	void EventManager::Subscribe(EventReceiver* receiver, std::string event)
	{
		try
		{
			Subscribers.at(event).push_back(receiver);
		}
		catch(std::out_of_range&)
		{
			GameEngine.ReportError("Event "+event+" doesn't exist");
		}
	}


	void EventManager::Unsubscribe(EventReceiver* receiver)
	{
		for (auto event : Events)
		{
			try
			{
				std::vector<EventReceiver*>& container = Subscribers.at(event.name);
				std::vector<EventReceiver*>::iterator position = std::find(container.begin(), container.end(), receiver);
				if (position != container.end()) // == myVector.end() means the element was not found
					container.erase(position);
			}
			catch (std::out_of_range&)
			{
				GameEngine.ReportError("Error occured while unsubscribing. Event " + event.name + " doesn't exist");
			}
		}
	}


	void EventManager::Unsubscribe(EventReceiver* receiver, std::string event)
	{
		try
		{
			std::vector<EventReceiver*> &receivers = Subscribers.at(event);
			receivers.erase(std::remove(receivers.begin(), receivers.end(), receiver), receivers.end());
		}
		catch(std::out_of_range&)
		{GameEngine.ReportError("Event "+event+" doesn't exist");}
	}


	void EventManager::SendEvent(EventReceiver* receiver, Event& event)
	{
		if(receiver)
			receiver->ProceedEvent(event);
	}


	void EventManager::SendEvent(Event& event)
	{
		try
		{
			for(auto subscriber : Subscribers.at(event.name))
				if(subscriber)
					subscriber->ProceedEvent(event);
		}
		catch(std::out_of_range&)
		{GameEngine.ReportError("Event "+event.name+" doesn't exist");}
	}


	void EventManager::SendGlobalEvent(Event& event)
	{
		for(auto subscribers : Subscribers)
			for(auto subscriber : subscribers.second)
				subscriber->ProceedEvent(event);
	}


	Event EventManager::GetEventPattern(std::string name)
	{
		Event default;
		default.name = "";
		for(auto event : Events)
			if(event.name == name)
				return event;
		return default;
	}


	bool EventManager::IsObjectSubscribed(EventReceiver* receiver, std::string event)
	{
		try
		{
			for(auto subscriber : Subscribers.at(event))
				if(subscriber == receiver)
					return true;
		}
		catch(std::out_of_range&)
		{GameEngine.ReportError("Event "+event+" doesn't exist");}
		return false;
	}
}