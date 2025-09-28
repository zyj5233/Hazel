#pragma once

#include "Hazel/Core/Window.h"

#include <GLFW/glfw3.h>
#include "Hazel/Renderer/GraphicsContext.h"
namespace Hazel {

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // 设置Application的回调函数，必须先保存回调，就像吃饭不可能客人来一个就上一个菜，先保存需求
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const { return m_Window; }   //无类型指针，glfw窗口指针
    private:
        virtual void Init(const WindowProps& props);    //初始化和销毁设为虚函数，方便GLWF等窗口重载
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

}