/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2017  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "test/unittests.h"

#include "client.h"
#include "graphicsmanager.h"

#include "being/actorsprite.h"

#include "fs/virtfs/fs.h"

#include "gui/gui.h"

#include "utils/delete2.h"
#include "utils/env.h"
#include "utils/xmlutils.h"

#include "render/sdlgraphics.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/sdlimagehelper.h"

#include "debug.h"

TEST_CASE("xmlutils leak test1", "")
{
    logger = new Logger();
    REQUIRE(gui == nullptr);
    ResourceManager::cleanOrphans(true);
    ResourceManager::deleteInstance();
    delete2(logger);
}

TEST_CASE("xmlutils readXmlIntVector 1", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    logger = new Logger();
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper();
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    ActorSprite::load();

    std::vector<int> arr;

    readXmlIntVector("graphics/gui/browserbox.xml",
        "skinset",
        "widget",
        "option",
        "value",
        arr,
        SkipError_false);

    REQUIRE(5 == arr.size());
    REQUIRE(1 == arr[0]);
    REQUIRE(15 == arr[1]);
    REQUIRE(0 == arr[2]);
    REQUIRE(1 == arr[3]);
    REQUIRE(1 == arr[4]);
    delete2(client);
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}

TEST_CASE("xmlutils readXmlStringMap 1", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    logger = new Logger();
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper();
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    ActorSprite::load();

    std::map<std::string, std::string> arr;

    readXmlStringMap("graphics/sprites/manaplus_emotes.xml",
        "emotes",
        "emote",
        "sprite",
        "name",
        "variant",
        arr,
        SkipError_false);

    REQUIRE(arr.size() == 27);
    REQUIRE(arr["Kitty"] == "0");
    REQUIRE(arr["xD"] == "1");
    REQUIRE(arr["Metal"] == "26");
    delete2(client);
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}

TEST_CASE("xmlutils readXmlIntMap 1", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    logger = new Logger();
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);
    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper();
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    ActorSprite::load();

    std::map<int32_t, int32_t> arr;

    readXmlIntMap("testintmap.xml",
        "tests",
        "sub",
        "item",
        "id",
        "val",
        arr,
        SkipError_false);

    REQUIRE(arr.size() == 3);
    REQUIRE(arr[1] == 2);
    REQUIRE(arr[10] == 20);
    REQUIRE(arr[3] == 0);
    delete2(client);
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}

TEST_CASE("xmlutils leak test2", "")
{
    logger = new Logger();
    REQUIRE(gui == nullptr);
    ResourceManager::cleanOrphans(true);
    ResourceManager::deleteInstance();
    delete2(logger);
}
