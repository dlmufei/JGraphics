// Graphics.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Graphics.h"
#define MAX_LOADSTRING 100
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名


HWND hWnd;
HDC	g_hdc;
HGLRC g_hRC;
RECT g_clientRect;
int	g_cliHeight;
int	g_cliWidth;
GLfloat	g_aspect;
Color g_defColor(0, 1.0f, 0);
Color g_whiteColor(1.0f, 1.0f, 1.0f);
Color g_blackColor(0, 0, 0);
Color g_redColor(1, 0, 0);
float g_defSize = 1.0f;
wchar_t explain_draw_line[] = L"按下鼠标左键并拖动，松开后完成绘制";
wchar_t explain_draw_circle[] = L"按下鼠标左键并拖动绘制，松开结束";
wchar_t explain_draw_polygon[] = L"按下鼠标左键并拖动，松开时确定第一条边，然后按下鼠标左键并拖动，松开时确定下一个顶点，点击鼠标右键结束绘制。";
wchar_t explain_draw_bezier[] = L"按下鼠标左键并拖动，松开时确定第一第二个点，继续按下鼠标左键并拖动，确定下一个顶点，点击鼠标右键结束绘制。";
wchar_t explain_fill[] = L"点击待填充的区域即可";
wchar_t error_move[] = L"请先绘制一个多边形";
wchar_t explain_move[] = L"按下左键拖动平移，按下右键拖动旋转与放缩";
wchar_t explain_cut[] = L"按下鼠标左键并拖动绘制多边形，按下鼠标右键进行裁剪";
wchar_t explain_cube[] = L"按下鼠标左键拖动旋转";
std::vector<PicElem*> vec;
MODE mode = mode_PENCIL;
bool is_lbtn_down = false;
bool is_rbtn_down = false;
Point lastPoint(0,0);
Cube cube;
// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。
	vec.push_back(new Lines);
	
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GRAPHICS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	//createPixels();
	GLOnCreate();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (g_cliWidth > 0 && g_cliHeight > 0)
		resize_Viewport(g_cliWidth, g_cliHeight);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICS));
	GL_show();
    MSG msg;
	
    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GRAPHICS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_DLGFRAME | WS_SYSMENU,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   ChangeDisplaySettings(NULL, 0);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
	case WM_CREATE:
		//AllocConsole();

		//freopen("conout$", "w", stdout);
		
		GetClientRect(hWnd, &g_clientRect);
		g_cliWidth = g_clientRect.right - g_clientRect.left;
		g_cliHeight = g_clientRect.bottom - g_clientRect.top;
		createPixels();
		break;
	case WM_PAINT:
		GL_show();
		::ValidateRect(hWnd, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
			if (mode == mode_CUBE) {
				cube.finish();
			}
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case ID_EXPLAIN:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_EXPLAIN), hWnd, About);
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_DRAW_LINE:
				MessageBox(hWnd, explain_draw_line, L"Tips", MB_OK);
				mode = mode_LINES;
				break;
			case IDM_DRAW_CIRCLE:
				MessageBox(hWnd, explain_draw_circle, L"Tips", MB_OK);
				mode = mode_CIRCLE;
				break;
			case IDM_DRAW_POLYGON:
				MessageBox(hWnd, explain_draw_polygon, L"Tips", MB_OK);
				mode = mode_POLYGON;
				break;
			case IDM_DRAW_BEZIER:
				MessageBox(hWnd, explain_draw_bezier, L"Tips", MB_OK);
				mode = mode_Bezier;
				break;
			case ID_FILL:
			{
				MessageBox(hWnd, explain_fill, L"Tips", MB_OK);
				mode = mode_Fill;
				PicElem *p = new Fill();
				vec.push_back(p);
				break;
			}
			case ID_CUT_IN:
				MessageBox(hWnd, explain_cut, L"Tips", MB_OK);
				mode = mode_CUT_IN;
				break;
			case ID_CUT_OUT:
				MessageBox(hWnd, explain_cut, L"Tips", MB_OK);
				mode = mode_CUT_OUT;
				break;
			case ID_PENCIL:
				mode = mode_PENCIL;
				break;
			case ID_MOVE:
				if (mode != mode_POLYGON&&mode != mode_POLYGON2) {
					MessageBox(hWnd, error_move, L"Tips", MB_OK);
					mode = mode_ERROR;
				}
				else {
					MessageBox(hWnd, explain_move, L"Tips", MB_OK);
					mode = mode_MOVE;
				}
				break;
			case ID_CUBE:
				MessageBox(hWnd, explain_cube, L"Tips", MB_OK);
				mode = mode_CUBE;
				cube.init();
				InvalidateRect(hWnd, NULL, false);
				break;
			case ID_OPEN:
				readFile();
				break;
			case ID_SAVE:
				saveFile();
				break;
			case ID_CLEAR:
				vec.clear();
				initPixels();
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
			InvalidateRect(hWnd, NULL, false);
        }
        break;
	case WM_LBUTTONDOWN:
		is_lbtn_down = true;
		switch (mode) {
		case mode_PENCIL:
		{
			//wchar_t str[10];
			//swprintf(str, 63, L"%d", LOWORD(lParam));
			//MessageBox(hWnd, str, L"Tips", MB_OK);
			Point pnt(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
			PicElem* p = new Pencil;
			p->add(pnt, g_defColor, g_defSize);
			vec.push_back(p);
			break;
		}
		case mode_LINES:
			addLines(lParam, lParam);
			break;
		case mode_CIRCLE:
		{
			Point pnt(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
			PicElem* p = new Circles;
			p->add(pnt, g_defColor, g_defSize);
			vec.push_back(p);
			break;
		}
		case mode_CUT_OUT:
		case mode_CUT_IN:
		{
			Point pnt(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
			PicElem* p = new Cut;
			p->add(pnt, pnt, g_redColor, g_defSize);
			p->add(pnt, g_redColor, g_defSize);
			vec.push_back(p);
			if(mode==mode_CUT_IN) mode = mode_CUT_IN2;
			else mode = mode_CUT_OUT2;
			break;
		}
		case mode_POLYGON:
		{
			Point pnt(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
			PicElem* p = new Polygons;
			p->add(pnt, pnt, g_defColor, g_defSize);
			p->add(pnt, g_defColor, g_defSize);
			vec.push_back(p);
			mode = mode_POLYGON2;
			break;
		}
		case mode_Bezier:
		{
			Point pnt(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
			PicElem* p = new Bezier();
			p->add(pnt, pnt, g_defColor, g_defSize);
			vec.push_back(p);
			mode = mode_Bezier2;
			break;
		}
		case mode_Fill:
			addLastPoint(lParam);
			InvalidateRect(hWnd, NULL, false);
			break;
		case mode_MOVE:
			lastPoint = Point(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
			break;
		case mode_CUBE:
			lastPoint = Point(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
			break;
		}
		break;
	case WM_LBUTTONUP:
		is_lbtn_down = false;
		switch (mode)
		{

		case mode_LINES:
			vec[0]->update();
			break;
		case mode_Bezier2:
			addLastPoint(lParam);
			vec[vec.size() - 1]->update();
			break;
		case mode_CUT_OUT2:
		case mode_CUT_IN2:
		case mode_POLYGON2:
			addLastPoint(lParam);
			break;
		case mode_CIRCLE:
			vec[vec.size() - 1]->update();
			break;
		case mode_MOVE:
			vec[vec.size() - 1]->update();
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_RBUTTONDOWN:
		is_rbtn_down = true;
		switch (mode)
		{
		case mode_CUT_IN2:
			mode = mode_CUT_IN;
			((Cut*)vec[vec.size() - 1])->cut_inside();
			break;
		case mode_CUT_OUT2:
			mode = mode_CUT_OUT;
			((Cut*)vec[vec.size() - 1])->cut_outside();
			break;
		case mode_POLYGON2:
			mode = mode_POLYGON;
			break;
		case mode_Bezier2:
			mode = mode_Bezier;
			vec[vec.size() - 1]->update();
			break;
		case mode_MOVE:
			lastPoint = Point(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
			((Polygons*)vec[vec.size() - 1])->clear();
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_RBUTTONUP:
		is_rbtn_down = false;
		if(mode==mode_MOVE) vec[vec.size() - 1]->update();
		break;
	case WM_MOUSEMOVE:
		if (is_lbtn_down) {
			switch (mode) {
			case mode_PENCIL:addLastPoint(lParam); break;
			case mode_LINES:modifyLastLine(lParam); break;
			case mode_CUT_OUT2:
			case mode_CUT_IN2:
			case mode_Bezier2:
			case mode_POLYGON2:
			case mode_CIRCLE: modifyLastPoint(lParam); break;
			case mode_MOVE:
			{
				Point p(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
				((Polygons*)vec[vec.size() - 1])->move(p.pnt[0] - lastPoint.pnt[0], p.pnt[1] - lastPoint.pnt[1]);
				lastPoint = p;
				break;
			}
			case mode_CUBE:
			{
				Point p(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
				cube.rotate(lastPoint, p);
				lastPoint = p;
				break;
			}
			default:break;
			}
			InvalidateRect(hWnd, NULL, false);
		}
		if (is_rbtn_down) {
			switch (mode)
			{
			case mode_MOVE:
			{
				Point p(LOWORD(lParam), g_clientRect.bottom - HIWORD(lParam));
				//double angle = (double)(p.pnt[0] - lastPoint.pnt[0] + p.pnt[1] - lastPoint.pnt[1])/(g_cliHeight+g_cliWidth)*PI2;
				((Polygons*)vec[vec.size() - 1])->rotation(lastPoint,p);
				lastPoint = p;
				break;
			}
			default:
				break;
			}
			InvalidateRect(hWnd, NULL, false);
		}
		break;
	case WM_SIZE:
	{
		GetClientRect(hWnd, &g_clientRect);
		g_cliWidth = g_clientRect.right - g_clientRect.left;
		g_cliHeight = g_clientRect.bottom - g_clientRect.top;
		if (g_cliWidth > 0 && g_cliHeight > 0)
			resize_Viewport(g_cliWidth, g_cliHeight);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

