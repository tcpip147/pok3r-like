#include <windows.h>

HHOOK hHook = NULL;

const int VK_Q = 0x51;
const int VK_W = 0x57;
const int VK_E = 0x45;
const int VK_A = 0x41;
const int VK_S = 0x53;
const int VK_D = 0x44;
const int VK_U = 0x55;
const int VK_I = 0x49;
const int VK_O = 0x4F;
const int VK_J = 0x4A;
const int VK_K = 0x4B;
const int VK_L = 0x4C;

bool is_down_caps = false;
bool is_down_ctrl = false;

static int send_key(WPARAM wParam, int key)
{
    switch (wParam) {
    case WM_KEYDOWN:
    {
        keybd_event(key, 0x1D, KEYEVENTF_EXTENDEDKEY | 0, 0);
        break;
    }
    case WM_KEYUP:
    {
        keybd_event(key, 0x1D, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        break;
    }
    default:
        break;
    }
    return 1;
}

static void set_flag(WPARAM wParam, bool* flag)
{
    switch (wParam) {
    case WM_KEYDOWN:
    {
        *flag = true;
        break;
    }
    case WM_KEYUP:
    {
        *flag = false;
        break;
    }
    default:
        break;
    }
}

static LRESULT CALLBACK LowLevelKeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
        if (is_down_caps)
        {
            if (p->vkCode == VK_Q) return send_key(wParam, VK_INSERT);
            else if (p->vkCode == VK_W) return send_key(wParam, VK_HOME);
            else if (p->vkCode == VK_E) return send_key(wParam, VK_PRIOR);
            else if (p->vkCode == VK_A) return send_key(wParam, VK_DELETE);
            else if (p->vkCode == VK_S) return send_key(wParam, VK_END);
            else if (p->vkCode == VK_D) return send_key(wParam, VK_NEXT);
            else if (p->vkCode == VK_U) return send_key(wParam, VK_BACK);
            else if (p->vkCode == VK_I) return send_key(wParam, VK_UP);
            else if (p->vkCode == VK_O) return send_key(wParam, VK_RETURN);
            else if (p->vkCode == VK_J) return send_key(wParam, VK_LEFT);
            else if (p->vkCode == VK_K) return send_key(wParam, VK_DOWN);
            else if (p->vkCode == VK_L) return send_key(wParam, VK_RIGHT);
        }

        if (p->vkCode == VK_CAPITAL)
        {
            set_flag(wParam, &is_down_caps);
            if (!is_down_ctrl)
            {
                return 1;
            }
        }
        else if (p->vkCode == VK_LCONTROL)
        {
            set_flag(wParam, &is_down_ctrl);
        }
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, hInstance, NULL);
    if (hHook == NULL)
    {
        return 1;
    }

    MSG messages;

    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return 0;
}