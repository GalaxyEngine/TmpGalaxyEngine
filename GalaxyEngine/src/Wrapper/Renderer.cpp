#include "pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Wrapper/Renderer.h"
#include "Wrapper/Window.h"
#include "Resource/Texture.h"
#include "Resource/Shader.h"

std::unique_ptr<Wrapper::Renderer> Wrapper::Renderer::m_instance = nullptr;

void Wrapper::Renderer::CreateInstance(RenderAPI renderAPI)
{
	switch (renderAPI)
	{
	case RenderAPI::OPENGL:
	{
		m_instance = std::make_unique<Wrapper::OpenGLRenderer>();
		break;
	}
	default:
		break;
	}
	m_instance->Initalize();
	m_instance->EnableDebugOutput();
}

// OpenGL Renderer
Wrapper::OpenGLRenderer::OpenGLRenderer() {}

Wrapper::OpenGLRenderer::~OpenGLRenderer() {}


void Wrapper::OpenGLRenderer::Initalize()
{
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(GetProcAddress))) {
		PrintError("Failed to initialize GLAD");
		return;
	}

	const GLubyte* glVersion = glGetString(GL_VERSION);
	PrintLog("Loaded OpenGL %s\n", glVersion);
	p_initalized = true;
}

void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204 || id == 131154) return;

	std::string log = "";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             log += "Source: API\n"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   log += "Source: Window System\n"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: log += "Source: Shader Compiler\n"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     log += "Source: Third Party\n"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     log += "Source: Application\n"; break;
	case GL_DEBUG_SOURCE_OTHER:           log += "Source: Other\n"; break;
	}
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               log += "Type: Error\n"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: log += "Type: Deprecated Behaviour\n"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  log += "Type: Undefined Behaviour\n"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         log += "Type: Portability\n"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         log += "Type: Performance\n"; break;
	case GL_DEBUG_TYPE_MARKER:              log += "Type: Marker\n"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          log += "Type: Push Group\n"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           log += "Type: Pop Group\n"; break;
	case GL_DEBUG_TYPE_OTHER:               log += "Type: Other\n"; break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         LOG(Debug::LogType::L_ERROR, "Debug message (%d): %s\n%s", id, message, log.c_str()); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG(Debug::LogType::L_WARNING, "Debug message (%d): %s\n%s", id, message, log.c_str()); break;
	case GL_DEBUG_SEVERITY_LOW:          LOG(Debug::LogType::L_WARNING, "Debug message (%d): %s\n%s", id, message, log.c_str()); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG(Debug::LogType::L_INFO, "Debug message (%d): %s\n%s", id, message, log.c_str()); break;
	}
}

void Wrapper::OpenGLRenderer::EnableDebugOutput()
{
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(DebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

void Wrapper::OpenGLRenderer::UseShader(Resource::Shader* shader)
{
	glUseProgram(shader->m_id);
}

void Wrapper::OpenGLRenderer::Viewport(const Vec2i& pos, const Vec2i& size)
{
	glViewport(pos.x, pos.y, size.x, size.y);
}

void Wrapper::OpenGLRenderer::ClearColorAndBuffer(const Vec4f& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}


bool Wrapper::OpenGLRenderer::LinkShaders(Resource::Shader* shader)
{
	auto& [vertex, geometry, fragment] = shader->m_subShaders;
	if (!vertex.lock() || !vertex.lock()->HasBeenSent() || !fragment.lock() || !fragment.lock()->HasBeenSent())
		return false;
	// link shaders
	shader->m_id = glCreateProgram();
	glAttachShader(shader->m_id, vertex.lock()->m_id);
	glAttachShader(shader->m_id, fragment.lock()->m_id);
	glLinkProgram(shader->m_id);
	// check for linking errors
	int success;
	glGetProgramiv(shader->m_id, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shader->m_id, 512, NULL, infoLog);
		PrintError("Error when Link shader %s :\n %s", shader->p_fullPath.c_str(), infoLog);
		return false;
	}
	return true;
}

bool Wrapper::OpenGLRenderer::CompileVertexShader(Resource::VertexShader* vertex)
{
	// Create a vertex shader object
	vertex->m_id = glCreateShader(GL_VERTEX_SHADER);
	const char* content = vertex->p_content.c_str();
	glShaderSource(vertex->m_id, 1, &content, nullptr);

	// Compile the shader
	glCompileShader(vertex->m_id);

	// Check for compilation errors
	int success;
	glGetShaderiv(vertex->m_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint infoLogLength;
		glGetShaderiv(vertex->m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<GLchar> infoLog(infoLogLength);
		glGetShaderInfoLog(vertex->m_id, infoLogLength, nullptr, infoLog.data());

		PrintError("Vertex shader %s compilation failed:\n %s", vertex->p_fullPath.c_str(), infoLog.data());
		glDeleteShader(vertex->m_id);
		return false;
	}

	PrintLog("Vertex shader %s compiled", vertex->p_fullPath.c_str());
	return true;
}

bool Wrapper::OpenGLRenderer::CompileFragmentShader(Resource::FragmentShader* fragment)
{
	// Create a vertex shader object
	fragment->m_id = glCreateShader(GL_FRAGMENT_SHADER);
	const char* content = fragment->p_content.c_str();
	glShaderSource(fragment->m_id, 1, &content, nullptr);

	// Compile the shader
	glCompileShader(fragment->m_id);

	// Check for compilation errors
	int success;
	glGetShaderiv(fragment->m_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint infoLogLength;
		glGetShaderiv(fragment->m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<GLchar> infoLog(infoLogLength);
		glGetShaderInfoLog(fragment->m_id, infoLogLength, nullptr, infoLog.data());

		PrintError("Fragment shader %s compilation failed:\n %s", fragment->p_fullPath.c_str(), infoLog.data());
		glDeleteShader(fragment->m_id);
		return false;
	}
	PrintLog("Framgment shader %s compiled", fragment->p_fullPath.c_str());
	return true;
}

void Wrapper::OpenGLRenderer::CreateTexture(Resource::Texture* texture)
{
	uint32_t filter = TextureFilteringToAPI(texture->m_filtering);
	uint32_t format = TextureFormatToAPI(texture->m_format);

	glGenTextures(1, &texture->m_id);
	glBindTexture(GL_TEXTURE_2D, texture->m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, filter);

	glTexImage2D(GL_TEXTURE_2D, 0, format, texture->m_size.x, texture->m_size.y, 0, format, GL_UNSIGNED_BYTE, texture->m_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Wrapper::OpenGLRenderer::DestroyTexture(Resource::Texture* texture)
{
	glDeleteTextures(1, &texture->m_id);
}

uint32_t Wrapper::OpenGLRenderer::TextureFilteringToAPI(Resource::TextureFiltering filtering)
{
	switch (filtering)
	{
	case Resource::TextureFiltering::REPEAT:
		return GL_REPEAT;
	case Resource::TextureFiltering::MIRRORED_REPEAT:
		return GL_MIRRORED_REPEAT;
	case Resource::TextureFiltering::CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;
	case Resource::TextureFiltering::CLAMP_TO_BORDER:
		return GL_CLAMP_TO_BORDER;
	default:
		PrintError("Texture filtering not recognize");
		return GL_REPEAT;
	}
}

uint32_t Wrapper::OpenGLRenderer::TextureFormatToAPI(Resource::TextureFormat format)
{
	switch (format) {
	case Resource::TextureFormat::RGB:
		return GL_RGB;
	case Resource::TextureFormat::RGBA:
		return GL_RGBA;
	case Resource::TextureFormat::ALPHA:
		return GL_ALPHA;
	case Resource::TextureFormat::LUMINANCE:
		return GL_LUMINANCE;
	case Resource::TextureFormat::LUMINANCE_ALPHA:
		return GL_LUMINANCE_ALPHA;
	default:
		PrintError("Texture format not recognize");
		return GL_RGBA;
	}
}

void Wrapper::OpenGLRenderer::ShaderSendInt(uint32_t location, int value)
{
	glUniform1i(location, value);
}

void Wrapper::OpenGLRenderer::ShaderSendFloat(uint32_t location, float value)
{
	glUniform1f(location, value);
}

void Wrapper::OpenGLRenderer::ShaderSendDouble(uint32_t location, double value)
{
	glUniform1d(location, value);
}

void Wrapper::OpenGLRenderer::ShaderSendVec2f(uint32_t location, const Vec2f& value)
{
	glUniform2fv(location, 1, value.Data());
}

void Wrapper::OpenGLRenderer::ShaderSendVec3f(uint32_t location, const Vec3f& value)
{
	glUniform3fv(location, 1, value.Data());
}

void Wrapper::OpenGLRenderer::ShaderSendVec4f(uint32_t location, const Vec4f& value)
{
	glUniform4fv(location, 1, value.Data());
}

void Wrapper::OpenGLRenderer::ShaderSendVec2i(uint32_t location, const Vec2i& value)
{
	glUniform2iv(location, 1, value.Data());
}

void Wrapper::OpenGLRenderer::ShaderSendVec3i(uint32_t location, const Vec3i& value)
{
	glUniform3iv(location, 1, value.Data());
}

void Wrapper::OpenGLRenderer::ShaderSendVec4i(uint32_t location, const Vec4i& value)
{
	glUniform4iv(location, 1, value.Data());
}

void Wrapper::OpenGLRenderer::ShaderSendMat4(uint32_t location, const Mat4& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, value.Data());
}

void Wrapper::OpenGLRenderer::CreateVertexArray(uint32_t& vao)
{
	glGenVertexArrays(1, &vao);
}

void Wrapper::OpenGLRenderer::BindVertexArray(uint32_t vao)
{
	glBindVertexArray(vao);
}

void Wrapper::OpenGLRenderer::CreateAndFillVertexBuffer(uint32_t& vbo
	, const std::vector<Vec3f>& positions
	, const std::vector<Vec2f>& textureUVs
	, const std::vector<Vec3f>& normals)
{
	// Vertex buffer initialization
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vec3f), positions.data()->Data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	uint32_t m_textureUVs;
	// Texture coordinate buffer initialization
	glGenBuffers(1, &m_textureUVs);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureUVs);
	glBufferData(GL_ARRAY_BUFFER, textureUVs.size() * sizeof(Vec2f), textureUVs.data()->Data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	// Normal buffer initialization
	uint32_t m_normals;
	glGenBuffers(1, &m_normals);
	glBindBuffer(GL_ARRAY_BUFFER, m_normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vec3f), normals.data()->Data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);
}

void Wrapper::OpenGLRenderer::CreateVertexBuffer(uint32_t& vbo, const void* data, size_t dataSize)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
}

void Wrapper::OpenGLRenderer::BindVertexBuffer(uint32_t vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void Wrapper::OpenGLRenderer::CreateIndexBuffer(uint32_t& ebo, const void* data, size_t dataSize)
{
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
}

void Wrapper::OpenGLRenderer::BindIndexBuffer(uint32_t ebo)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Wrapper::OpenGLRenderer::VertexAttribPointer(uint32_t index, int size, int stride, const void* pointer)
{
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
	glEnableVertexAttribArray(index);
}

void Wrapper::OpenGLRenderer::DrawElement(size_t count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void Wrapper::OpenGLRenderer::UnbindVertexArray()
{
	glBindVertexArray(0);
}

void Wrapper::OpenGLRenderer::UnbindVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Wrapper::OpenGLRenderer::DrawArrays(size_t start, size_t count)
{
	glDrawArrays(GL_TRIANGLES, 0, count);
}
