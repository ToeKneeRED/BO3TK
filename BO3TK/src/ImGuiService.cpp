#include "ImGuiService.h"
#include "Hooks.h"
#include "Log.h"

#pragma comment(lib, "d3d11.lib")

WNDPROC ImGuiService::OriginalWndProc = nullptr;
ImGuiService* ImGuiService::s_instance = nullptr;

ImGuiService* ImGuiService::Init(const HWND& acWindow, ID3D11Device* apDevice, ID3D11DeviceContext* apContext)
{
    if (!s_instance)
    {
        OriginalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(g_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProcHook)));
        s_instance = new ImGuiService(acWindow, apDevice, apContext);
    }
    return s_instance;
}

ImGuiService* ImGuiService::Get()
{
    if (!s_instance)
    {
        Log::Get()->Error("ImGuiService not initialized. Call Get(window, device, context) first");
    }

    return s_instance;
}

LRESULT CALLBACK ImGuiService::WndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    const ImGuiIO& cIo = ImGui::GetIO();

    if (cIo.WantCaptureKeyboard)
    {
        switch (uMsg)
        {
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP: return 0;
        default: break;
        }
    }

    if (cIo.WantCaptureMouse)
    {
        switch (uMsg)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_MOUSEWHEEL:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP: return 0;
        default: break;
        }
    }

    return CallWindowProcA(OriginalWndProc, hWnd, uMsg, wParam, lParam);
}

void ImGuiService::OnDraw() noexcept
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("BO3TK");

    const ImVec4& cColor = *BO3E::IsInGame ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImGui::Text("IsInGame: ");
    ImGui::SameLine();
    ImGui::TextColored(cColor, "%s", *BO3E::IsInGame ? "true" : "false");

    if (*BO3E::IsInGame)
    {
        ImGui::Text("Local Player Name:");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.5f, 1.0f), "%s", BO3E::LocalPlayer.Name);

        ImGui::Text("Kills: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%hu", *BO3E::LocalPlayer.Kills);

        ImGui::Text("Points: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%u", *BO3E::LocalPlayer.Points);

        ImGui::Text("Ping: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%u", *BO3E::LocalPlayer.Ping);

        ImGui::Text("Zombies Alive: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%u", *BO3E::ZombiesAlive);
    }

    ImGui::PushItemWidth(100);
    ImGui::InputText("RVA", m_inputString, 256, ImGuiInputTextFlags_CharsNoBlank);

    const int cBase = (m_inputString[0] == '0' && (m_inputString[1] == 'x' || m_inputString[1] == 'X')) ? 16 : 10;

    m_inputAddress = min(std::strtoull(m_inputString, nullptr, cBase), (uintptr_t)Exe::End);
    m_watchAddress = ADDRESS(m_inputAddress);
    ImGui::PopItemWidth();

    if (m_watchAddress)
    {
        ImGui::Text("%p: ", reinterpret_cast<void*>(m_watchAddress));
        ImGui::Text("  uint64\t%u", *reinterpret_cast<uint64_t*>(ADDRESS(m_inputAddress)));
        ImGui::Text("  float\t%f", *reinterpret_cast<float*>(ADDRESS(m_inputAddress)));
        ImGui::Text("  const char\t%s", reinterpret_cast<const char*>(ADDRESS(m_inputAddress)));
        ImGui::Text("  bool\t\t%s", *reinterpret_cast<bool*>(ADDRESS(m_inputAddress)) ? "true" : "false");
    }

    ImGui::End();
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiService::Test()
{
    Log* log = Log::Get();

    WNDCLASSEX windowClass;
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = DefWindowProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandle(nullptr);
    windowClass.hIcon = nullptr;
    windowClass.hCursor = nullptr;
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = "ImGuiService";
    windowClass.hIconSm = nullptr;

    RegisterClassEx(&windowClass);

    const HWND cWindow = CreateWindow(windowClass.lpszClassName, "Toe Knee Test", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

    if (const auto cD3d11 = GetModuleHandleA("d3d11.dll"); !cD3d11)
    {
        log->Error("Failed to find d3d11");
        return false;
    }
    else
    {
        if (void* d3D11CreateDeviceAndSwapChain; (d3D11CreateDeviceAndSwapChain = GetProcAddress(cD3d11, "D3D11CreateDeviceAndSwapChain")) == nullptr)
        {
            log->Error("Failed to find D3D11CreateDeviceAndSwapChain");

            DestroyWindow(cWindow);
            UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

            return false;
        }
        else
        {
            D3D_FEATURE_LEVEL featureLevel;
            constexpr D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0};

            DXGI_RATIONAL refreshRate;
            refreshRate.Numerator = 60;
            refreshRate.Denominator = 1;

            DXGI_MODE_DESC bufferDesc;
            bufferDesc.Width = 100;
            bufferDesc.Height = 100;
            bufferDesc.RefreshRate = refreshRate;
            bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

            DXGI_SAMPLE_DESC sampleDesc;
            sampleDesc.Count = 1;
            sampleDesc.Quality = 0;

            DXGI_SWAP_CHAIN_DESC swapChainDesc;
            swapChainDesc.BufferDesc = bufferDesc;
            swapChainDesc.SampleDesc = sampleDesc;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = 1;
            swapChainDesc.OutputWindow = cWindow;
            swapChainDesc.Windowed = 1;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
            swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

            IDXGISwapChain* swapChain;
            ID3D11Device* device;
            ID3D11DeviceContext* context;

            if (static_cast<long(__stdcall*)(
                    IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**,
                    ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**)>(d3D11CreateDeviceAndSwapChain)(
                    nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel,
                    &context) < 0)
            {
                log->Error("D3D11CreateDeviceAndSwapChain call failed...");
                DestroyWindow(cWindow);
                UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
                return false;
            }

            Hooks::MethodsTable = static_cast<uint64_t*>(calloc(205, sizeof(uint64_t)));
            memcpy(Hooks::MethodsTable, *reinterpret_cast<uint64_t**>(swapChain), 18 * sizeof(uint64_t));
            memcpy(Hooks::MethodsTable + 18, *reinterpret_cast<uint64_t**>(device), 43 * sizeof(uint64_t));
            memcpy(Hooks::MethodsTable + 18 + 43, *reinterpret_cast<uint64_t**>(context), 144 * sizeof(uint64_t));

            swapChain->Release();
            swapChain = nullptr;

            device->Release();
            device = nullptr;

            context->Release();
            context = nullptr;

            DestroyWindow(cWindow);
            UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        }
    }

    return true;
}