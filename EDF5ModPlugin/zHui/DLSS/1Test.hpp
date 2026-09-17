#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <unordered_map>

struct ThreadInfo {
    DWORD threadId;
    DWORD_PTR mask;
};

std::unordered_map<DWORD, DWORD_PTR> g_savedThreads; // 线程ID -> 原始掩码
DWORD_PTR g_originalProcessMask = 0;
bool g_affinitySaved = false;

// 加载前：保存所有线程掩码 + 进程掩码
void SaveThreadAffinities() {
    g_savedThreads.clear();
    g_affinitySaved = true;

    // 保存进程掩码
    DWORD_PTR systemMask = 0;
    GetProcessAffinityMask(GetCurrentProcess(), &g_originalProcessMask, &systemMask);

    // 遍历所有线程，保存掩码
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    THREADENTRY32 te = { sizeof(te) };
    DWORD pid = GetCurrentProcessId();

    if (Thread32First(hSnapshot, &te)) {
        do {
            if (te.th32OwnerProcessID != pid) continue;

            HANDLE hThread = OpenThread(
                THREAD_QUERY_LIMITED_INFORMATION, FALSE, te.th32ThreadID);
            if (hThread) {
                GROUP_AFFINITY ga = {};
                if (GetThreadGroupAffinity(hThread, &ga)) {
                    g_savedThreads[te.th32ThreadID] = ga.Mask;
                }
                CloseHandle(hThread);
            }
        } while (Thread32Next(hSnapshot, &te));
    }
    CloseHandle(hSnapshot);
}

// 加载后：恢复旧线程掩码，新线程设为进程掩码
void RestoreThreadAffinities() {
    if (!g_affinitySaved) return;

    // 先恢复进程掩码
    SetProcessAffinityMask(GetCurrentProcess(), g_originalProcessMask);

    // 遍历当前所有线程
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    THREADENTRY32 te = { sizeof(te) };
    DWORD pid = GetCurrentProcessId();

    if (Thread32First(hSnapshot, &te)) {
        do {
            if (te.th32OwnerProcessID != pid) continue;

            HANDLE hThread = OpenThread(
                THREAD_SET_LIMITED_INFORMATION | THREAD_QUERY_LIMITED_INFORMATION,
                FALSE, te.th32ThreadID);
            if (hThread) {
                auto it = g_savedThreads.find(te.th32ThreadID);
                /*
                if (it != g_savedThreads.end()) {
                    SetThreadAffinityMask(hThread, it->second);
                } else {
                    SetThreadAffinityMask(hThread, g_originalProcessMask);
                }*/
                SetThreadAffinityMask(hThread, g_originalProcessMask);
                CloseHandle(hThread);
            }
        } while (Thread32Next(hSnapshot, &te));
    }
    CloseHandle(hSnapshot);

    g_savedThreads.clear();
    g_affinitySaved = false;
}

// 在 EDF5.exe+EB0B0 处调用
void OnLoadStart() {
    SaveThreadAffinities();
    SetProcessAffinityMask(GetCurrentProcess(), 0b100); // 单核
}

// 在加载完成后调用
void OnLoadEnd() {
    RestoreThreadAffinities();
}
