#ifndef CLASS_NCINE_LUAAPPLICATION
#define CLASS_NCINE_LUAAPPLICATION

struct lua_State;

namespace ncine {

/// Lua bindings around the `Application` class
class LuaApplication
{
  public:
	static void expose(lua_State *L);

  private:
	static int renderingSettings(lua_State *L);
	static int setRenderingSettings(lua_State *L);

	static int rootNode(lua_State *L);
	static int interval(lua_State *L);

	static int width(lua_State *L);
	static int height(lua_State *L);
	static int screenDimensions(lua_State *L);

	static int isPaused(lua_State *L);
	static int setPaused(lua_State *L);

	static int quit(lua_State *L);

	static int datapath(lua_State *L);
	static int savepath(lua_State *L);
};

}

#endif