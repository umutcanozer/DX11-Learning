# DirectX11 Learning Project 🎮

This project was created as a personal journey to understand the fundamentals of DirectX 11 using C++.  
It focuses on building a basic graphics application from scratch with some documentations and tutorials.

---

## 🔍 What I Learned

- How the DirectX 11 rendering pipeline works
- Creating a window and handling Win32 messages manually
- Initializing Direct3D device, swap chain, render targets, and depth buffer
- Compiling and using HLSL shaders (vertex + pixel)
- Sending World, View, Projection matrices via Constant Buffers
- Drawing geometry using vertex/index buffers
- Understanding GPU vs CPU responsibilities in the graphics pipeline
- Basic rotation and transformation math using DirectXMath

---

## 🧠 Why I Built This
 
This project is a stepping stone towards building a more complete custom engine and learning advanced concepts like:

- ECS (Entity-Component-System)
- Camera movement
- Lighting
- Model loading
- ImGui integration
- Physics component integration

---

## ⚒️ What's Next

> I'm currently working on refactoring this project into a cleaner architecture with systems like `Window`, `Input`, `Graphics`, and a basic `GameObject-Component` structure — with a future goal of transitioning into ECS and building editor tools.

---

## 💬 Note

This repository is a work-in-progress learning project.  
### 📐 Matrices (World, View, Projection)

- **World Matrix**: Positions and rotates each object in the 3D scene.
- **View Matrix**: Represents the camera's position and orientation — transforms world-space to view-space.
- **Projection Matrix**: Transforms 3D coordinates into 2D screen coordinates (perspective division).

I used `XMMatrixLookAtLH`, `XMMatrixPerspectiveFovLH`, and `XMMatrixRotationY` to build them.

---

### 🎨 Shaders (HLSL)

- Wrote basic `VertexShader.hlsl` and `PixelShader.hlsl`.
- Learned how to send data to the shaders using **constant buffers**.
- Understood semantics like `POSITION`, `COLOR`, and `SV_POSITION`.

---

### ⚙️ DirectX 11 Components

- `ID3D11Device`, `ID3D11DeviceContext` for rendering.
- `IDXGISwapChain` for presenting the frame to the screen.
- `ID3D11RenderTargetView`, `ID3D11DepthStencilView` for setting up the framebuffer.
- `ID3D11Buffer` for vertex/index/constant data.

---

### 📦 Constant Buffers

- Used `D3D11_USAGE_DYNAMIC` buffers with `Map` and `Unmap` to update them every frame.
- Sent matrices to shaders using a custom `MatrixBufferType` struct.
