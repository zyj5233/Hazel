#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override        //sandboxapp层级
    {
        HZ_INFO("ExampleLayer::Update");
    }

    void OnEvent(Hazel::Event& event) override
    {
        HZ_TRACE("{0}", event.ToString()); 
    }

};

class Sandbox : public Hazel::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());      //创建一个example的实例压入栈
        PushOverlay(new Hazel::ImGuiLayer());
    }

    ~Sandbox()
    {

    }

};

Hazel::Application* Hazel::CreateApplication()
{
    return new Sandbox();
}