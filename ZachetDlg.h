
// ZachetDlg.h: файл заголовка
//

#pragma once


// Диалоговое окно CZachetDlg
class CZachetDlg : public CDialogEx
{
// Создание
public:
	CZachetDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZACHET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenFile();
	struct Point
	{
		double x, y;
	};

	CArray <Point, Point> Points;

	afx_msg void OnBnClickedOk();
	bool CheckData();
	int num_otv;
	int d_otv;
	int d_raspol;

	afx_msg void OnBnClickedGo();
//	int d_det;
	int h_det;
	int d_det;
};
