
// ZachetDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Zachet.h"
#include "ZachetDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma once

#import "C:\\Program Files\\Autodesk\\Inventor 2022\\Bin\\RxInventor.tlb" \
rename_namespace("InventorNative") \
named_guids raw_dispinterfaces \
high_method_prefix("Method") \
rename("DeleteFile", "APIDeleteFile") \
rename("CopyFile", "APICopyFile") \
rename("MoveFile", "APIMoveFile") \
rename("SetEnvironmentVariable", "APISetEnvironmentVariable") \
rename("GetObject", "APIGetObject") \
exclude("_FILETIME", "IStream", "ISequentialStream", \
"_LARGE_INTEGER", "_ULARGE_INTEGER", "tagSTATSTG", \
"IEnumUnknown", "IPersistFile", "IPersist", "IPictureDisp")

using namespace InventorNative;

CComPtr<Application> pInvApp;

PartDocumentPtr pPartDoc;
PartComponentDefinition* pPartComDef;
TransientGeometry* pTransGeom;
Transaction* pTrans;

TransactionManagerPtr pTransManager;


PlanarSketches* sketches;
PartFeatures* ft;

WorkPlanes* wp;
WorkAxes* wax;
WorkPoints* wpt;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CZachetDlg



CZachetDlg::CZachetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ZACHET_DIALOG, pParent)
	, num_otv(6)
	, d_otv(10)
	, d_raspol(50)
	//, d_det(100)
	, h_det(50)
	, d_det(100)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZachetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NUM_OTV, num_otv);
	DDV_MinMaxInt(pDX, num_otv, 1, 1000);
	DDX_Text(pDX, IDC_D_OTV, d_otv);
	DDV_MinMaxInt(pDX, d_otv, 1, 1000);
	DDX_Text(pDX, IDC_D_RASPOL, d_raspol);
	DDV_MinMaxInt(pDX, d_raspol, 1, 1000);
	//  DDX_Text(pDX, IDC_D_DET, d_det);
	//  DDV_MinMaxInt(pDX, d_det, 1, 1000);
	DDX_Text(pDX, IDC_H_DET, h_det);
	DDV_MinMaxInt(pDX, h_det, 1, 1000);
	DDX_Text(pDX, IDC_D_DET, d_det);
	DDV_MinMaxInt(pDX, d_det, 1, 1000);
}

BEGIN_MESSAGE_MAP(CZachetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_FILE, &CZachetDlg::OnBnClickedOpenFile)
	ON_BN_CLICKED(IDOK, &CZachetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_GO, &CZachetDlg::OnBnClickedGo)
END_MESSAGE_MAP()


// Обработчики сообщений CZachetDlg

BOOL CZachetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CZachetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CZachetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CZachetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CZachetDlg::OnBnClickedOpenFile()
{
	CFileDialog dlg(TRUE);
	int ChoiceFile = dlg.DoModal();
	if (ChoiceFile == IDOK)
	{
		FILE* fl;
		CString WayFile = dlg.GetPathName();
		CStringA WF(WayFile);
		fopen_s(&fl, WF, "rt");

		if (!fl)
		{
			CString Mess = L"File " + WayFile + " no found";
			AfxMessageBox(Mess);
		}
		else
		{
			MessageBox(L"Файл успешно открыт");
			char buf[100];
			double x, y;
			while (!feof(fl))
			{
				fgets(buf, 100, fl);
				if (sscanf_s(buf, "%lf %lf", &x, &y) == 2)
				{
					Point pt;
					pt.x = x;
					pt.y = y;
					Points.Add(pt);
				}
				else
				{
					AfxMessageBox(L"Data error");
				}
			}
			fclose(fl);
		}
	}
}


void CZachetDlg::OnBnClickedOk()
{
	CDialog::OnOK();
	AfxMessageBox(L"Приложение закрывается...");
	exit(0);
}

bool CZachetDlg::CheckData()
{
	if (!UpdateData())
		return false;

	d_otv /= 10;
	d_raspol /= 10;
	d_det /= 10;
	h_det /= 10;

	if (d_det / h_det != 2)
	{
		AfxMessageBox(L"Диаметр детали должен быть вдвое больше высоты");
		return false;
	}

	if (d_otv > d_raspol/2)
	{
		AfxMessageBox(L"Слишком большое значение для диаметра отверстия");
		return false;
	}

	return true;
}



void CZachetDlg::OnBnClickedGo()
{
	BeginWaitCursor();

	if (!CheckData())
		return;

	if (!pInvApp)
	{
		CLSID InvAppClsid;
		HRESULT hRes = CLSIDFromProgID(L"Inventor.Application", &InvAppClsid);
		if (FAILED(hRes)) {
			pInvApp = nullptr;
			return;
		}

		CComPtr<IUnknown> pInvAppUnk = nullptr;
		hRes = ::GetActiveObject(InvAppClsid, NULL, &pInvAppUnk);
		if (FAILED(hRes)) {
			hRes = CoCreateInstance(InvAppClsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IUnknown), (void**)&pInvAppUnk);
			if (FAILED(hRes)) {
				pInvApp = nullptr;
				return;
			}
		}
		hRes = pInvAppUnk->QueryInterface(__uuidof(Application), (void**)&pInvApp);
		if (FAILED(hRes)) {
			return;
		}
	}
	pInvApp->put_Visible(TRUE);
	pPartDoc = pInvApp->Documents->MethodAdd(kPartDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kPartDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);
	pPartDoc->DisplayName = _T("Деталь");
	pPartDoc->get_ComponentDefinition(&pPartComDef);
	pInvApp->get_TransientGeometry(&pTransGeom);
	pTransManager = pInvApp->GetTransactionManager();
	Document* Doc = CComQIPtr <Document>(pPartDoc);
	pTransManager->raw_StartTransaction(Doc, _T("Деталь"), &pTrans);
	pPartComDef->get_Sketches(&sketches);

	pPartComDef->get_WorkPlanes(&wp);
	pPartComDef->get_Features(&ft);
	pPartComDef->get_WorkAxes(&wax);
	pPartComDef->get_WorkPoints(&wpt);

	////////////////////////////////////////////// основная деталь //////////////////////////////////////////////
	
	PlanarSketch* pSketch;
	sketches->raw_Add(wp->GetItem(2), false, &pSketch);

	SketchPointPtr point[4];
	SketchLinePtr lines[4];
	SketchArcPtr arc;

	SketchPoints* skPoints;
	SketchLinesPtr skLines;
	Profiles* skProfiles;
	SketchArcsPtr skArcs;

	pSketch->get_SketchPoints(&skPoints);
	pSketch->get_SketchLines(&skLines);
	pSketch->get_Profiles(&skProfiles);
	pSketch->get_SketchArcs(&skArcs);

	point[0] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(0, 0), false);
	point[1] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(d_det/2.f, 0), false);
	point[2] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(d_det / 2.f, h_det), false);
	point[3] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(0, h_det), false);

	lines[0] = skLines->MethodAddByTwoPoints(point[0], point[1]);
	lines[1] = skLines->MethodAddByTwoPoints(point[1], point[2]);

	arc = skArcs->MethodAddByCenterStartEndPoint(point[3], point[0], point[2], true);

	Profile* pProfile;
	skProfiles->raw__AddForSolid(&pProfile);

	RevolveFeatures* ftRevolve;
	ft->get_RevolveFeatures(&ftRevolve);

	RevolveFeaturePtr revolve1 = ftRevolve->MethodAddFull(pProfile, wax->GetItem(3), kJoinOperation);

	////////////////////////////////////////////// отверстие //////////////////////////////////////////////

	PlanarSketch* pSketch1;
	sketches->raw_Add(wp->GetItem(3), false, &pSketch1);

	SketchPointPtr point1[4];
	SketchLinePtr lines1[4];
	SketchCirclePtr circ1;

	SketchPoints* skPoints1;
	SketchLinesPtr skLines1;
	SketchCirclesPtr skCircles1;
	Profiles* skProfiles1;;

	pSketch1->get_SketchPoints(&skPoints1);
	pSketch1->get_SketchLines(&skLines1);
	pSketch1->get_Profiles(&skProfiles1);
	pSketch1->get_SketchCircles(&skCircles1);

	point1[0] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d(d_raspol/2.f, 0), false);

	circ1 = skCircles1->MethodAddByCenterRadius(point1[0], d_otv/2.f);

	Profile* pProfile1;

	skProfiles1->raw__AddForSolid(&pProfile1);

	ExtrudeFeatures* ftExtrude1;
	ft->get_ExtrudeFeatures(&ftExtrude1);


	ExtrudeFeaturePtr extrude1 = ftExtrude1->MethodAddByDistanceExtent(pProfile1, 1000, kSymmetricExtentDirection, kCutOperation);

	////////////////////////////////////////////// массив отверстий //////////////////////////////////////////////

	ObjectCollection* pCollection;

	pInvApp->TransientObjects->raw_CreateObjectCollection(vtMissing, &pCollection);

	pCollection->MethodAdd(extrude1);

	CircularPatternFeatures* pCircPatFeat;

	ft->get_CircularPatternFeatures(&pCircPatFeat);

	CircularPatternFeature* circFeat = pCircPatFeat->MethodAdd(pCollection, wax->GetItem(3), true, num_otv, "360 град", true, kIdenticalCompute);

	pTrans->MethodEnd();

}