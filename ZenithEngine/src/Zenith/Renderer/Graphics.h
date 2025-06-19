#pragma once

namespace Zenith
{
	class Window;

	class Graphics
	{
	public:
		enum class API { D3D11, OpenGL, None };

	public:
		Graphics(Window& targetWindow);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		virtual ~Graphics();

		virtual void EndFrame();
		virtual void ClearBuffer(float red, float green, float blue, float alpha) noexcept;
		virtual void DrawIndexed(unsigned int count);

		API GetAPIType() const noexcept;

	protected:
		Window& m_TargetWindow;
		API m_CurrentAPI;
	};
}
