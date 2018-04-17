#include "MapRedactor.h"
#include "Game.h"
#include "Map.h"
#include <gl/GL.h>
#include <CEGUI/RendererModules/OpenGL/Texture.h>

extern TDEngine::GameManager TDEngine::GameEngine;


	unsigned int sf::getTexId(sf::Texture* tex)
	{
		return  tex->m_texture;
	}


namespace TDGame
{

	MapRedactor::MapRedactor() : brush("grass_top")
	{
		LastBrush = 0;
		TilePalette = CEGUI::WindowManager::getSingleton().createWindow( 
			TDEngine::GameEngine.GetSetting<std::string>("DefaultWidgetLook")+"/FrameWindow", "MapRedactor_TileSelecter" );
		TilePalette->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.0f, TDEngine::GameEngine.GraphicsEngine.Window->getSize().x-106 ), CEGUI::UDim( 0.0f, 0.f ) ) );
		TilePalette->setSize( CEGUI::USize( CEGUI::UDim( 0.0f, 140.f ), CEGUI::UDim( 1.0f, 0.f ) ) );

		TileProperties =  CEGUI::WindowManager::getSingleton().loadLayoutFromFile("TileProperties.layout");
	//	TileProperties->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, TDEngine::GameEngine.GraphicsEngine.Window->getSize().x - 106), CEGUI::UDim(0.0f, 0.f)));
		//TileProperties->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 140.f), CEGUI::UDim(1.0f, 0.f)));

		IsActive = true;
		UpdateMenu();
		pos = 0.f;
	}


	void MapRedactor::Render()
	{
		
		TDEngine::GameEngine.GraphicsEngine.Window->pushGLStates();
		//TilePalette->activate();
		if(pos > 0)
			pos = 0;
		/*glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.f, TDEngine::GameEngine.GraphicsEngine.Window->getSize().x,
			TDEngine::GameEngine.GraphicsEngine.Window->getSize().y, 0.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();*/
		TDEngine::GameEngine.GraphicsEngine.Window->setView(sf::View(sf::FloatRect(0.f,0.f,TDEngine::GameEngine.GraphicsEngine.getWidth(),TDEngine::GameEngine.GraphicsEngine.getHeight())));
		sf::RenderStates states;
		states.transform = sf::Transform().translate(0.f, pos);
		for(int i = 0; i < tiles.size(); i++)
		{
				TDEngine::GameEngine.GraphicsEngine.Window->draw(*tiles[i], states);
		}

		TDEngine::GameEngine.GraphicsEngine.sfgui->Display(*TDEngine::GameEngine.GraphicsEngine.Window);
		TDEngine::GameEngine.GraphicsEngine.Window->popGLStates();
	}



	void MapRedactor::UpdateMenu(float x, float y)
	{
		unsigned int WindowXSize         = TDEngine::GameEngine.GraphicsEngine.Window->getSize().x;

		CEGUI::Window* button            = nullptr;
		CEGUI::OpenGLTexture* texturePtr = nullptr; //CEGUI pointer to texture
		CEGUI::Image* ButtonImage        = nullptr;
		sf::Texture* tex                 = nullptr; //SFML pointer to texture
		int i = 0, j = 1; //loop counters


		TilePalette->setText( "Tiles list" );
		

		for(auto tileset : TDEngine::GameEngine.GraphicsEngine.getTilesets())
			for(auto tile : tileset.tiles)
			{
				tex = TDEngine::GameEngine.GraphicsEngine.GetTexture(tile);
				texturePtr = (CEGUI::OpenGLTexture*)(&CEGUI::System::getSingleton().getRenderer()->createTexture(tile+"_tex"));
				texturePtr->setOpenGLTexture(sf::getTexId(tex), CEGUI::Sizef(tex->getSize().x, tex->getSize().y));
				CEGUI::Image* ButtonImage = &CEGUI::ImageManager::getSingleton().create("BasicImage", tile);
				((CEGUI::BasicImage*)ButtonImage)->setArea(CEGUI::Rectf(0, 0, tex->getSize().x, tex->getSize().y));
				((CEGUI::BasicImage*)ButtonImage)->setTexture(texturePtr);

				

				button = CEGUI::WindowManager::getSingleton().createWindow( 
					TDEngine::GameEngine.GetSetting<std::string>("DefaultWidgetLook")+"/ImageButton", tile );
				button->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.0f, (33*(j-1))+1), CEGUI::UDim( 0.0f, (33*i)+1)));
				button->setSize( CEGUI::USize( CEGUI::UDim( 0.0f, 32 ), CEGUI::UDim( 0.0f, 32 ) ) );
				button->setText( tile);
				button->setProperty("NormalImage", tile);
				button->setProperty("HoverImage", tile);
				button->setProperty("PushedImage", tile);
				button->subscribeEvent(CEGUI::PushButton::EventClicked, 
					CEGUI::Event::Subscriber([&](const CEGUI::EventArgs& args) -> bool
				{
					if(LastBrush)
					{
						LastBrush->setProperty("NormalImageColour", "FFFFFFFF");
						LastBrush->setProperty("HoverImageColour", "FFFFFFFF");
						LastBrush->setProperty("PushedImageColour", "FFFFFFFF");
					}
					brush = ((CEGUI::WindowEventArgs&)args).window->getText().c_str();
					((CEGUI::WindowEventArgs&)args).window->setProperty("NormalImageColour", "601AFF00");
					((CEGUI::WindowEventArgs&)args).window->setProperty("HoverImageColour", "601AFF00");
					((CEGUI::WindowEventArgs&)args).window->setProperty("PushedImageColour", "601AFF00");
					LastBrush = ((CEGUI::WindowEventArgs&)args).window;
					TilePalette->activate();
					return true;
				}));		
				TilePalette->addChild(button);
				

				i++;
				if((i*33)+1 > ((TilePalette->getSize().d_height.d_scale*TDEngine::GameEngine.GraphicsEngine.Window->getSize().y)-64))
				{
					i = 0;
					j++;
				}
			}
		TilePalette->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber([&](const CEGUI::EventArgs& args) -> bool
		{
			TilePalette->activate();
			return true;
		}));
			TileProperties =  CEGUI::WindowManager::getSingleton().loadLayoutFromFile("TileProperties.layout");
			TileProperties->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.0f, WindowXSize-((33*j)+16) ), CEGUI::UDim( 0.0f, 0.f ) ) );
			TileProperties->setSize( CEGUI::USize( CEGUI::UDim( 0.0f, (33*j)+16 ), CEGUI::UDim( 1.0f, 0.f ) ) );

			button = TileProperties->getChild(0)->getChild(2);
			
		//	((CEGUI::Combobox*)button)->addItem(new CEGUI::ListboxTextItem("true"));
		//	((CEGUI::Combobox*)button)->addItem(new CEGUI::ListboxTextItem("false", 1));
	//	button = CEGUI::WindowManager::getSingleton().createWindow( 
	//			TDEngine::GameEngine.GetSetting<std::string>("DefaultWidgetLook")+"/Combobox", "TerrainTilePassabilitySelecter");

		((CEGUI::Combobox*)button)->addItem(new CEGUI::ListboxTextItem("true"));
		((CEGUI::Combobox*)button)->addItem(new CEGUI::ListboxTextItem("false", 1));
		((CEGUI::Combobox*)button)->getEditbox()->setReadOnly(true);
	//	((CEGUI::Combobox*)button)->setTextParsingEnabled(false);
			CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(TileProperties);

CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(TilePalette);
auto wnd = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
		//TileProperties->addChild(button);
	/*	sizeX = x;
		sizeY = y;
		tiles.clear();
		int i = 0;
		for(auto tileset : TDEngine::GameEngine.GraphicsEngine.getTilesets())
			for(auto tile : tileset.tiles)
			{
				sf::RectangleShape* Tile = new sf::RectangleShape();
				Tile->setSize(sf::Vector2f(x, y));
				Tile->setTexture(TDEngine::GameEngine.GraphicsEngine.GetTexture(tile));
				Tile->setPosition(TDEngine::GameEngine.GraphicsEngine.Window->getSize().x-x, i*y);
				tiles.push_back(Tile);
				i++;
			}*/


		// SFGUI
		
		propertiesWindow = sfg::Window::Create(sfg::Window::Style::TITLEBAR | sfg::Window::Style::BACKGROUND | sfg::Window::Style::RESIZE | sfg::Window::Style::CLOSE).get();




		TDEngine::GameEngine.GraphicsEngine.desktop->Add(std::make_shared<sfg::Window>(*propertiesWindow));

	}


	void MapRedactor::ProceedEvent(sf::Event& event)
	{
		static const float speed = 128.0f;
		switch(event.type)
		{
		case sf::Event::MouseLeft:
			if(!TilePalette->isMouseContainedInArea())
			{
			Tile* obj = Map::MapInstance->getObjectByMouseCoordinates(event.mouseButton.x, event.mouseButton.y);
			if(obj)
			{
				TDEngine::Event ev = TDEngine::GameEngine.EventEngine.GetEventPattern("event_changeTile");
				ev.Parameters[0].second = (void*)TDEngine::GameEngine.GraphicsEngine.GetTexture(brush);
				obj->ProceedEvent(ev);
				if(dynamic_cast<TerrainTile*>(obj))
					((TerrainTile*)obj)->SetPassability(false);
			}
			}
			break;
		}
	}
}


