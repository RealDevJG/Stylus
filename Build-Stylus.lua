-- premake5.lua
workspace "Stylus"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Stylus"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Build-Stylus-External.lua"
include "Stylus/Build-Stylus-App.lua"
