#pragma once

#include "Hazel/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

    class HAZEL_API LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);       //同样函数以不同命名空间区分，压入普通层到层栈
        void PushOverlay(Layer* overlay);   //压入覆盖层到层栈，总是会在最上方
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }      //遍历层栈的容器
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    private:
        std::vector<Layer*> m_Layers;       //存储指向层的指针
        unsigned int m_LayerInsertIndex = 0;        //更高效
    };

}