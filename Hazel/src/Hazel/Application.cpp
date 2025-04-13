#include "hzpch.h"      //需要与premake里面的pchheader "hzpch.h"一致

#include "Application.h"
#include "Hazel/Log.h"

#include <GLFW/glfw3.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application()
    {     
        //Application创建窗口，控制ui等整体业务逻辑
        m_Window = std::unique_ptr<Window>(Window::Create());
        //保存窗口事件回调函数
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }


    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        HZ_CORE_TRACE("{0}", e.ToString());
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}
