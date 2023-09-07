#include "EngineCore/Editor/EditorApplication.h"

#include "EngineCore/Resources/ResourceManager.h"

int main(int argc, char** argv)
{
    glm::ivec2 windowSize(800, 600);

    ResourceManager::setExecutablePath(argv[0]);

    EditorApplication* editorApplication = new EditorApplication();

    editorApplication->start(windowSize, "Engine editor", "res/resources.json", "EngineTest.ini");

    delete editorApplication;

    return 0;
}