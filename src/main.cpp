#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "Games/Tower_Defense/GameApplication.h"    

#include "EngineCore/System/Log.h"

//#define EDITOR_BUILD

#ifdef EDITOR_BUILD
//#include "EngineCore/Editor/EditorApplication.h"
#endif // EDITOR_BUILD


int main(int argc, char** argv)
{
    glm::ivec2 windowSize(800, 600);

    ResourceManager::setExecutablePath(argv[0]);

#ifndef EDITOR_BUILD
    GameApp* gameApp = new GameApp();
    try
    {
        gameApp->start(windowSize, "Tower defence", "res/resources.json", "Engine.ini");
    }
    catch (const std::exception& ex)
    {
        LOG_ERROR("Exception: {0}", ex.what());
    }
    delete gameApp;
#endif // !EDITOR_BUILD                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

#ifdef EDITOR_BUILD
    EditorApplication* editorApplication = new EditorApplication();
    editorApplication->start(windowSize, "Engine editor", "res/resources.json", "EngineTest.ini");
    delete editorApplication;
#endif // EDITOR_BUILD

#ifdef DEBUG_CONSOLE
    system("pause");
#endif
    return 0;
}