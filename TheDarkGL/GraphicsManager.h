#pragma once
//#include <gl/glew.h>
#include <SFML/Graphics.hpp>
#include <sFML/OpenGL.hpp>
#include <sfgui/SFGUI.hpp>
#include <SFGUI\Desktop.hpp>
#include <gl\GLU.h>
#include <map>
#include <hash_map>
#include <queue>
#include <string>
#include <mutex>
#include "Animation.h"

namespace TDEngine
{
	class GraphicsManager
	{
		friend class GameManager;
	public:
		GraphicsManager();
		~GraphicsManager();

		struct Tileset
		{
			sf::Vector2f size;
			std::string name;
			std::vector<std::string> tiles;
		};

		void SetWindowParams(unsigned int Width = 640, unsigned int Height = 480, unsigned int FPS = 6000,
			bool Fullscreen = 0, std::string Title = "");
		bool UpdateWindow();
		bool PollWinEvent(sf::Event&);
		bool WaitWinEvent(sf::Event&);
		void UpdateOpenGL(sf::VideoMode = sf::VideoMode(640, 480), std::string = "The Dark!", bool = 0, int = 60);
		void RenderText(std::string text, sf::Vector2f pos, sf::Font& font, sf::Color& color = sf::Color(255, 255, 255), unsigned int size = 30);
		void RenderGUI();
		void InitGraphics();
		void InitGraphicsMT();  //Multithreaded version
		void Render();

		sf::Font& GetFont(std::string);
		sf::Font& GetDefaultFont();
		sf::Texture* GetTexture(std::string, sf::Vector2u size = sf::Vector2u(16U,16U));
		Animation GetAnimation(std::string) const;
		std::string GetTextureName(const sf::Texture*);
		bool LoadFonts(std::string);
		void LoadTextures(std::string);

		unsigned int getWidth()  const {return WinWidth;}
		unsigned int getHeight() const {return WinHeight;}
		const std::vector<Tileset>& getTilesets() const {return Tilesets;}

		sf::RenderWindow*  Window;
		static sfg::Desktop desktop;
		float ViewX, ViewY;
		float frameInterval;
		static sfg::SFGUI       sfgui;

		static std::mutex gui_mutex;
	private:
		
		void RenderFPS();
		void LoadAnimations();

		unsigned int      WinWidth, WinHeight, WinFPS;
		bool              WinFullscreen;
		
		std::string       WinTitle;
		std::thread* RenderThread;

		std::map<std::string, sf::Font> Fonts;
		std::hash_map<std::string, std::vector<sf::Texture*>> Textures;
		std::hash_map<std::string, Animation> Animations;

		std::vector<Tileset> Tilesets;
	};
}