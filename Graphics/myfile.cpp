#include "stdafx.h"
#include "myfile.h"
const int magic_num = 19950929;
void readFile()
{
	OPENFILENAME opfn;
	WCHAR strFilename[MAX_PATH];//存放文件名  
								//初始化  
	ZeroMemory(&opfn, sizeof(OPENFILENAME));
	opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小  
											//设置过滤  
	opfn.lpstrFilter = L"所有文件\0*.*\0";
	//默认过滤器索引设为1  
	opfn.nFilterIndex = 1;
	//文件名的字段必须先把第一个字符设为 \0  
	opfn.lpstrFile = strFilename;
	opfn.lpstrFile[0] = '\0';
	opfn.nMaxFile = sizeof(strFilename);
	//设置标志位，检查目录或文件是否存在  
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	//opfn.lpstrInitialDir = NULL;  
	// 显示对话框让用户选择文件  
	if (!GetOpenFileName(&opfn))
	{
		return;
	}
	HANDLE hFile = ::CreateFile(strFilename,     //创建文件的名称。
		GENERIC_READ,          // 读文件。
		0,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		OPEN_ALWAYS,          // 如果文件存在，也创建。
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。       
		NULL);                 // 模板文件为空。
	if (hFile == INVALID_HANDLE_VALUE)
	{
	}
	DWORD dwReadSize = 0;
	int num;
	BOOL bRet = ::ReadFile(hFile, &num, 4, &dwReadSize, NULL);
	if (num != magic_num) {
		CloseHandle(hFile);
		MessageBox(hWnd, L"不可识别的文件", L"ERROR", MB_OK);
		return;
	}
	vec.clear();
	initPixels();
	while(true){
		int size;
		BOOL bRet = ::ReadFile(hFile, &size, 4, &dwReadSize, NULL);
		if (dwReadSize==0) break;
		shape_type thistype;
		bRet=::ReadFile(hFile, &thistype, 4, &dwReadSize, NULL);
		int* input = new int[4 * (size-2)];
		if (size > 2) {
			bRet = ::ReadFile(hFile, input, 4 * (size - 2), &dwReadSize, NULL);
		}
		PicElem* p=NULL;
		switch (thistype)
		{
		case shape_bezier:
			p = new Bezier();
			break;
		case shape_circle:
			p = new Circles();
			break;
		case shape_cut_in:
		case shape_cut_out:
			p = new Cut();
			((Cut*)p)->type = thistype;
			break;
		case shape_fill:
			p = new Fill();
			break;
		case shape_lines:
			p = new Lines();
			break;
		case shape_pencil:
			p = new Pencil();
			break;
		case shape_polygon:
			p = new Polygons();
			break;
		default:
			continue;
			break;
		}
		vec.push_back(p);
		p->input(input, size);

		//if (bRet)
		//{
		//	std::cout << "读文件成功\n";
		//}
		//else
		//{
		//	//获取出错码。
		//	DWORD dwError = GetLastError();
		//	//处理出错。            
		//	TCHAR chErrorBuf[1024];
		//	std::cout << "error:" << dwError << "\n";
		//}
	}
	CloseHandle(hFile);
}

void saveFile()
{
	OPENFILENAME opfn;
	WCHAR strFilename[MAX_PATH];//存放文件名  
								//初始化  
	ZeroMemory(&opfn, sizeof(OPENFILENAME));
	opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小  
											//设置过滤  
	opfn.lpstrFilter = L"所有文件\0*.*\0";
	//默认过滤器索引设为1  
	opfn.nFilterIndex = 1;
	//文件名的字段必须先把第一个字符设为 \0  
	opfn.lpstrFile = strFilename;
	opfn.lpstrFile[0] = '\0';
	opfn.nMaxFile = sizeof(strFilename);
	//设置标志位，检查目录或文件是否存在  
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	//opfn.lpstrInitialDir = NULL;  
	// 显示对话框让用户选择文件  
	if (!GetSaveFileName(&opfn))
	{
		return;
	}
	HANDLE hFile = ::CreateFile(strFilename,     //创建文件的名称。
		GENERIC_WRITE | GENERIC_READ,          // 写和读文件。
		0,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		CREATE_ALWAYS,          // 如果文件存在，也创建。
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。       
		NULL);                 // 模板文件为空。
	if (hFile == INVALID_HANDLE_VALUE)
	{
	}
	DWORD dwWritenSize = 0;
	::WriteFile(hFile, &magic_num, 4, &dwWritenSize, NULL);
	for (int i = 0; i < vec.size(); i++) {
		int* output = vec[i]->output();
		if (output == NULL) continue;
		int size = output[0];
		BOOL bRet = ::WriteFile(hFile, output, 4 * size, &dwWritenSize, NULL);
		if (bRet)
		{
		}
	}
	FlushFileBuffers(hFile);
	CloseHandle(hFile);
}