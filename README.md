# Stylus

## Summary
Stylus is paint-like app created for Windows 10 and 11 with the potential to become cross-platform in the future. It focuses on using the best practices by both [Klaus Iglberger](https://github.com/igl42) and [Jason Turner](https://github.com/lefticus), both known figures in the C++ community. This app runs off of the [Walnut](https://github.com/studiocherno/walnut) framework, created by [Studio Cherno](https://github.com/studiocherno)

## Tech Stack & Requirements
Many of these are provided as a git submodule and do not need external installation - only Vulkan SDK and Visual Studio do
- [Visual Studio](https://visualstudio.com)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows)
- [Walnut](https://github.com/studiocherno/walnut)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLFW](https://github.com/glfw/glfw)
- [stb_image](https://github.com/nothings/stb)
- [GLM](https://github.com/g-truc/glm)
- A compiler capable of C++23

## Features
- **Tools**
    - Brush
    - Eraser
    - Colour Picker
- **History Manager:** undo or redo the changes you make.
- **Open/Save:** Open an image file to display onto the canvas or save your file to many formats.
- **New Canvas:** Create a new canvas whenever you want.
- **Tool Shapes:** 11 tool shapes exist to pick from.
- **Anti-Aliasing:** Smooth the edges of your drawings

## Keyboard shortcuts
- `X` - Swap the primary and secondary colours around
- `B` - Swap to the brush tool
- `E` - Swap to the eraser tool
- `K` - Swap to the colour picker tool
- `[`- Decrease the brush size
- `]`- Increase the brush size
- `Ctrl + Z` - undo a previous action
- `Ctrl + Y` - redo a previous action
- `Ctrl + 0` - Reset the canvas zoom level to 100% and centre the canvas
- `Ctrl + Scroll Wheel` - zoom in and out of the canvas viewport
- `Space + Drag Click` - pan the canvas viewport

## How to run the project
After cloning the repo, run `scripts/Setup-Stylus.bat` to generate Visual Studio 2022 solution files. Open the solution file and run the project. If you'd like to use a more recent version of visual studio, change it in the `scripts/Setup-Stylus.bat` file, or just close Visual Studio's Setup Assistant tab and ignore the warning.

## Visuals
![](./Github/Stylus%20Showcase.gif)
> *The GIFs shown are rendered at 24 frames per second and at lower quality - they are not representative of the in-app experience.*

## Additional
- Walnut uses the [Roboto](https://fonts.google.com/specimen/Roboto) font ([Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0))
