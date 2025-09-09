#pragma once

namespace Hazel {

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		//不隐式转换，很不直观，直接显示调用
		//operator float() const { return m_Time; }		//隐式转换Timestep对象为float类型

		float GetSeconds() const { return m_Time; }		//返回秒
		float GetMilliseconds() const { return m_Time * 1000.0f; }	//返回毫秒
	private:
		float m_Time;
	};

}