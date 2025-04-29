#include "hzpch.h"      //需要与premake里面的pchheader "hzpch.h"一致

#include "Application.h"
#include "Hazel/Log.h"
#include "Input.h"
#include "glad/glad.h"
namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;     //一次只能有一个实例

    Application::Application()
    {     
        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;      //this指向application实例

        //Application创建窗口，控制ui等整体业务逻辑
        m_Window = std::unique_ptr<Window>(Window::Create());
        //保存窗口事件回调函数
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VertexArray);       //创建vao，绘图配方
        glBindVertexArray(m_VertexArray);           //绑定为当前操作对象

        glGenBuffers(1, &m_VertexBuffer);           //创建vbo，原料仓库
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);  //绑定到GL_ARRAY_BUFFER

        float vertices[3 * 3] = {
            -1.0f, 0.0f, 0.0f,     //左下角（x，y，z）
             1.0f, 0.0f, 0.0f,     //右下角
             0.0f,  0.5f, 0.0f      //顶点
        };
        
        //数据传到vbo
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);      //静态数据静态绘制

        //取数据
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        //ibo索引
        glGenBuffers(1, &m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

        //索引数组和传递索引数组
        unsigned int indices[3] = { 0, 1, 2 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }


    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )        //逆向遍历处理事件
        {
            (*--it)->OnEvent(e);        //这个调用的是当前层的onevent
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1);       //窗口颜色
            glClear(GL_COLOR_BUFFER_BIT);

            //方便多个vao切换
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);      //绘制三角形，3个索引数量，索引int型

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();      //是层级的onupdata，处理onupdata

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();       //处理onevent
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}
