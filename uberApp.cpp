// uberApp.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "uberApp.h"
#include <windows.h>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <algorithm>
#include<iostream>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void DrawMap(HWND hwnd, HDC hdc);

using namespace  std;
//#define MAX_LOADSTRING = 100;

class city
{
public:
    int id;
    int x;
    int y;
    wstring name;
    city(int id, int x, int y, const wstring name) :id(id), x(x), y(y), name(name)
    {
    }

};

struct Edge {
    int a,b;
};


class Driver
{
public:
    int id;
    wstring name;
    wstring carModel;
    wstring carNumber;
    int currentCityID;
    bool isavailable;

public:
    Driver(int id, wstring name, wstring carModel, wstring carNumber, int cityId)
    {
        this->id = id;
        this->name = name;
        this->carModel = carModel;
        this->carNumber = carNumber;
        this->currentCityID = cityId;
        this->isavailable = true;

    }
    
};

class mapGraph {
    vector<city> cities;
    vector<Edge> roads;
    vector<Driver> drivers;
public:
    void initialize()
    {
        cities.push_back(city(0, 150, 150, L"City A"));
        cities.push_back(city(1, 400, 100, L"City B"));
        cities.push_back(city(2, 650, 250, L"City C"));
        cities.push_back(city(3, 300, 380, L"City D"));
        cities.push_back(city(4, 550, 450, L"Main Station"));

        roads = { {0, 1}, {1, 2}, {0, 3}, {3, 2}, {3, 4}, {2, 4} };

        drivers.push_back(Driver(101, L"Rahul Sharma", L"Maruti Swift", L"DL-04-AB-1234", 0));
        drivers.push_back(Driver(102, L"Amit Verma", L"Hyundai i20", L"UP-16-XY-5678", 1));
        drivers.push_back(Driver(103, L"Priya Singh", L"Honda City", L"HR-26-ZZ-9999", 2));
    }

    const vector<city>& GetCities() const { return cities; }
    const vector<Driver>& GetDriverDetails() const { return drivers; }
    const vector<Edge>& GetRoads() const { return roads; }

    int findClickedCity(int mouseX, int mouseY, int radius = 15)const {
        for (const auto& city : cities)
        {
            int dx = mouseX - city.x;
            int dy = mouseY - city.y;
            if ((dx * dx + dy * dy) <= radius * radius)
            {
                return city.id;
            }
        }
        return -1;
    }

    double Distance(int a, int b) const {
        double dx = cities[a].x - cities[b].x;
        double dy = cities[a].y - cities[b].y;
        return sqrt(dx * dx + dy * dy);
    }

    vector<int> CalculateShortestPath(int startId, int endId) const {
        const double INF = numeric_limits<double>::infinity();
        int n = cities.size();
        vector<double> dist(n, INF);
        vector<bool> visited(n, false);
        vector<double>parent(n, -1);
        dist[startId] = 0;

       // priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
       // pq.push({ 0.0, startId });
        bool changed = true;
        while (changed) {               // jab tak kuch badalta rahe
            changed = false;
            for (auto road : roads) {   // har road dekho
                int a = road.a, b = road.b;
                double w = Distance(a, b);   // road ki lambai

                if (dist[a] + w < dist[b]) {   // a se b jaana sasta?
                    dist[b] = dist[a] + w;
                    parent[b] = a;
                    changed = true;
                }
                if (dist[b] + w < dist[a]) {   // b se a jaana sasta?
                    dist[a] = dist[b] + w;
                    parent[a] = b;
                    changed = true;
                }
            }
        }

        vector<int> path;
        if (dist[endId] == 1e18) return path;          // raasta hi nahi
        for (int c = endId; c != -1; c = parent[c]) path.push_back(c);
        reverse(path.begin(), path.end());
        return path;
    }

    Driver* FindNearestDriver(int pickupCityID)
    {
        Driver* bestDriver = NULL;
        double minDis = 1e9;
        for (auto& driver : drivers)
        {
            if (!driver.isavailable) continue;
            vector<int> path = CalculateShortestPath(driver.currentCityID, pickupCityID);

            double totalDist = 0;
            for (size_t i = 0;i < path.size() - 1;++i)
            {
                totalDist += sqrt(pow(cities[path[i + 1]].x - cities[path[i]].x, 2) +
                   pow( cities[path[i + 1]].y - cities[path[i]].y, 2));
           
            }
            if (totalDist < minDis)
            {
                minDis = totalDist;
                bestDriver = &driver;
            }


        }
        return bestDriver;
    }

};

mapGraph mg;
int pickupId = -1;
int dropoffId = -1;
vector<int> activeRoute;
wstring appStatus = L"status: click a city to select pick up location";
wstring assignedDriverInfo = L"assigned driver : none";









int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    mg.initialize();
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_UBERAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UBERAPP));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UBERAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_UBERAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, 900, 750, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawMap(hWnd, hdc);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_LBUTTONDOWN:
    {
        int mouseX = LOWORD(lParam);
        int mouseY = HIWORD(lParam);
        int clickedCityId = mg.findClickedCity(mouseX, mouseY);

        if (clickedCityId != -1)
        {
            const auto& cities = mg.GetCities();
            wstring clickedName = cities[clickedCityId].name;
            if (pickupId == -1)
            {
                pickupId = clickedCityId;
                activeRoute.clear();
                assignedDriverInfo = L"Assigned Driver : None";
                appStatus = L"Pickup set to " + clickedName + L" Now set to destination";
            
            }
            else if (dropoffId == -1 && clickedCityId != pickupId)
            {
                dropoffId = clickedCityId;
                activeRoute = mg.CalculateShortestPath(pickupId, dropoffId);
                Driver* assignedDriver = mg.FindNearestDriver(pickupId);

                if (assignedDriver) {
                    assignedDriverInfo = L"Driver : " + assignedDriver->name + L" car model is " +
                        assignedDriver->carModel + L"number plate is " + assignedDriver->carNumber;
                    appStatus= L"Ride Booked! Driver is on the way";
                   // assignedDriver->isavailable = false;
                }
                else
                {
                 //   assignedDriverInfo = L"Driver : " + assignedDriver->name + L" car model is " +
                   //     assignedDriver->carModel + L"number plate is " + assignedDriver->carNumber;

                    assignedDriverInfo = L"No drivers available nearby.";
                    appStatus = L"Ride Booked, but no drivers found.";
                }
            }
            else
            {
                pickupId = clickedCityId;
                dropoffId = -1;
                activeRoute.clear();
                assignedDriverInfo = L"Assigned Driver: None";
                appStatus = L"New Pickup set to " + clickedName + L". Click Destination.";

            }
            InvalidateRect(hWnd, NULL, TRUE);

        }

    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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


void DrawMap(HWND hwnd, HDC hdc)
{
    HPEN hRoadPen = CreatePen(PS_SOLID, 3, RGB(180, 180, 180));
    HPEN hPathPen = CreatePen(PS_SOLID, 6, RGB(0, 180, 0));
    HBRUSH hCityBrush = CreateSolidBrush(RGB(0, 120, 215));
    HBRUSH hPickupBrush = CreateSolidBrush(RGB(0, 200, 0));
    HBRUSH hDropBrush = CreateSolidBrush(RGB(220, 20, 60));

    HGDIOBJ holdPen = SelectObject(hdc, hRoadPen);
    HGDIOBJ holdBrush = SelectObject(hdc, hCityBrush);

    const auto& cities = mg.GetCities();
    const auto& roads = mg.GetRoads();
    const auto& drivers = mg.GetDriverDetails();

    // Draw Roads
    for (const auto& road : roads) {
        MoveToEx(hdc, cities[road.a].x, cities[road.a].y, NULL);
        LineTo(hdc, cities[road.b].x, cities[road.b].y);
    }

    // Draw Shortest Path Route
    SelectObject(hdc, hPathPen);
    if (activeRoute.size() > 1) {
        for (size_t i = 0; i < activeRoute.size() - 1; ++i) {
            MoveToEx(hdc, cities[activeRoute[i]].x, cities[activeRoute[i]].y, NULL);
            LineTo(hdc, cities[activeRoute[i + 1]].x, cities[activeRoute[i + 1]].y);
        }
    }

    int radius = 12;
    SetTextColor(hdc, RGB(20, 20, 20));
    SetBkMode(hdc, TRANSPARENT);

    // Draw Cities
    for (const auto& city : cities) {
        if (city.id == pickupId) SelectObject(hdc, hPickupBrush);
        else if (city.id == dropoffId) SelectObject(hdc, hDropBrush);
        else SelectObject(hdc, hCityBrush);

        SelectObject(hdc, hRoadPen);
        Ellipse(hdc, city.x - radius, city.y - radius, city.x + radius, city.y + radius);
        TextOut(hdc, city.x + 18, city.y - 10, city.name.c_str(), (int)city.name.length());
    }

    // Draw Driver Icons on Map (Orange small circles at driver locations)
    HBRUSH hDriverBrush = CreateSolidBrush(RGB(255, 140, 0));
    SelectObject(hdc, hDriverBrush);
    for (const auto& driver : drivers) {
        const auto& dCity = cities[driver.currentCityID];
        Ellipse(hdc, dCity.x - 6, dCity.y - 6, dCity.x + 6, dCity.y + 6);
    }
    DeleteObject(hDriverBrush);

    // Status Bar & Driver Info Display Boxes at bottom
    

    RECT rectStatus = { 20, 530, 850, 570 };
    SetBkColor(hdc, RGB(240, 240, 240));
    ExtTextOut(hdc, 30, 540, ETO_OPAQUE, &rectStatus, appStatus.c_str(), (UINT)appStatus.length(), NULL);

    RECT rectDriver = { 20, 580, 850, 620 };
    ExtTextOut(hdc, 30, 590, ETO_OPAQUE, &rectDriver, assignedDriverInfo.c_str(), (UINT)assignedDriverInfo.length(), NULL);
    // Cleanup
    SelectObject(hdc, holdPen);
    SelectObject(hdc, holdBrush);
    DeleteObject(hRoadPen);
    DeleteObject(hPathPen);
    DeleteObject(hCityBrush);
    DeleteObject(hPickupBrush);
    DeleteObject(hDropBrush);
}