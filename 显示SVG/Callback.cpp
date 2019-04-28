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

///<summary>�����ڴ���ʱ��������Ϣ</summary>
///<param name="wParam">This parameter is not used.</param>
///<param name="lParam">ָ��CREATESTRUCT�ṹ���ָ��</param>
LRESULT Application::Handle_WM_CREATE(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

///<summary>����WM_PAINT��Ϣ�����ͻ�����Ҫ������ʱ����</summary>
///<param name="wParam">This parameter is not used.</param>
///<param name="lParam">This parameter is not used.</param>
LRESULT Application::Handle_WM_PAINT(WPARAM wParam, LPARAM lParam)
{
	ValidateRect(m_hWnd, nullptr);
	return 0;
}


///<summary>����WM_LBUTTONDOWN��Ϣ������������ʱ����</summary>
///<param name="wParam">��ʾ�Ƿ������ⰴ�����£���MK_CONTROL��</param>
///<param name="lParam">����ڿͻ������ϽǵĹ�������</param>
LRESULT Application::Handle_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


///<summary>����WM_LBUTTONUP��Ϣ���������ɿ�ʱ����</summary>
///<param name="wParam">��ʾ�Ƿ������ⰴ�����£���MK_CONTROL��</param>
///<param name="lParam">����ڿͻ������ϽǵĹ�������</param>
LRESULT Application::Handle_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


///<summary>����WM_RBUTTONDOWN��Ϣ������Ҽ�����ʱ����</summary>
///<param name="wParam">��ʾ�Ƿ������ⰴ�����£���MK_CONTROL��</param>
///<param name="lParam">����ڿͻ������ϽǵĹ�������</param>
LRESULT Application::Handle_WM_RBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

///<summary>����WM_RBUTTONUP��Ϣ������Ҽ��ɿ�ʱ����</summary>
///<param name="wParam">��ʾ�Ƿ������ⰴ�����£���MK_CONTROL��</param>
///<param name="lParam">����ڿͻ������ϽǵĹ�������</param>
LRESULT Application::Handle_WM_RBUTTONUP(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


///<summary>����WM_MOUSE��Ϣ������ڿͻ����ƶ�ʱ����</summary>
///<param name="wParam">��ʾ�Ƿ������ⰴ�����£���MK_CONTROL��</param>
///<param name="lParam">����ڿͻ������ϽǵĹ�������</param>
LRESULT Application::Handle_WM_MOUSEMOVE(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}


///<summary>����WM_SIZE��Ϣ�������ڴ�С�ı�ʱ����</summary>
///<param name="wParam">��ʾ��С�ı�����ͣ���SIZE_MAXIMIZED��SIZE_HIDE��</param>
///<param name="lParam">���ڿͻ����Ĵ�С</param>
LRESULT Application::Handle_WM_SIZE(WPARAM wParam, LPARAM lParam)
{
	m_width = LOWORD(lParam); m_height = HIWORD(lParam);
	m_deviceResources->SetTargetSize(m_width, m_height);
	m_deviceResources->CreateWindowSizeDependentResources();
	Update();
	return 0;
}


///<summary>����WM_KEYDOWN��Ϣ�������̰�������ʱ����</summary>
///<param name="wParam">��ϵͳ���������ֵ��Virtual-Key Code��</param>
///<param name="lParam">������Ϣ</param>
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


///<summary>����WM_MOUSEWHEEL��Ϣ���������ֹ���ʱ����</summary>
///<param name="wParam">
///���ֱ������ֹ����ķ�����ֵ������ǰ������ֵ��������
///���ֱ���ʱ����������������£�MK_CONTROL���ȣ�
///</param>
///<param name="lParam">��������λ�ã��������Ļ���Ͻ�</param>
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

///<summary>����WM_DROPFILES��Ϣ</summary>
///<param name="wParam">һ��ָ��������ק�ļ����ڲ��ṹ��ľ����HDROP</param>
///<param name="lParam">Must be zero.</param>
LRESULT Application::Handle_WM_DROPFILES(WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop = (HDROP)wParam;
	TCHAR filename[MAX_PATH];
	UINT count = DragQueryFile(hDrop, -1, nullptr, 0);
	for (UINT i = 0; i < count; i++)
	{
		DragQueryFile(hDrop, i, filename, _countof(filename));
		//�����ȡ�����ļ���

	}
	return 0;
}

///<summary>����WM_COMMAND��Ϣ��
///1���û�ѡ��ĳ���˵���ʱ��2���ؼ�����֪ͨ��������ڡ�3�����ټ���������
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

