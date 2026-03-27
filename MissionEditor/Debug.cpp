#include "stdafx.h"
#include <Dbghelp.h>
#include <filesystem>
#include <future>
#include <afx.h>
#include "Debug.h"
#include "variables.h"
#include "Version.h"

namespace fs = std::filesystem;

class CDumpProgressDlg : public CDialog
{
public:
    CDumpProgressDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_DUMP_PROGRESS };

    void SetMessage(LPCTSTR lpszMsg);

protected:
    virtual BOOL OnInitDialog() override;
    CProgressCtrl m_Progress;
    CStatic       m_StaticMsg;
    CString       m_strInitialMsg;

    DECLARE_MESSAGE_MAP()
};

static std::tuple<const char*, const char*, CString> translateException(const PEXCEPTION_RECORD pRecord)
{
    switch (pRecord->ExceptionCode) {
    default:
        return { "Unknown exception", "", {} };
    case EXCEPTION_ACCESS_VIOLATION:
    {
        CString additional;
        auto const pPrefix = pRecord->ExceptionInformation[0] ?
            "Write access to 0x" : "Read access from 0x";
        additional.Format("\nAdditional information: %s%X", pPrefix, pRecord->ExceptionInformation[1]);
        return {
            "EXCEPTION_ACCESS_VIOLATION",
            "Thread did not have read or write access to the virtual address.",
            additional,
        };
    }
    break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        return {
            "EXCEPTION_ARRAY_BOUNDS_EXCEEDED",
            "Thread tried to access an array out of bounds.",
            {}
        };
    case EXCEPTION_BREAKPOINT:
        return {
            "EXCEPTION_BREAKPOINT",
            "Encountered breakpoint.",
            {}
        };
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        return {
            "EXCEPTION_DATATYPE_MISALIGNMENT",
            "Access to memory was misaligned for the given datatype.",
            {}
        };
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        return {
            "EXCEPTION_FLT_DENORMAL_OPERAND",
            "Denormal operand in floating point operation.",
            {}
        };
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        return {
            "EXCEPTION_FLT_DIVIDE_BY_ZERO",
            "Thread divided by zero in a floating point operation.",
            {}
        };
    case EXCEPTION_FLT_INEXACT_RESULT:
        return {
            "EXCEPTION_FLT_INEXACT_RESULT",
            "Floating point operation result not representable with exact decimal fraction.",
            {}
        };
    case EXCEPTION_FLT_INVALID_OPERATION:
        return {
            "EXCEPTION_FLT_INVALID_OPERATION",
            "Invalid floating point operation.",
            {}
        };
    case EXCEPTION_FLT_OVERFLOW:
        return {
            "EXCEPTION_FLT_OVERFLOW",
            "Floating point overflow error.",
            {}
        };
    case EXCEPTION_FLT_STACK_CHECK:
        return {
            "EXCEPTION_FLT_STACK_CHECK",
            "Floating point operation caused stack overflow or underflow.",
            {}
        };
    case EXCEPTION_FLT_UNDERFLOW:
        return {
            "EXCEPTION_FLT_UNDERFLOW",
            "Floating point underflow error.",
            {}
        };
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        return {
            "EXCEPTION_ILLEGAL_INSTRUCTION",
            "Thread executed illegal instruction.",
            {}
        };
    case EXCEPTION_IN_PAGE_ERROR:
        return {
            "EXCEPTION_IN_PAGE_ERROR",
            "Thread tried to access a page that could not be retrieved by the system.",
            {}
        };
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        return {
            "EXCEPTION_INT_DIVIDE_BY_ZERO",
            "Thread divided by zero in an integer operation.",
            {}
        };
    case EXCEPTION_INT_OVERFLOW:
        return {
            "EXCEPTION_INT_OVERFLOW",
            "Integer operation caused overflow.",
            {}
        };
    case EXCEPTION_INVALID_DISPOSITION:
        return {
            "EXCEPTION_INVALID_DISPOSITION",
            "Exception handler returned invalid disposition.",
            {}
        };
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        return {
            "EXCEPTION_NONCONTINUABLE_EXCEPTION",
            "Cannot continue execution after a noncontinuable exception.",
            {}
        };
    case EXCEPTION_PRIV_INSTRUCTION:
        return {
            "EXCEPTION_PRIV_INSTRUCTION",
            "Instruction not valid in the current machine mode.",
            {}
        };
    case EXCEPTION_SINGLE_STEP:
        return {
            "EXCEPTION_SINGLE_STEP",
            "Instruction step has been executed.",
            {}
        };
    case EXCEPTION_STACK_OVERFLOW:
        return {
            "EXCEPTION_STACK_OVERFLOW",
            "Stack overflow.",
            {}
        };

    }
}

static std::wstring fullDump(
    std::wstring destinationFolder,
    PMINIDUMP_EXCEPTION_INFORMATION const pException)
{
    std::wstring filename = std::move(destinationFolder);
    filename += L"\\extcrashdump.dmp";

    HANDLE dumpFile = CreateFileW(filename.c_str(), GENERIC_WRITE,
        0, nullptr, CREATE_ALWAYS, FILE_FLAG_RANDOM_ACCESS, nullptr);

#if 0
    MINIDUMP_TYPE type = static_cast<MINIDUMP_TYPE>(MiniDumpWithFullMemory);
#else
    MINIDUMP_TYPE type = (MINIDUMP_TYPE)(
        MiniDumpNormal |
        MiniDumpWithHandleData |
        MiniDumpWithThreadInfo |
        MiniDumpWithProcessThreadData |
        MiniDumpWithIndirectlyReferencedMemory |
        MiniDumpWithDataSegs |          // Global/Static variables
        MiniDumpIgnoreInaccessibleMemory // Make it stable
        );
#endif

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile, type, pException, nullptr, nullptr);
    CloseHandle(dumpFile);

    return filename;
}

std::wstring prepareCrashdumpDir()
{
    auto const now = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
    std::wstring subpath = std::format(L"snapshot-{:%Y%m%d-%H%M%S}", now);
    auto const crashdumpPath = (fs::path(u8AppDataPath) / "debug" / subpath).wstring();
    if (!fs::exists(crashdumpPath)) {
        fs::create_directories(crashdumpPath);
    }
    return crashdumpPath;
}

void popDumpFolder()
{
    auto const strRelativePath = _T("%LOCALAPPDATA%\\FinalRevenge\\debug");
    TCHAR szFullPath[MAX_PATH];

    DWORD dwRet = ExpandEnvironmentStringsA(strRelativePath, szFullPath, MAX_PATH);

    if (dwRet > 0 && dwRet <= MAX_PATH) {
        HINSTANCE hInst = ShellExecute(NULL, _T("open"), szFullPath, NULL, NULL, SW_SHOWNORMAL);

        if ((INT_PTR)hInst <= 32) {
            errstream << "could not open debug folder";
        }
    }
}

LONG __stdcall Debug::ExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
    errstream << "Exception occured. Current data:" << endl;
    errstream << "Last succeeded operation:" << last_succeeded_operation << endl;
    errstream << "Last succeeded library operation:" << FSunPackLib::last_succeeded_operation << endl;

    auto const [exceptionTypeStr, exceptionDesc, exceptionAdditionalInfo] = translateException(ExceptionInfo->ExceptionRecord);
    errstream << "Exception type:" << exceptionTypeStr << " description: " << exceptionDesc << endl;
    errstream << "Additional Info:" << exceptionAdditionalInfo << endl;

    auto errDlg = std::make_unique<CDumpProgressDlg>();
    errDlg->Create(CDumpProgressDlg::IDD);
    bool dumpComplete = false;
    auto const crashdumpPath = prepareCrashdumpDir();

    auto handle = std::async(std::launch::async, [ExceptionInfo, &errDlg, &dumpComplete, &crashdumpPath] {
        MINIDUMP_EXCEPTION_INFORMATION expParam;
        expParam.ThreadId = GetCurrentThreadId();
        expParam.ExceptionPointers = ExceptionInfo;
        expParam.ClientPointers = FALSE;

        fullDump(crashdumpPath, &expParam);
        dumpComplete = true;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        errDlg->PostMessage(WM_CLOSE);
    });

    const char* pFormatterStr = "INTERNAL APPLICATION ERROR\n\n" \
        "Version: " PRODUCT_VERSION_STRING
        "Application will now try to free memory, save the current map as \"fcrash_backup.map\" in the %s directory and quit.\n\n\n" \
        "Important: If this error has occured while loading graphics, it can very often be fixed by using another system color resolution (16, 24 or 32 bit)." \
        "\n\nThe following information is available, please note every line below:\n\n" \
        "Last succeeded operation: %d\n" \
        "\nLast library operation: %d\n" \
        "\nException data:\n%s\n" \
        "%s\n" \
        "\nAt address: %p\n"
        "\n%s";

    if (theApp.m_Options.LanguageName == "Chinese") {
        pFormatterStr = "地图编辑器程序错误\n\n" \
            "版本号: " PRODUCT_VERSION_STRING
            "本应用将尝试将地图保存至 %s 文件夹内的\"fcrash_backup.map\" 并且退出.\n\n\n" \
            "当你看到这个窗口的时候，请截图并反馈给开发者" \
            "\n\n下面为错误信息详情:\n\n" \
            "上一次成功的操作ID: %d\n" \
            "\n刚才执行中的操作ID: %d\n" \
            "\n异常报告:\n%s\n" \
            "%s\n" \
            "\n内存地址: %p\n"
            "\n%s";
    }

    CString exceptionReport;
    exceptionReport.Format(pFormatterStr,
        FA2_EDITOR_NAME,
        last_succeeded_operation,
        FSunPackLib::last_succeeded_operation,
        exceptionTypeStr,
        exceptionDesc,
        ExceptionInfo->ExceptionRecord->ExceptionAddress,
        exceptionAdditionalInfo
    );

    {
        errDlg->SetMessage(exceptionReport);
        errDlg->ShowWindow(SW_SHOW);
        theApp.MainWindow()->EnableWindow(FALSE);

        MSG msg;
        while (!dumpComplete
            && ::IsWindow(errDlg->GetSafeHwnd())
            && GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        handle.wait();

        errDlg->ShowWindow(SW_HIDE);
        errDlg.reset();
        popDumpFolder();
    }

    errstream << "Trying to save current map as emergency backup" << endl;
    errstream.flush();

    auto const dstLogFilePath = (fs::path(crashdumpPath) / FA2_LOG_FIL).string();
    CopyFileA(theApp.getLogFileName().c_str(), dstLogFilePath.c_str(), FALSE);

    std::string file = u8AppDataPath;
    file += "\\fcrash_backup.map";
    theApp.MainWindow()->SaveMap(file.c_str());

    theApp.m_loading->FreeAll();

    rules.Clear();
    ai.Clear();
    art.Clear();
    tiles_t.Clear();
    tiles_s.Clear();
    tiles_u.Clear();
    Map->GetIniFile().Clear();
    sound.Clear();
    tutorial.Clear();
    g_data.Clear();
    language.Clear();

    return EXCEPTION_EXECUTE_HANDLER;//EXCEPTION_CONTINUE_SEARCH;//EXCEPTION_EXECUTE_HANDLER;
}

CDumpProgressDlg::CDumpProgressDlg(CWnd* pParent) : CDialog(IDD_DUMP_PROGRESS, pParent) {}

BEGIN_MESSAGE_MAP(CDumpProgressDlg, CDialog)
END_MESSAGE_MAP()

BOOL CDumpProgressDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_StaticMsg.SubclassDlgItem(IDC_STATIC_MSG, this);
    if (!m_strInitialMsg.IsEmpty()) {
        m_StaticMsg.SetWindowText(m_strInitialMsg);
    }

    m_Progress.SubclassDlgItem(IDC_PROGRESS1, this);
    m_Progress.ModifyStyle(0, PBS_MARQUEE);
    m_Progress.SendMessage(PBM_SETMARQUEE, TRUE, 50);

    return TRUE;
}

void CDumpProgressDlg::SetMessage(LPCTSTR lpszMsg)
{
    if (::IsWindow(m_StaticMsg.GetSafeHwnd())) {
        m_StaticMsg.SetWindowText(lpszMsg);
    } else {
        m_strInitialMsg = lpszMsg;
    }
}