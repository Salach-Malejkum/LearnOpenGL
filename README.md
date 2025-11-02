# LearnOpenGL — Portfolio: Game Rendering Programmer

Small, focused OpenGL example project intended as a portfolio piece. It demonstrates basic GLFW/GLAD initialization, a simple Shader wrapper, and a minimal rendering loop showing two triangles.

Current contents
- main.cpp       — application entry: window setup, vertex data, and render loop
- Shader.h/.cpp  — lightweight shader wrapper (load, compile, link, basic uniforms)
- vertex.glsl    — vertex shader used by the example
- fragment.glsl  — fragment shader with a uniform color toggle

Build & run (quick)
- Requires GLFW and GLAD (and a C++17 toolchain).
- Example compile (adjust include/library paths as needed):
  g++ -std=c++17 LearnOpenGL/main.cpp LearnOpenGL/Shader.cpp -I/path/to/glad/include -lglfw -ldl -o LearnOpenGL

Notes
- The project is intentionally small. It is a starting point and will be expanded and documented further.
- Code aims for clarity; later changes may introduce small refactors (RAII wrappers, utilities, build scripts).

Author
- Salach-Malejkum — Game Rendering Programmer (portfolio)
  - GitHub: https://github.com/Salach-Malejkum
