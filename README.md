# Nocturne Renderer
### Background
I want to build a Renderer from scratch using Vulkan. I just finished a course on Udemy that gave me a quick rundown of what Vulkan has to offer. But there is still a lot to learn for me (which is exciting!). Listed below are a couple of tasks that I would like to do with the Vulkan Renderer I plan on writing. 
### Notes
For all of these tasks, implementing is not enough for me. I want to be able to say with confidence that I understand the concepts as well as the math behind each of these concepts.

## Tasks
#### Rewrite Current Vulkan Renderer
I made this Renderer while working alongside the lecturer of the course I was taking. This means that although I have a good grasp on how Vulkan works overall. The nitty gritty of the API itself is still somewhat a blur for me. I think rewriting the Renderer without any hand holding will solidify my understanding of how to use the Vulkan API. 
#### Camera Controls and Transformations
Implement basic camera controls, like orbiting around a point, panning, zooming, and rotation. Also, develop a system for object transformations like translation, rotation, and scaling.
#### Basic Shading and Lighting
Implement basic shading models like Phong or Lambert. I'll also need to incorporate some basic lighting models, starting with a simple directional light.
#### Model Loading
Use a library like Assimp to load complex 3D models from a variety of file formats. At this stage, focus on rendering these models with a simple, uniform color.
#### Textures
Add texture support for your models. This includes both single textures (diffuse) and multiple textures (specular, normal, etc.).
#### Deferred Shading
Implement a more efficient lighting model. Will need to modify my shaders and rendering pipeline to accommodate a two-pass approach.
#### Advanced Lighting
Improve the lighting model to support point lights, spotlights, and multiple lights. Try to implement more advanced lighting techniques like Normal Mapping, Parallax Mapping, HDR, Bloom, and so on.
#### Shadow Mapping
Implement basic shadow mapping, which involves rendering the scene from the perspective of a light source and then using that information to create shadows.
#### Post-processing Effects
Learn how to implement post-processing effects, like bloom, depth of field, or screen-space ambient occlusion (SSAO). This will drastically improve the look of the renders.
#### Animation
Add support for basic animations. This will probably involve skeletal animation, and can also involve things like vertex shaders for more dynamic, procedural animations.
#### PBR Materials
Implement physically-based rendering (PBR) materials. This is a more modern approach to rendering, which provides a more realistic and consistent look under various lighting conditions.
#### Scene Graph
Implement a scene graph to handle hierarchical or parent-child relationships between objects. This is essential for complex scenes and is also useful for culling and level-of-detail techniques.
#### Optimization
Finally, work on optimizing the renderer. This can involve techniques like frustum culling, occlusion culling, and level-of-detail rendering. Look into multi-threaded rendering and GPU instancing.

## Screenshots

Texture Demonstration!

https://github.com/CoolGuyRy/Nocturne-Renderer/assets/86322787/e9c3b6bc-5649-4685-ac12-6a4b3d36921d

Depth Buffer + Dynamic Uniform Buffer Demo!

https://github.com/CoolGuyRy/Nocturne-Renderer/assets/86322787/97305d10-8018-4975-8df7-e8cbf206727b

First ever Render run with the Nocturne Renderer!
![First Run Screenshot](https://github.com/CoolGuyRy/Nocturne-Renderer/assets/86322787/46bb09b0-04c9-45b2-956d-863b618a4a40)

Console output from first run with the Nocturne Renderer!
![First Run Console Output Modified](https://github.com/CoolGuyRy/Nocturne-Renderer/assets/86322787/1dd96993-80d2-43a5-a46d-e5203d5f96df)



