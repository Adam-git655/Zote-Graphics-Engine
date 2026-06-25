# ZOTE GRAPHICS ENGINE

## Overview

A 3D Graphics engine built using C++ and OpenGL, featuring a Unity styled editor with a dockable interface, dynamic object management, dynamic lighting, post processing, skybox rendering and more.

<img width="1906" height="977" alt="image" src="https://github.com/user-attachments/assets/8fc2b0a3-6de3-44f2-9351-afe4010c6348" />

Resource: https://learnopengl.com/ by JoeyDeVries

## Features
- **Editor**: Dockable panels (Viewport, Scene hierarchy, inspector) built with Dear ImGui, with a fullscreen toggle
- **Viewport**: Scene renders to an offscreen framebuffer and is displayed within an ImGui window
- **Scene/Object Management**: Add primitives (cube, plane), load .obj models at runtime using a file dialog, rename and delete objects, all from the UI
- **Inspector**: Customize properties of gameobjects directly from the UI.
- **Transformation gizmos**: Translate, Rotate and scale objects directly in the viewport using ImGuizmo, with a local/world space toggle and keyboard shortcuts to switch between transformation modes (W, E, R)
- **Dynamic lighting**: Directional, point and spot lights with adjustable ambient, diffuse and specular properties. Point lights can be added and removed on the fly.
- **Materials**: Textured and solid-color objects share a unified lighting pipeline (Blinn-Phong lighting). Supports diffuse and specular mapping in case of textures.
- **Transparency**: Transparent objects render correctly with correct blending of alpha color values. Uses a back to front sorted rendering approach.
- **Skybox**: Cubemap based skybox rendering
- **Post processing**: Switchable screen-space post process effects (inversion, grayscale, sharpen, blur, edge detection) via a dedicated post-process frame buffer.

## Controls
| Input | Action |
|-------|--------|
| `W` `A` `S` `D` | Move camera |
| `E` / `Q` | Move camera up / down |
| `Space` | Toggle mouse capture |
| Mouse movement | Look around (when captured) |
| Scroll | Zoom (when captured) |
| `F` | Toggle fullscreen viewport |
| `Q` / `W` / `E` / `R` | Gizmo disable / translate / rotate / scale (when mouse not captured) |
| `Delete` | Delete selected object |
| Right click in Scene panel | Add/load objects |
| Double-click object name | Rename |
| `Esc` | Quit |

---

## Building

### Requirements
- CMake 3.16+
- A C++17 compiler
- OpenGL 3.3 capable GPU

### Dependencies
All dependencies are included in "thirdparty/" folder

- Glad to load OpenGL functions
- GLFW for window creation and Inputs
- GLM for math
- Dear ImGui for UI
- ImGuizmo for transformation gizmos
- Assimp for model loading
- stb_image for loading textures
- tinyfiledialogs for file dialogs

### Steps to build
Run the following commands in PowerShell/terminal in the folder where CMakeLists.txt is located
```
git clone https://github.com/Adam-git655/Zote-Graphics-Engine.git
mkdir build
cd build
cmake ../
```
Make sure the 'resources/' folder is accessible by the executable (The resource path is configured at compile time)

---
## Architectural notes
- The engine uses a lightweight scene graph approach. The 'Scene' owns a vector of GameObjects, meshes and models. Each GameObject contains a transform, some properties, a non owning pointer to a shader owned by 'Application' and a non owning pointer to a mesh or a model (or none). 
- All visible game objects are initialized in the scene class itself. Rendering is split into opaque and transparent passes with the skybox drawn in between them.
- The viewport is rendered to a framebuffer and post-processed through a second framebuffer before being displayed in the viewport.
- A 'Model' reads the model data and converts it into several 'Mesh' objects using assimp.
- The lighting properties of directional, point and spot lights are managed by a lighting class which syncs the properties between the shader and the inspector.
- 'Shader' and 'Texture Loader' exist to abstract shader code and texture loading.
- Setting up of VAO, VBO, vertices, tex coords, normals of primitives like cubes and planes are abstracted away within a primitives namespace. 




