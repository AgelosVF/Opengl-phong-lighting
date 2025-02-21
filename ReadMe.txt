by Άγγελος Βενέτης Φανουράκης
The project was developed on archlinux and should be able to compile and run on any unix system using the Makefile.
For it to compile and run it the following extra libraries need to be installed:
	-GLFW -> https://www.glfw.org/download.html
	-GLM -> https://glm.g-truc.net/0.9.8/index.html
	-Assimp -> https://github.com/assimp/assimp/blob/master/Build.md
It also uses the stb_image.h and GLAD which are already included.
How to run:
-----------------------------------------------------------------------------------------------------------
To run the program you need to go in the MakeFile directory and use make to compile it. Then in the parent directory the
executable light_sphere_face will be created.
If you are using windows and can't run a makefile after instaling the above libraries you can open a terminal window
navigate to the Makefile directory and use the following commands:
	g++ -std=c++11 -I../HeaderFiles -c ../SourceFiles/main.cpp -o Main.o
	g++ -std=c++11 -I../HeaderFiles -c ../SourceFiles/Sphere.cpp -o Sphere.o
	g++ -std=c++11 -I../HeaderFiles -c ../Libraries/glad.c -o glad.o

	g++ Main.o Sphere.o glad.o -o ../light_sphere_face.exe -L../Libraries -lglfw -lassimp 
Then move to the parent directory and run the light_sphere_face.exe .


Key inputs:
-----------------------------------------------------------------------------------------------------------
You can rotate the camera around you using the mouse/touchpad.
You can zoom in and zoom out using the scroll of the mouse/touchpad.
You can move left using the "a" key.
You can move right using the "d" key.
You can move up using the "w" key.
You can move down using the "s" key.
You can speed up the rotation of the sphere using the "h" key.
You can slow down the rotation of the sphere using the "j" key.
You can increase the size of the face using the "n" key.
You can decrease the size of the face using the "m" key.
You can close the window by pressing escape.


Files and Directories:
------------------------------------------------------------------------------------------------------------
├── HeaderFiles
│   ├── Camera.h <-- Header for camera class. Used for the camera movement and position
│   ├── Mesh.h 	<-- Header for the mesh class. Used for loading assimp meshes in Model.h 
│   ├── Model.h	<-- Header for the model class. Used for loading and using .obj files
│   │		Also for updating uniforms in the shaders and activating the shader for use.
│   ├── Sphere.h <-- Header for the sphere class.
│   └── stb_image.h
├── head_obj	<-- Files used for drawing the face including vertices and textures.
│   ├── Black.png
│   ├── eye.tga
│   ├── head.tga
│   ├── woman1.mtl
│   └── woman1.obj
├── Libraries
│   └── glad.c
├── MakeFile
│   └──  Makefile <-- Makefile used for compiling. Recommended over the cmake
├── Shaders	<-- Fragment and vertex shaders used for the face and sphere.
│   ├── head.fs	
│   ├── head.vs
│   ├── light_sphere.fs
│   └── light_sphere.vs
└── SourceFiles
    ├── main.cpp <-- The main function.
    └── Sphere.cpp <--Source code for the sphere.

Explaining the Code: (there are also many comments on the code about more specific details)
------------------------------------------------------------------
	Shaders:
		 The light_sphere.vs is the vertex shader for the sphere. In it we calculate the position of the
		sphere by transforming its coordinates to ndc using matrix multiplications. That is with the
		"gl_Position = projection * view * model * vec4(aPos, 1.0);" . The model view and projection
		coordinate are uniforms passed to the shader by the main.
		 The light_shader.fs is the fragment shader for the sphere. Since we handle the sphere as a 
		light source its color is white and doesn't change. So we dont do anything other than changing
		the color here.
		 The head.vs is the vertex shader for the head. Its position is calculated the same way as
		the spheres using uniforms. But this time we also need to pass to the fragment shader the 
		position ,the normal vectors and the texture coordinates. They are passed as an Interface block
		which is like a C++ struct so we dont have too many ins and outs.
		 The head.fs is the fragment shader for the head. ambient represents the ambient light of the
		space the head is.diffuse represents the diffuse light is the light reflected on head by the 
		sphere and is calculated by finding the angle between the light direction and the surface 
		normal. And finaly specular represents the specular light which simulates the white spots on
		the object. It is calculated using the reflection of the light on the head and the view 
		direction. Combining these three vectors we get the phong light effect.
	
	Source files:
		Sphere.cpp:: 
		  setupSphere():
			 We start by generating a VAO,VBO and EBO. The VBO is the vertex buffer object were we
			store all the vertices that will be stored in the gpu. The EBO is the element buffer 
			object its like a VBO but we store indices in it that OpenGL will use to draw the 
			vertices. The VAO is a vertex array object, its like an array that will store in it
			any vertex attribute calls made after its activated in it.
			 We start by binding the VAO so it saves the buffers and after the VBO and EBO and store
			in each the corresponding data. After that we set the attribute pointers of the VBO so 
			to the numbers used in the shader so that opengl knows how to interpret the data. Since
			the data is in a struct it is stored sequencely. So we use sizeof(Vertex) to navigate
			between vertices (the vector) and offsetof(Vertex, type) inside the Vertex struct.
			At the end we unbind the VAO to avoid storing more data to it by accident.
		  Draw():
		  	 To draw the sphere we just need to bind the VAO and use glDrawELements() to draw it.
		
		main.cpp::
		  Functions:
			processInput():
				 It's the function that handles the keybinds. For w,a,s and d keybind it uses
				the camera.ProcessKeyboard() to handle them. When j or h are pressed it changes
				the value of the global float lightspeed that is used for the movement of the
				sphere. When escape is pressed it uses the glfw function 
				glfwSetWindowShouldClose(window, true); to close the window. For reducing the
				size of the head with n and m it changes the global float headsize.
			framebuffer_size_callback():
				 Used to update the window size when the operating system or the user changes
				it.
			mouse_callback()
				 Used to handle the movement of the mouse. It uses two static and two global
				variables to find the changes in the offsets in the mouse position between calls
				and then calls camera.ProcessMouseMovement() to handle them.
			scroll_callback()
				 Used to handle the scroll zoom.

			Main():
				 Main starts by making a context window and setting the input catures using 
				glfw functions. Then it creates the needed object: the sphere, the model for
				the head and the shader for each. Since the buffers are created, filled and
				bound by the objects it moves to the while loop to draw.
				 In the while loop we start by updating our currentFrame, deltaTime and 
				lastFrame timers. These values store the time between frames and are used to
				smooth the movement of the camera and sphere.
				 After that we set up the sphere. We start by activating the shader and 
				calculating the next position of the sphere in the rotation. For the rotation
				we use the globals: 
						glm::vec3 lightPos;
						float lightSpeed;
						float rotationAngle;
						float radius;
				To move the sphere around the head we need to keep changing its x and z 
				coordinates while keeping the y the same, for that we use:
						rotationAngle += lightSpeed * deltaTime;
						lightPos.x = radius * cos(rotationAngle);
						lightPos.z = radius * sin(rotationAngle);
				After that we calculate the view, projection and model uniforms that are needed
				by the vertex shader, we pass them to it and draw the sphere.
				 Next we draw the head. The projection and view uniforms are the same as the 
				spheres since they depend on the cameras perspective so we dont need to 
				re-calculate them and we pass them. We also already have calculated the light 
				source position. So we only need to calculate the model before sending it to the
				shader. We move it in the center using 
					glm::translate(modelW, glm::vec3(0.0f, 0.0f, 0.0f)); 
				and we scale it with 
					modelW = glm::scale(modelW, glm::vec3(headsize, headsize, headsize));
				Then we pass all the uniforms to the shaders and call draw.
				 At the end of each loop we swap the window buffers to update the image and 
				use glfwPollEvents() to check for inputs.
			
