#include "hzpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Hazel {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);	//垂直翻转
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);	//加载图像文件到内部
		HZ_CORE_ASSERT(data, "Failed to load image!");	//是否加载成功/&channels：输出参数，存储图像的通道数（如RGB为3，RGBA为4）
		m_Width = width;
		m_Height = height;


		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);	//创建1个纹理对象
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);	//分配纹理空间

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//缩小过滤/线性插值
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//放大过滤/最近临插值

		//让纹理重复：s表示水平方向，t表示垂直方向（当纹理坐标超出【0，1】范围时）
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);		//注释仍然能工作，因为默认复制
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);	//把图像数据上传上面开辟的GPU纹理空间

		stbi_image_free(data);	//释放由 stbi_load加载的图像数据
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}