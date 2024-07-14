C:\VulkanSDK\1.3.280.0\Bin\glslc.exe ..\shaders\simple_shader.vert -o ..\shaders\compiled_shaders\simple_shader.vert.spv
C:\VulkanSDK\1.3.280.0\Bin\glslc.exe ..\shaders\simple_shader.frag -o ..\shaders\compiled_shaders\simple_shader.frag.spv
C:\VulkanSDK\1.3.280.0\Bin\glslc.exe ..\shaders\point_light.vert -o ..\shaders\compiled_shaders\point_light.vert.spv
C:\VulkanSDK\1.3.280.0\Bin\glslc.exe ..\shaders\point_light.frag -o ..\shaders\compiled_shaders\point_light.frag.spv
mingw32-make buildwindows
vulkan.exe