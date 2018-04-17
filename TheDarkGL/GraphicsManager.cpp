#include "GraphicsManager.h"
#include "Game.h"
#include "Utils.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "INI1.31.h"


std::mutex TDEngine::GraphicsManager::gui_mutex;
sfg::SFGUI TDEngine::GraphicsManager::sfgui;
sfg::Desktop TDEngine::GraphicsManager::desktop;
extern TDEngine::GameManager TDEngine::GameEngine;


TDEngine::GraphicsManager::GraphicsManager()
{
	//SetWindowParams();

}


TDEngine::GraphicsManager::~GraphicsManager()
{
	//for(auto texture : Textures)
	//	if(texture.second)
	//		delete texture.second;
}


void TDEngine::GraphicsManager::InitGraphics()
{
	SetWindowParams(TDEngine::GameEngine.GetSetting<int>("Width"), TDEngine::GameEngine.GetSetting<int>("Height"),
		0, TDEngine::GameEngine.GetSetting<int>("FullscreenMode"), TDEngine::GameEngine.GetSetting<std::string>("WindowTitle"));
	UpdateWindow();

	GameEngine.LogEngine.LogInfo("Loading fonts.", "Graphics engine");
	LoadFonts(TDEngine::GameEngine.GetSetting<std::string>("Fonts"));
	GameEngine.LogEngine.LogInfo("Fonts have been loaded.", "Graphics engine");

	Window->pushGLStates();
	GameEngine.LogEngine.LogInfo("Loading textures.", "Graphics engine");
	LoadTextures(TDEngine::GameEngine.GetSetting<std::string>("Textures"));
	GameEngine.LogEngine.LogInfo("Textures have been loaded.", "Graphics engine");
	Window->popGLStates();
	
	
	GameEngine.LogEngine.LogInfo("Loading Animations.", "Graphics engine");
	LoadAnimations();
	GameEngine.LogEngine.LogInfo("Animations have been loaded.", "Graphics engine");
	//sfgui = new sfg::SFGUI();
	//desktop = new sfg::Desktop();
	desktop.GetEngine().GetResourceManager().SetDefaultFont(std::make_shared<sf::Font>(GetFont("arial")));
}


//TODO: Refactor or remove this shit
void TDEngine::GraphicsManager::InitGraphicsMT()
{
	bool initFlag = false;

	RenderThread = new std::thread([&]() {

		InitGraphics();
		initFlag = true;
		Sleep(2000);

		while (true)
		{
			Render();
		}
	});

	while (!initFlag) { Sleep(5); }
}


void TDEngine::GraphicsManager::Render()
{
		Window->clear();

		auto gameState = TDEngine::GameEngine.GetCurrentGameState();
		if (gameState)
			gameState->renderScene();

		//Rendering FPS counter and console
		Window->pushGLStates();
		Window->setView(sf::View(sf::FloatRect(0.f, 0.f, getWidth(), getHeight())));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (TDEngine::GameEngine.IsConsoleActive)
			TDEngine::GameEngine.console.RenderConsole();
		//if (!TDEngine::GameEngine.IsConsoleActive)
		//	RenderFPS();

		RenderText("FPS: " + boost::lexical_cast<std::string>(1000/(int)TDEngine::GameEngine.getElapsedTime())
			+ "\nFrame time: " + boost::lexical_cast<std::string>(TDEngine::GameEngine.getElapsedTime())
			+ "\nLogic time: " + boost::lexical_cast<std::string>(TDEngine::GameEngine.getLogicTime())
			+ "\nRender time: " + boost::lexical_cast<std::string>(TDEngine::GameEngine.getRenderTime()),
			sf::Vector2f(0.f, 0.f), GetFont("arial"), sf::Color(255.f, 0.f, 0.f), 24U);

		Window->popGLStates();

		//Rendering sfgui
		TDEngine::GameEngine.GraphicsEngine.Window->pushGLStates();
		TDEngine::GameEngine.GraphicsEngine.Window->setView(sf::View(sf::FloatRect(0.f, 0.f, TDEngine::GameEngine.GraphicsEngine.getWidth(), TDEngine::GameEngine.GraphicsEngine.getHeight())));
		TDEngine::GameEngine.GraphicsEngine.RenderGUI();
		TDEngine::GameEngine.GraphicsEngine.Window->popGLStates();

		desktop.Update(1.f / 60.f);
		Window->display();
}


void TDEngine::GraphicsManager::RenderFPS()
{
	static float fpst = 0, prevTicks = 0, fps = 0, frameTime = 0;
	static sf::Clock clock;
	float currentTime = clock.getElapsedTime().asSeconds();

	frameInterval = currentTime - frameTime;
	frameTime = currentTime;

	fpst++;
	if (currentTime - prevTicks > 1.0f)
	{
		prevTicks = currentTime;
		fps = fpst;
		fpst = 0;
	}
	sf::Text text("FPS: " + boost::lexical_cast<std::string>((int)fps), GetFont("arial"));
	text.setColor(sf::Color(255, 0, 0));
	Window->draw(text);

}


void TDEngine::GraphicsManager::SetWindowParams(unsigned int Width, unsigned int Height,
									unsigned int FPS,bool Fullscreen, std::string Title)
{
	WinWidth	  = Width;
	WinHeight	  = Height;
	WinFPS		  = FPS;
	WinFullscreen = Fullscreen;
	WinTitle      = Title;
}




bool TDEngine::GraphicsManager::UpdateWindow()
{
	Window = new sf::RenderWindow(sf::VideoMode(WinWidth, WinHeight), WinTitle, 
		WinFullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar);

	if(!Window)
		return false;

	Window->setFramerateLimit(WinFPS);
	return true;
}


bool TDEngine::GraphicsManager::PollWinEvent(sf::Event& event)
{
	return Window->pollEvent(event);
}


bool TDEngine::GraphicsManager::WaitWinEvent(sf::Event& event)
{
	return Window->waitEvent(event);
}


void TDEngine::GraphicsManager::RenderGUI()
{
	//std::lock_guard<std::mutex> guard(gui_mutex);
	if (gui_mutex.try_lock())
	{
		sfgui.Display(*Window);
		gui_mutex.unlock();
	}
}


bool TDEngine::GraphicsManager::LoadFonts(std::string folder)
{
	using namespace boost::filesystem;
	std::vector<path> files;
	sf::Font tmpFont;
	copy(directory_iterator(folder), directory_iterator(), std::back_inserter(files)); 
	for(auto file : files)
	{
		if(file.extension() == ".ttf" || file.extension() == ".TTF")
			if(!tmpFont.loadFromFile(file.string()))
			return false;
		Fonts[file.stem().generic_string()]=tmpFont;
	}
	return true;
}


sf::Font& TDEngine::GraphicsManager::GetFont(std::string font)
{
	try
	{
		return Fonts.at(font);
	}
	catch(std::out_of_range&)
	{
		return GetDefaultFont();
	}
}


sf::Texture* TDEngine::GraphicsManager::GetTexture(std::string texture, sf::Vector2u size)
{
	try
	{
		std::vector<sf::Texture*>& textures = Textures.at(texture);
		auto res = std::find_if(textures.begin(), textures.end(), [&size](const sf::Texture* tx)->bool {return tx->getSize() == size;});
		if (res == textures.end())
		{
			//GameEngine.LogEngine.LogInfo("Resized");
			sf::Texture* tex = const_cast<sf::Texture*>(&TDEngine::Utils::ResizeTexture(*textures[0], size.x, size.y));
			textures.push_back(tex);
			return tex;
		}
		return *res;
	}
	catch(std::out_of_range&)
	{
		return Textures["Default"][0];
	}
}

sf::Font& TDEngine::GraphicsManager::GetDefaultFont()
{
	return Fonts["Default"];
}


void TDEngine::GraphicsManager::LoadTextures(std::string folder)
{
	using namespace boost::filesystem;
	std::vector<path> files;
	auto tmpTexture = new sf::Texture;
	std::string filename;
	copy(directory_iterator(folder), directory_iterator(), std::back_inserter(files)); 
	for(auto file : files)
	{
		filename = file.stem().generic_string();
		if(filename.find("tileset_") != std::string::npos)
		{
			std::string tmp, tilesetName("");
			Tileset tileset;
			int x = 0, y = 0, pos = filename.find("_")+1;
			while(filename[pos] != 'x')
			{
				tmp += filename[pos];
				pos++;
			}
			x = boost::lexical_cast<int>(tmp);
			tmp = "";
			pos++;
			while(filename[pos] != '_')
			{
				tmp += filename[pos];
				pos++;
			}
			for (int i = pos+1; i < (int)filename.size(); i++)
				tilesetName += filename[i];
			y = boost::lexical_cast<int>(tmp);
			tileset.size.x = x; tileset.size.y = y;
			tileset.name = tilesetName;
			sf::Image image;
			image.loadFromFile(file.string());
			int id = 0;
			for(int j = 0; j < image.getSize().y/y; j++)
				for(int i = 0; i < image.getSize().x/x; i++)
				{
					tmpTexture = new sf::Texture();
					sf::IntRect rect(i*x, j*y, x, y);
					tmpTexture->loadFromImage(image, rect);
					Textures[filename+"_"+boost::lexical_cast<std::string>(id)].push_back(tmpTexture);
					tileset.tiles.push_back(filename+"_"+boost::lexical_cast<std::string>(id));
					id++;
				}
			Tilesets.push_back(tileset);
		}
		else
		{
			tmpTexture = new sf::Texture();
			tmpTexture->loadFromFile(file.string());
			Textures[filename].push_back(tmpTexture);
		}
	}
}


void TDEngine::GraphicsManager::UpdateOpenGL(sf::VideoMode mode, std::string title, bool isFullscreen, int fps)
{
	Window->create(mode, title, isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	Window->setFramerateLimit(fps);
	glEnable(GL_TEXTURE_2D);
	WinWidth = mode.width;
	WinHeight = mode.height;
}


void TDEngine::GraphicsManager::RenderText(std::string text, sf::Vector2f pos, sf::Font& font, sf::Color& color, unsigned int size)
{
	Window->pushGLStates();
	Window->setView(sf::View(sf::FloatRect(0.f,0.f,getWidth(),getHeight())));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	sf::Text txt(text, font, size);
	txt.setColor(color);
	txt.setPosition(pos);
	Window->draw(txt);

	Window->popGLStates();
	
}


std::string TDEngine::GraphicsManager::GetTextureName(const sf::Texture* texture)
{
	for(auto tex : Textures)
		if (std::find_if(tex.second.begin(), tex.second.end(), [texture](const sf::Texture* tex)->bool {return tex == texture;})
			!= tex.second.end())
			return tex.first;
	return "default";
}


void TDEngine::GraphicsManager::LoadAnimations()
{
	typedef TDEngine::INI <std::string, std::string, std::string> ini_t;
	std::string fileName = GameEngine.GetSetting<std::string>("AnimationSettings", "Data\\Animations.ini");
	ini_t ini(fileName, true);

	for (auto i = ini.sections.begin(); i != ini.sections.end(); i++)
	{
		std::string name = i->first;
		float Duration = 0.f;
		sf::Texture* textures[Animation::MAXFRAMES] = { 0 };
		sf::Texture* defaultTexture = GetTexture("Default");

		unsigned int size = 0;

		for (auto j = i->second->begin(); j != i->second->end(); j++)
			if (j->first.compare("Duration") == 0)
				Duration = boost::lexical_cast<float>(j->second);
			else if (j->first.find("Frame") != std::string::npos && (j->first.find("DefaultFrame") == std::string::npos))
			{
				std::size_t pos = j->first.find("Frame") + size_t(5);
				if (pos == std::string::npos)
				{
					GameEngine.LogEngine.LogError("Error occured during parsing file " + fileName + ". File structure is incorrect.", "GraphicsManager");
					continue;
				}
				textures[boost::lexical_cast<int>(j->first.substr(pos))]
					= GetTexture(j->second);
				size++;
			}
			else if (j->first.find("DefaultFrame") != std::string::npos)
				defaultTexture = GetTexture(j->second);
			else
				continue;

		Animations[name] = Animation(textures, size, Duration, *defaultTexture);
	}
}


TDEngine::Animation TDEngine::GraphicsManager::GetAnimation(std::string name) const
{
	try
	{
		return Animations.at(name);
	}
	catch (std::out_of_range&)
	{
		GameEngine.LogEngine.LogWarning("Animation " + name + "not found", "GraphicsManager");
		return Animation();
	}
}