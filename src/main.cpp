#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Editor/EditorApplication.h"
#include "Games/Tower_Defense/GameApplication.h"    

//#define EDITOR_BUILD

int main(int argc, char** argv)
{
    glm::ivec2 windowSize(800, 600);

    ResourceManager::setExecutablePath(argv[0]);

#ifndef EDITOR_BUILD
    GameApp* gameApp = new GameApp();    
    gameApp->start(windowSize, "Tower defence", "res/resources.json", "EngineGamePreview.ini");
    //delete gameApp;
#endif // !EDITOR_BUILD

#ifdef EDITOR_BUILD
    EditorApplication* editorApplication = new EditorApplication();
    editorApplication->start(windowSize, "Engine editor", "res/resources.json", "EngineTest.ini");
    //delete editorApplication;
#endif // EDITOR_BUILD

    return 0;
}