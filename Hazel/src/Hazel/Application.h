#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Window.h"

namespace Hazel {

    class HAZEL_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);
    private:

        bool OnWindowClose(WindowCloseEvent& e);
        std::unique_ptr<Window> m_Window;       //智能指针管理，同一时间只能管理一个窗口，m_Window是一个指针
        bool m_Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();

}
