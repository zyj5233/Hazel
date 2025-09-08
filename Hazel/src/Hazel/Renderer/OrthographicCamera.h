#pragma once

#include <glm/glm.hpp>   

namespace Hazel{

    class OrthographicCamera 
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        const glm::vec3& GetPosition() const { return m_Position; }     //获取摄像机位置//读操作
        void SetPosition(const glm::vec3& position) {       //写操作，无法获取位置
            m_Position = position;  //更新位置
            RecalculateViewMatrix();    //更新V矩阵
        }

        float GetRotation() const { return m_Rotation; }        //获取z轴的旋转角度
        void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }     //获取P矩阵
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }     //获取V矩阵
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }     //获取P*V矩阵
    private:
        void RecalculateViewMatrix();       //计算观察矩阵
    private:
        glm::mat4 m_ProjectionMatrix;       //p矩阵
        glm::mat4 m_ViewMatrix;             //v矩阵
        glm::mat4 m_ViewProjectionMatrix;   //P*V矩阵（矩阵是左乘）

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };// 初始化相机位置，由于是（0，0，0），v矩阵的位移矩阵就是单位矩阵
        float m_Rotation = 0.0f;					// 绕z轴的旋转角度
    };
}

