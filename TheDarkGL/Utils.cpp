#include "Utils.h"
#include <Windows.h>
#include <boost/lexical_cast.hpp>

namespace TDEngine
{
	namespace Utils
	{
		
		// Casts string to pointer to base type
		void* CastStringToPointer(TYPE type, std::wstring par)
		{
			void* ret = 0;
			switch(type)
			{
			case BOOL:
				ret = new bool(boost::lexical_cast<bool>(par));
				break;
			case INT:
				ret = new int(boost::lexical_cast<int>(par));
				break;
			case FLOAT:
				ret = new float(boost::lexical_cast<float>(par));
				break;
			case STRING:
				ret = new std::wstring(par);
				break;
			case WCHAR:
			//	ret = new wchar_t(boost::lexical_cast<wchar_t>(par));
				break;
			default:
				ret = 0;
				break;
			}
			return ret;
		}


		std::wstring CastPointerToWstring(TYPE type, void* ptr)
		{
			std::wstring ret = L"";
			switch(type)
			{
			case BOOL:
				ret = boost::lexical_cast<std::wstring>(*(bool*)ptr);
				break;
			case INT:
				ret = boost::lexical_cast<std::wstring>(*(int*)ptr);
				break;
			case FLOAT:
				ret = boost::lexical_cast<std::wstring>(*(float*)ptr);
				break;
			case STRING:
				ret = *(std::wstring*)ptr;
				break;
			case WCHAR:
				ret = *(wchar_t*)ptr;
				break;
			default:
				ret = L"";
				break;
			}
			return ret;
		}


		void DeletePtr(TYPE type, void* ptr)
		{
			switch(type)
			{
			case BOOL:
				delete (bool*)ptr;
				break;
			case INT:
				delete (int*)ptr;
				break;
			case FLOAT:
				delete (float*)ptr;
				break;
			case STRING:
				delete (std::string*)ptr;
				break;
			case WCHAR:
				delete (wchar_t*)ptr;
				break;
			}
		}

		std::string WstringToString(std::wstring wide)
		{
			return std::string( wide.begin(), wide.end() );
		}


		std::wstring GetTextFromClipboard()
		{
			
			std::wstring strClipboardData(L"");

			if (IsClipboardFormatAvailable(CF_UNICODETEXT) && OpenClipboard(NULL))
			{
				HGLOBAL hGlobal = GetClipboardData(CF_UNICODETEXT) ;
				if (hGlobal != NULL) 
				{ 
					wchar_t* lpszData = (wchar_t*) GlobalLock(hGlobal) ;
					if (lpszData != NULL) 
					{ 
						strClipboardData = lpszData ;
						GlobalUnlock(hGlobal) ;
					}
				}
				CloseClipboard() ;
			}

			return strClipboardData ;
		}


		bool ClipCursorWnd(sf::Window* wnd)
		{
			if(wnd)
			{
				RECT rect;
				GetWindowRect(wnd->getSystemHandle(), &rect);
				return ClipCursor(&rect);
			}
		}


		std::string GetCurrentTimeString()
		{
			
			SYSTEMTIME Time;
			GetLocalTime(&Time);
			return boost::lexical_cast<std::string>(Time.wHour)+":"
				+boost::lexical_cast<std::string>(Time.wMinute)+":"
				+boost::lexical_cast<std::string>(Time.wSecond);
		}

		

		sf::Image ResizeImage(const sf::Texture& p_image, float p_width, float p_height)
		{
			sf::Sprite spriteTmp(p_image);
				spriteTmp.scale(p_width / (float)(p_image.getSize().x), p_height / (float)(p_image.getSize().y));
			sf::RenderTexture image;
			image.create(p_width, p_height);
			image.clear(sf::Color(0, 0, 0, 255));
			image.draw(spriteTmp);
			image.display();
			return image.getTexture().copyToImage();
		}


		const sf::Texture& ResizeTexture(const sf::Texture& p_image, float p_width, float p_height)
		{
			sf::Sprite spriteTmp(p_image);
			spriteTmp.scale(p_width / (float)(p_image.getSize().x), p_height / (float)(p_image.getSize().y));
			static sf::RenderTexture image;
			image.create(p_width, p_height);
			image.clear(sf::Color(0, 0, 0, 255));
			image.draw(spriteTmp);
			image.display();
			return image.getTexture();
		}


		bool CheckMouseCollision(std::shared_ptr<sfg::Widget> widget, sf::Vector2f& pos)
		{
			if (!widget->IsGloballyVisible())
				return false;
			if (!widget->GetAllocation().contains(pos))
				return false;
			return true;
		}

	}
}