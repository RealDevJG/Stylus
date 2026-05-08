project "Stylus"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++23"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files
   {
      "assets/**.comp",
      "assets/**.glsl",

      "src/**.h",
      "src/**.cpp"
   }

   includedirs
   {
      "../vendor/imgui",
      "../vendor/glfw/include",

      "../vendor/walnut/Walnut/Platform/GUI",
      "../vendor/walnut/Walnut/Source",

      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}"
   }

   links
   {
      "Walnut"
   }

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }
      buildoptions { "/utf-8" }

   filter "configurations:Debug"
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"