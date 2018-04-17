#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFGUI\Widget.hpp>
#include "Console.h"
#include <cmath>

namespace TDEngine
{
	namespace Utils
	{
		// Casts string to pointer to base type
		void* CastStringToPointer(TYPE, std::wstring);
		std::wstring CastPointerToWstring(TYPE, void*);
		void  DeletePtr(TYPE, void* ptr);
		std::string WstringToString(std::wstring);
		std::wstring GetTextFromClipboard();
		bool ClipCursorWnd(sf::Window*);
		std::string GetCurrentTimeString();
		sf::Image ResizeImage(const sf::Texture& p_image, float p_width, float p_height);
		const sf::Texture& ResizeTexture(const sf::Texture& p_image, float p_width, float p_height);
		bool CheckMouseCollision(std::shared_ptr<sfg::Widget>, sf::Vector2f& pos);

		template<typename T>
		sf::Vector2<T> GetRectSize(sf::Rect<T> rect)
		{
			return sf::Vector2<T>(abs(rect.width + rect.left), abs(rect.height + rect.top));
		}
	}
}