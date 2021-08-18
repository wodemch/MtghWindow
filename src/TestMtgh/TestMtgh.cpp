// TestMtgh.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include"../MtghWindow/MtghWindow.h"

#ifdef _WIN64
#pragma comment(lib, "../bin/MtghWindow_x64.lib")
#else
#pragma comment(lib, "../bin/MtghWindow_Win32.lib")
#endif


int main()
{
    std::cout << "Hello World!\n";
	int res = -1;
	while (true)
	{
		//res = MT_IsDirHaveFolder("E:\\111", "数字单1");

		//res	= MT_IsExist("E:\\111\\lop",true);

		/*vector<string> VRes;
		MT_LoadAllPath("E:\\111", VRes,false);

		for each (string var in VRes)
		{
			printf("%s\n", var.c_str());
		}
		printf("=======================\n");
		VRes.clear();
		MT_LoadAllFile("E:\\111", VRes, false);
		for each (string var in VRes)
		{
			printf("%s\n", var.c_str());
		}*/

		res = MT_CopyFile("E:\\111\\cap.jpg", "E:\\111\\456\\2\\cap.jpg");

		//res = MT_Delete("E:\\111\\1111");

		//res = MT_StartExe("E:\\111\\ColorDiff.exe",SF_OPENMORE);
		//res = MT_KillExe("ColorDiff.exe");
		string path;
		vector<string> vPath;
		res = MT_CmdOrder(CT_OPEN,"E:\\111");
		printf("%d----%s\n", res, path.c_str());
		getchar();
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
