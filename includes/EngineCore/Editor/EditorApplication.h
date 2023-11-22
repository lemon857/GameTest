#pragma once

#include "EngineCore/Engine/Application.h"

#include "EngineCore/Renderer/Camera.h"
#include "EngineCore/Physics/Ray.h"
#include "EngineCore/Renderer/Line.h"

#include "EngineCore/Editor/UI/UImodule.h"
#include "EngineCore/Editor/UI/UIlayouts.h"

#include "EngineCore/Resources/Scene.h"

class EditorApplication : public Application
{
public:
    EditorApplication();
    ~EditorApplication();

    void on_ui_render() override;
    bool init() override;
    void on_key_update(const double delta) override;
    void on_update(const double delta) override;
    bool init_events() override;
private:
    Scene m_scene;

    Camera* m_cam;

    Ray* m_ray;

    RenderEngine::Line* m_line;

    RenderEngine::Line* m_line_transform;

    float m_colors[4] = { 0.33f, 0.33f, 0.33f, 0.f };
    float m_cam_pos[3] = { -5.f, 0.f, 0.f };
    float m_cam_rot[3] = { 0.f, 0.f, 0.f };

    float m_light_color[3] = { 1.f, 1.f, 1.f };

    int item_current = -1;

    float m_cam_velocity = 0.01f;
    float m_cam_rotate_velocity = 0.1f;
    float m_cam_sensetivity = 0.5f;
    float m_cam_fov = 60.f;
    float m_cam_sov = 1.f;
    float m_cam_far_plane = 100.f;

    float m_add_ctrl_speed = 2.f;

    float m_ambient_factor = 0.1f;
    float m_diffuse_factor = 1.f;
    float m_specular_factor = 1.f;
    float m_diffuse_specular_factor = 0.5f;
    float m_metalic_factor = 0.f;
    float m_shininess = 1.f;

    float m_distance = 10.f;

    double m_init_mouse_pos_x = 0;
    double m_init_mouse_pos_y = 0;

    int m_mouse_pos_x = 0;
    int m_mouse_pos_y = 0;
    int m_fps = 0;

    double m_world_mouse_dir_x = 0;
    double m_world_mouse_dir_y = 0;
    double m_world_mouse_dir_z = 0;
    
    double m_world_mouse_pos_x = 0;
    double m_world_mouse_pos_y = 0;
    double m_world_mouse_pos_z = 0;

    bool m_isPerspectiveCam = true;
    bool m_isInversiveMouseY = false;
    bool m_drawNullIntersection = false;
    bool m_moveObject = false;
    bool m_isUIhovered = false;

    int component_current = 0;
    int objstype_current = 0;
    int material_current = 0;
    int obj_current = 0;
    int file_current = 0;

    bool isReloadObjFile = false;
    bool isSelectedObjFile = false;

    std::string selectedObj = "";
    
    char bufName[32];

    float test = 0;

    double curTime = 0;

    int curFpsEs = 0;
    int fps = 0;
    int countFpsEs = 3;

    std::vector<int> fpses{ 0, 0, 0 };

    UIlayoutTransform transformLayout;
    UIlayoutHighlight highlightLayout;
    std::shared_ptr<UIlayoutMaterial> materialLayout;
    UIlayoutMeshRenderer meshrendererLayout;
};