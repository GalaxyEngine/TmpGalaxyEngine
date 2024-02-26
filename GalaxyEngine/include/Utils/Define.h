#pragma once

// -- Shaders -- //
#define PICKING_PATH ENGINE_RESOURCE_FOLDER_NAME"/shaders/PickingShader/picking.frag"
#define OUTLINE_PATH ENGINE_RESOURCE_FOLDER_NAME"/shaders/PostProcess/Outline/outline.ppshader"
#define BILLBOARD_PATH ENGINE_RESOURCE_FOLDER_NAME "/shaders/BillboardShader/billboard.shader"
#define GRID_PATH ENGINE_RESOURCE_FOLDER_NAME"/shaders/GridShader/grid.shader"

// -- Icons -- //
#define FOLDER_ICON_PATH ENGINE_RESOURCE_FOLDER_NAME"/icons/folder.png"
#define FILE_ICON_PATH ENGINE_RESOURCE_FOLDER_NAME"/icons/file.png"
#define LIGHT_ICON_PATH ENGINE_RESOURCE_FOLDER_NAME"/icons/light.png"
#define CAMERA_ICON_PATH ENGINE_RESOURCE_FOLDER_NAME"/icons/camera.png"

#define THUMBNAIL_PATH "Cache/thumbnail/"

// -- Meshes -- //
#define PLANE_PATH ENGINE_RESOURCE_FOLDER_NAME"/models/Plane.obj:Plane"
#define SPHERE_PATH ENGINE_RESOURCE_FOLDER_NAME"/models/Sphere.obj:Sphere"

// -- Paths -- //
#define ASSET_FOLDER_NAME "Assets"
#define ENGINE_RESOURCE_FOLDER_NAME "CoreResources"

// -- Misc -- //
#define NONE_RESOURCE "None"
#define MAX_LIGHT_NUMBER 8

#define INDEX_NONE -1

// -- Colors -- //
#define BUTTON_RED Vec4f(0.8f, 0.15f, 0.1f, 1.f)