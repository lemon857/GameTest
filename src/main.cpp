#include "EngineCore/Editor/EditorApplication.h"
#include "Games/GameApplication.h"    

#include "EngineCore/Resources/ResourceManager.h"

int main(int argc, char** argv)
{
    glm::ivec2 windowSize(800, 600);

    ResourceManager::setExecutablePath(argv[0]);

    //GameApp* gameApp = new GameApp();
    //gameApp->start(windowSize, "Engine preview game", "res/resources.json", "EngineGamePreview.ini");
    //delete gameApp;

    EditorApplication* editorApplication = new EditorApplication();
    editorApplication->start(windowSize, "Engine editor", "res/resources.json", "EngineTest.ini");
    delete editorApplication;

    return 0;
}