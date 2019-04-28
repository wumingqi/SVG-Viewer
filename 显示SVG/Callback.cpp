#include "Pch.h"
#include "Application.h"


#define HANDLE_MSG(msg) case msg: return app.Handle_##msg(wParam, lParam);
LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Application& app = *(reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)));
	switch (msg)
	{
		HANDLE_MSG(WM_CREATE);
		//HANDLE_MSG(WM_PAINT);
		HANDLE_MSG(WM_SIZE);
		HANDLE_MSG(WM_KEYDOWN);
		HANDLE_MSG(WM_LBUTTONDOWN);
		HANDLE_MSG(WM_LBUTTONUP);
		HANDLE_MSG(WM_RBUTTONDOWN);
		HANDLE_MSG(WM_RBUTTONUP);
		HANDLE_MSG(WM_MOUSEMOVE);
		HANDLE_MSG(WM_MOUSEWHEEL);
		HANDLE_MSG(WM_DROPFILES);
		HANDLE_MSG(WM_COMMAND);
		HANDLE_MSG(WM_DESTROY);
	default:return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
#undef HANDLE_MSG

///<summary>当窗口创建时触发此消息</summary>
///<param name="wParam">This parameter is not used.</param>
///<param name="lParam">指向CREATESTRUCT结构体的指针</param>
LRESULT Application::Handle_WM_CREATE(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

///<summary>处理WM_PAINT消息，当客户区需要被更新时触发</summary>
///<param name="wParam">This parameter is not used.</param>
///<param name="lParam">This parameter is not used.</param>
LRESULT Application::Handle_WM_PAINT(WPARAM wParam, LPARAM lParam)
{
	ValidateRect(m_hWnd, nullptr);
	return 0;
}


///<summary>处理WM_LBUTTONDOWN消息，鼠标左键按下时触发</summary>
///<param name="wParam">表示是否有虚拟按键按下，如MK_CONTROL等</param>
///<param name="lParam">相对于客户区左上角的光标的坐标</param>
LRESULT Application::Handle_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


///<summary>处理WM_LBUTTONUP消息，鼠标左键松开时触发</summary>
///<param name="wParam">表示是否有虚拟按键按下，如MK_CONTROL等</param>
///<param name="lParam">相对于客户区左上角的光标的坐标</param>
LRESULT Application::Handle_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


///<summary>处理WM_RBUTTONDOWN消息，鼠标右键按下时触发</summary>
///<param name="wParam">表示是否有虚拟按键按下，如MK_CONTROL等</param>
///<param name="lParam">相对于客户区左上角的光标的坐标</param>
LRESULT Application::Handle_WM_RBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

///<summary>处理WM_RBUTTONUP消息，鼠标右键松开时触发</summary>
///<param name="wParam">表示是否有虚拟按键按下，如MK_CONTROL等</param>
///<param name="lParam">相对于客户区左上角的光标的坐标</param>
LRESULT Application::Handle_WM_RBUTTONUP(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


///<summary>处理WM_MOUSE消息，鼠标在客户区移动时触发</summary>
///<param name="wParam">表示是否有虚拟按键按下，如MK_CONTROL等</param>
///<param name="lParam">相对于客户区左上角的光标的坐标</param>
LRESULT Application::Handle_WM_MOUSEMOVE(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}


///<summary>处理WM_SIZE消息，当窗口大小改变时触发</summary>
///<param name="wParam">表示大小改变的类型，如SIZE_MAXIMIZED、SIZE_HIDE等</param>
///<param name="lParam">窗口客户区的大小</param>
LRESULT Application::Handle_WM_SIZE(WPARAM wParam, LPARAM lParam)
{
	m_width = LOWORD(lParam); m_height = HIWORD(lParam);
	m_deviceResources->SetTargetSize(m_width, m_height);
	m_deviceResources->CreateWindowSizeDependentResources();
	Update();
	return 0;
}


///<summary>处理WM_KEYDOWN消息，当键盘按键按下时触发</summary>
///<param name="wParam">非系统键的虚拟键值（Virtual-Key Code）</param>
///<param name="lParam">其他信息</param>
LRESULT Application::Handle_WM_KEYDOWN(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_F1:
		break;
	case VK_F2:
		break;
	}
	return 0;
}


///<summary>处理WM_MOUSEWHEEL消息，当鼠标滚轮滚动时触发</summary>
///<param name="wParam">
///高字表明滚轮滚动的方向，正值表明向前滚，负值表明向后滚
///低字表明时候有其他虚拟键按下（MK_CONTROL、等）
///</param>
///<param name="lParam">鼠标的坐标位置，相对于屏幕左上角</param>
LRESULT Application::Handle_WM_MOUSEWHEEL(WPARAM wParam, LPARAM lParam)
{
	auto flag = HIWORD(wParam) & 0x8000;
	if (LOWORD(wParam) == MK_CONTROL)
	{
		if (flag)
		{
		}
		else
		{
		}
	}
	return 0;
}

///<summary>处理WM_DROPFILES消息</summary>
///<param name="wParam">一个指向描述拖拽文件的内部结构体的句柄，HDROP</param>
///<param name="lParam">Must be zero.</param>
LRESULT Application::Handle_WM_DROPFILES(WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop = (HDROP)wParam;
	TCHAR filename[MAX_PATH];
	UINT count = DragQueryFile(hDrop, -1, nullptr, 0);
	for (UINT i = 0; i < count; i++)
	{
		DragQueryFile(hDrop, i, filename, _countof(filename));
		//处理获取到的文件名

	}
	return 0;
}

///<summary>处理WM_COMMAND消息：
///1、用户选择某个菜单项时。2、控件发送通知码给父窗口。3、加速键被触发。
///</summary>
/*
Message Source 	wParam(high word) 						wParam(low word) 				lParam
Menu 			0 										Menu identifier(IDM_*) 			0
Accelerator 	1 										Accelerator identifier(IDM_*) 	0
Control 		Control - defined notification code 	Control identifier				Handle to the control window
*/
LRESULT Application::Handle_WM_COMMAND(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


LRESULT Application::Handle_WM_DESTROY(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}

