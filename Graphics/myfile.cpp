#include "stdafx.h"
#include "myfile.h"
const int magic_num = 19950929;
void readFile()
{
	OPENFILENAME opfn;
	WCHAR strFilename[MAX_PATH];//����ļ���  
								//��ʼ��  
	ZeroMemory(&opfn, sizeof(OPENFILENAME));
	opfn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С  
											//���ù���  
	opfn.lpstrFilter = L"�����ļ�\0*.*\0";
	//Ĭ�Ϲ�����������Ϊ1  
	opfn.nFilterIndex = 1;
	//�ļ������ֶα����Ȱѵ�һ���ַ���Ϊ \0  
	opfn.lpstrFile = strFilename;
	opfn.lpstrFile[0] = '\0';
	opfn.nMaxFile = sizeof(strFilename);
	//���ñ�־λ�����Ŀ¼���ļ��Ƿ����  
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	//opfn.lpstrInitialDir = NULL;  
	// ��ʾ�Ի������û�ѡ���ļ�  
	if (!GetOpenFileName(&opfn))
	{
		return;
	}
	HANDLE hFile = ::CreateFile(strFilename,     //�����ļ������ơ�
		GENERIC_READ,          // ���ļ���
		0,                      // �������д��
		NULL,                   // ȱʡ��ȫ���ԡ�
		OPEN_ALWAYS,          // ����ļ����ڣ�Ҳ������
		FILE_ATTRIBUTE_NORMAL, // һ����ļ���       
		NULL);                 // ģ���ļ�Ϊ�ա�
	if (hFile == INVALID_HANDLE_VALUE)
	{
	}
	DWORD dwReadSize = 0;
	int num;
	BOOL bRet = ::ReadFile(hFile, &num, 4, &dwReadSize, NULL);
	if (num != magic_num) {
		CloseHandle(hFile);
		MessageBox(hWnd, L"����ʶ����ļ�", L"ERROR", MB_OK);
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
		//	std::cout << "���ļ��ɹ�\n";
		//}
		//else
		//{
		//	//��ȡ�����롣
		//	DWORD dwError = GetLastError();
		//	//�������            
		//	TCHAR chErrorBuf[1024];
		//	std::cout << "error:" << dwError << "\n";
		//}
	}
	CloseHandle(hFile);
}

void saveFile()
{
	OPENFILENAME opfn;
	WCHAR strFilename[MAX_PATH];//����ļ���  
								//��ʼ��  
	ZeroMemory(&opfn, sizeof(OPENFILENAME));
	opfn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С  
											//���ù���  
	opfn.lpstrFilter = L"�����ļ�\0*.*\0";
	//Ĭ�Ϲ�����������Ϊ1  
	opfn.nFilterIndex = 1;
	//�ļ������ֶα����Ȱѵ�һ���ַ���Ϊ \0  
	opfn.lpstrFile = strFilename;
	opfn.lpstrFile[0] = '\0';
	opfn.nMaxFile = sizeof(strFilename);
	//���ñ�־λ�����Ŀ¼���ļ��Ƿ����  
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	//opfn.lpstrInitialDir = NULL;  
	// ��ʾ�Ի������û�ѡ���ļ�  
	if (!GetSaveFileName(&opfn))
	{
		return;
	}
	HANDLE hFile = ::CreateFile(strFilename,     //�����ļ������ơ�
		GENERIC_WRITE | GENERIC_READ,          // д�Ͷ��ļ���
		0,                      // �������д��
		NULL,                   // ȱʡ��ȫ���ԡ�
		CREATE_ALWAYS,          // ����ļ����ڣ�Ҳ������
		FILE_ATTRIBUTE_NORMAL, // һ����ļ���       
		NULL);                 // ģ���ļ�Ϊ�ա�
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