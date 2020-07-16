/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */
#ifndef _ENGINE_H_
#define _ENGINE_H_
#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <IL/il.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <lua535/lua.hpp>
#include <BASS/bass.h>
#include <vector>

#include "Defines.h"



namespace CE {
	
	// Constants...

	constexpr u8 MAX_SPRITESHEETS = 6;
	constexpr int MAX_FONTS = 1;



	// Structs...

	typedef struct Settings {
		u16 screenWidth;
		u16 screenHeight;
		float scale;
		bool fullScreen;
		bool windowResizable;
		float musicVolume;
	} Settings;


	typedef struct TextureRect {
		GLfloat x, y;
		GLfloat w, h;
	} TextureRect;


	typedef struct FontItem {
		short ascii;
		short ucode;
		short top;
		short bottom;
		short x;
		short y;
		short width;
		short height;
		short leading;
		short trailing;
	} FontItem;


	typedef struct TileSetSource {
		char* name;
		u16 tileWidth;
		u16 tileHeight;
		u16 tileCount;
		u16 columns;
		u16 width;
		u16 height;
		char* imageSource;
	} TileSetSource;


	typedef struct TiledObject {
		u16 id;
		char* name;
		char* type;
		char* templateFile;
		s16 x;
		s16 y;
		u16 firstGid;
		char* source;
		u16 gid;
		u16 width;
		u16 height;
	} TiledObject;


	typedef struct ObjectGroup {
		u16 id;
		char* name;
		u16 objectsCount;
		std::vector<TiledObject*> objects;
	} ObjectGroup;


	typedef struct TiledTemplate {
		u16 tileSetFirstGid;
		std::string source;
		u16 objectGid;
		u16 width;
		u16 height;
	} TiledTemplate;


	typedef struct TileSet {
		u16 firstGid;
		TileSetSource* source;
	} TileSet;


	typedef struct Layer {
		u16 id;
		char* name;
		u16 width;
		u16 height;
		CE_ArrayInt data;
		u16 dataSize;
	} Layer;


	typedef struct Map {
		u16 width;
		u16 height;
		u16 tileWidth;
		u16 tileHeight;
		u16 nextLayerId;
		u16 nextObjectId;
		std::vector<TileSet*> tileSets;
		std::vector<Layer*> layers;
		std::vector<ObjectGroup*> objectGroups;
		Map() : width(0), height(0), tileWidth(0), tileHeight(0), nextLayerId(0), nextObjectId(0), tileSets(), layers(), objectGroups() {}
	} Map;



	// Classess...

	class Music {
	public:
		// DEFAULT MUSIC VOLUME = 100%
		constexpr static float DEFAULT_MUSIC_VOLUME = 1.0f;
		Music(const std::string& musicFile, float volume, bool loop);
		~Music();
		bool playMusic(float _volume);
		bool playMusic();
		bool stopMusic();
		bool pauseMusic();
		HSTREAM getStream();
	private:
		HSTREAM stream;
		std::string musicFileName;
		float volume;
		bool loop;
	};

	
	class Engine {
	public:
		Engine();
		void launch(void);
		void stop();
		void stop(s16 _exitCode);
		Settings settings;
		SDL_Window* window;
		SDL_GLContext glContext;
		s16 exitCode;
		Music* getCurrentMusic(void);
		void loadMusic(std::string musicFile);
		bool playMusic(float volume);
		bool playMusic(void);
		bool stopMusic(void);
		bool pauseMusic(void);
		void releaseMusic(void);
	private:
		Music* currentMusic;
		SDL_Cursor* systemCursor;
		SDL_Surface* cursorIcon;
		void setSystemCursor(void);
		void init(void);
		void initSDL(void);
		void initOGL(void);
		void initDevIL(void);
		void deviLSettings(void);
		void initBASS(void);
	};


	class ShaderLoader {
	public:
		ShaderLoader(void);
		~ShaderLoader(void);
		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void use(void);
		void unuse(void);
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		GLuint programID;
	private:
		GLuint vertexShaderID;
		GLuint fragmentShaderID;
		GLint colAttrib;
		GLint posAttrib;
		void compileShaderFile(const std::string& filePath, GLuint& shaderId);
	};


	class Texture {
	public:
		Texture(std::string _fileName, GLfloat _x, GLfloat _y, u16 _screenWidth, u16 _screenHeight);
		~Texture();
		ILuint imageId;
		GLuint textureId;
		GLfloat x, y;
		GLfloat width, height;
		GLfloat tileWidth, tileHeight;
		u16 screenWidth, screenHeight;
		ILubyte* data;
		ILuint format;
		ILuint bytesPerPixel;
		ILuint bitsPerPixel;
		ILuint depth;
		int columns;
		void draw(GLfloat _x, GLfloat _y);
		void draw(GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height);
		/*void draw(TextureRect _src, TextureRect _dest);
		void drawTile(int _id, GLfloat _dx, GLfloat _dy);
		void drawTile(int _id, GLfloat _dx, GLfloat _dy, GLfloat _scale);*/
	private:
		GLuint loadTexture(std::string _fileName);
		GLfloat coordToFloatX(GLfloat _x);
		GLfloat coordToFloatY(GLfloat _y);
		GLfloat coordToFloat(GLfloat _i, u16 length);
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
	};


	class GraphicAssets {
	public:
		static cu8 IMAGE_ASSETS_MOUSE_CURSOR = 0;
		static cu8 IMAGE_ASSETS_MAIN_MENU_BACKGROUND = 1;
		static cu8 IMAGE_ASSETS_LOGO = 2;
		static cu8 IMAGE_ASSETS_MAIN_MENU_BUTTONS = 3;
		static cu8 IMAGE_ASSETS_VINGUE_FONT = 4;
		static cu8 IMAGE_ASSETS_BIG_SPRITESHEET = 5;
		static GraphicAssets* getAssets();
		static void addToAssets(std::string fileName, cu8 imagesEnum);
		static void releaseAssets();
		Texture* textures[MAX_SPRITESHEETS];
	private:
		static GraphicAssets* instance;
		GraphicAssets();
	};


	class XMLHelper {
	public:
		static int readPropInt(xmlNodePtr node, const xmlChar* prop);
		static short readPropShort(xmlNodePtr node, const xmlChar* prop);
	private:
		static int xmlCharToInt(const xmlChar* a);
		static short xmlCharToShort(const xmlChar* a);
	};


	class TextFont {
	public:
		TextFont(std::string fn, Texture* tex);
		~TextFont();
		void draw(const char* text, GLfloat x, GLfloat y, float size);
	private:
		Texture* fontImage;
		int imageWidth;
		int imageHeight;
		std::vector<FontItem*> fontItems;
		int charsCount;
		std::string fontName;
		int fontWidth;
		int fontHeight;
		int fontSpace;
		void parseXML(std::string xmlFileName);
		float getWithOfFontString(const char* text);
	};


	class FontAssets {
	public:
		static const int FONT_ASSETS_VINGUE = 0;
		static FontAssets* getAssets();
		static void addToAssets(std::string fileName, Texture* tex, const int fontEnum);
		static void releaseAssets();
		TextFont* fonts[MAX_FONTS];
	private:
		static FontAssets* instance;
		FontAssets();
		~FontAssets();
	};


	class TiledMap {
	public:
		TiledMap(std::string fileName);
		~TiledMap();
		Map* map;
	private:
		TileSetSource* getTileSetSource(std::string tsxFileName);
		CE_ArrayInt parseData(xmlDocPtr doc, xmlNodePtr cur);
		std::vector<TiledObject*> getObjects(xmlNodePtr cur, int objectCount);
	};


	class GameObject {
	public:
		GameObject();
		GameObject(Vector2* _vector, float _width, float _height);
		GameObject(const char* _name, float _x, float _y, float _width, float _height);
		~GameObject();
		virtual void update() = 0;
		virtual void render() = 0;
		int getTileX(u16 tileWidth);
		int getTileY(u16 tileHeight);
		std::string name;
		float width, height;
		Vector2* vector;
	};


	class LuaHandler {
	public:
		LuaHandler(const std::string& fileName);
		~LuaHandler();
		void close();
		bool loadFile(const std::string fileName);
		bool getInt(const char* name, int& value);
		bool getString(const char* name, std::string& value);
		
		CE::GameObject* getObject(const char* _objectName);

		bool getGlobal(const char* name);
	private:
		lua_State* luaState;
		lua_Number getFieldInt(const char* objectName, const char* propertyName);
	};


	class Animation {
	public:
		Animation(u16 speed, cu16 size, std::vector<u16> tilesArray);
		~Animation();
		void nextFrame(void);
		u16 getTile(void);
		u16 getCurFrame(void);
		void setCurrentFrame(u16 curFrame);
	private:
		u16 size;
		std::vector<u16> tilesArray;
		unsigned short speed;
		u16 curFrame;
		u16 counter;
	};

}

#endif
