cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(PROJECT_NAME EngineTest)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
src/main.cpp

src/Examples/TanksDemo/TanksDemoGame.cpp
src/Examples/TanksDemo/TanksDemoGame.h
src/Examples/TanksDemo/Tank.cpp
src/Examples/TanksDemo/Tank.h
src/Examples/TanksDemo/BrickWall.h
src/Examples/TanksDemo/BrickWall.cpp
src/Examples/TanksDemo/Bullet.h
src/Examples/TanksDemo/Bullet.cpp

src/Examples/SnakeDemo/SnakeDemoGame.cpp
src/Examples/SnakeDemo/SnakeDemoGame.h
src/Examples/SnakeDemo/SnakePart.h
src/Examples/SnakeDemo/SnakePart.cpp
src/Examples/SnakeDemo/SnakePart.h

src/Examples/PongDemo/PongDemoGame.h
src/Examples/PongDemo/PongDemoGame.cpp
src/Examples/PongDemo/PongObject.h
src/Examples/PongDemo/PongObject.cpp
src/Examples/PongDemo/Ball.h
src/Examples/PongDemo/Ball.cpp

src/Resources/ResourceManager.cpp
src/Resources/ResourceManager.h
src/Resources/stb_image.h

src/Renderer/ShaderProgram.cpp
src/Renderer/ShaderProgram.h
src/Renderer/Texture2D.cpp
src/Renderer/Texture2D.h
src/Renderer/Sprite.cpp
src/Renderer/Sprite.h
src/Renderer/VertexBuffer.h
src/Renderer/VertexBuffer.cpp
src/Renderer/IndexBuffer.h
src/Renderer/IndexBuffer.cpp
src/Renderer/Renderer.h
src/Renderer/Renderer.cpp
src/Renderer/VertexArray.h
src/Renderer/VertexArray.cpp
src/Renderer/VertexBufferLayout.h
src/Renderer/VertexBufferLayout.cpp
src/Renderer/Point.h
src/Renderer/Point.cpp
src/Renderer/Line.h
src/Renderer/Line.cpp
src/Renderer/ShowOutline.h
src/Renderer/ShowOutline.cpp
src/Renderer/Camera.h
src/Renderer/Camera.cpp

src/Renderer/Animator.h
src/Renderer/Animator.cpp
src/Renderer/Animation.h
src/Renderer/Animation.cpp

src/Physics/PhysicsEngine.h
src/Physics/PhysicsEngine.cpp
src/Physics/Collider.h
src/Physics/Collider.cpp
src/Physics/MoveController.h
src/Physics/MoveController.cpp

src/System/Timer.h
src/System/Timer.cpp
src/System/TemplateTimer.h
src/System/Log.h

src/Engine/Engine.h
src/Engine/Engine.cpp
src/Engine/IGameObjec.h
src/Engine/IComponent.h
src/Engine/IGame.h
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/spdlog)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} spdlog)

include_directories(external/glm)
include_directories(external/rapidjson/include)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
#Для игры копирование ресурсов из источника в билд
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)