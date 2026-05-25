#pragma once

#include <Windows.h>
#include <shobjidl.h>

#include <string>

namespace Stylus::Utils {

    inline std::string OpenFileDialog()
    {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        bool shouldUninit = SUCCEEDED(hr);

        IFileOpenDialog* dialog = nullptr;

        hr = CoCreateInstance(
            CLSID_FileOpenDialog,
            nullptr,
            CLSCTX_ALL,
            IID_PPV_ARGS(&dialog)
        );

        if (FAILED(hr) || !dialog)
        {
            if (shouldUninit)
            {
                CoUninitialize();
            }

            return {};
        }

        hr = dialog->Show(nullptr);

        if (FAILED(hr))
        {
            dialog->Release();

            if (shouldUninit)
            {
                CoUninitialize();
            }

            return {};
        }

        IShellItem* item = nullptr;
        hr = dialog->GetResult(&item);

        if (FAILED(hr) || !item)
        {
            dialog->Release();

            if (shouldUninit)
            {
                CoUninitialize();
            }

            return {};
        }

        PWSTR filePath = nullptr;
        hr = item->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

        if (FAILED(hr) || !filePath)
        {
            item->Release();
            dialog->Release();

            if (shouldUninit)
            {
                CoUninitialize();
            }

            return {};
        }

        std::wstring ws(filePath);

        int sizeNeeded = WideCharToMultiByte(
            CP_UTF8,
            0,
            ws.c_str(),
            -1,
            nullptr,
            0,
            nullptr,
            nullptr
        );

        std::string path(sizeNeeded - 1, '\0');

        WideCharToMultiByte(
            CP_UTF8,
            0,
            ws.c_str(),
            -1,
            path.data(),
            sizeNeeded,
            nullptr,
            nullptr
        );

        CoTaskMemFree(filePath);

        item->Release();
        dialog->Release();

        if (shouldUninit)
        {
            CoUninitialize();
        }

        return path;
    }

    inline std::string SaveFileDialog(const wchar_t* filter = L"PNG Files (*.png)\0*.png\0")
    {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        bool shouldUninit = SUCCEEDED(hr);

        IFileSaveDialog* dialog = nullptr;

        hr = CoCreateInstance(
            CLSID_FileSaveDialog,
            nullptr,
            CLSCTX_ALL,
            IID_PPV_ARGS(&dialog)
        );

        if (FAILED(hr) || !dialog)
        {
            if (shouldUninit)
            {
                CoUninitialize();
            }

            return {};
        }

        COMDLG_FILTERSPEC fileTypes[] =
        {
            { L"PNG Files", L"*.png" },
            { L"JPEG Files", L"*.jpg;*.jpeg" },
            { L"BMP Files", L"*.bmp" }
        };

        dialog->SetFileTypes(_countof(fileTypes), fileTypes);
        dialog->SetDefaultExtension(L"png");

        hr = dialog->Show(nullptr);

        if (FAILED(hr))
        {
            dialog->Release();

            if (shouldUninit)
            {
                CoUninitialize();
            }

            return {};
        }

        IShellItem* item = nullptr;
        hr = dialog->GetResult(&item);

        if (FAILED(hr) || !item)
        {
            dialog->Release();

            if (shouldUninit)
            {
                CoUninitialize();
            }

            return {};
        }

        PWSTR filePath = nullptr;
        hr = item->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

        if (FAILED(hr) || !filePath)
        {
            item->Release();
            dialog->Release();

            if (shouldUninit)
            {
                CoUninitialize();
            }

            return {};
        }

        std::wstring ws(filePath);

        int sizeNeeded = WideCharToMultiByte(
            CP_UTF8,
            0,
            ws.c_str(),
            -1,
            nullptr,
            0,
            nullptr,
            nullptr
        );

        std::string path(sizeNeeded - 1, '\0');

        WideCharToMultiByte(
            CP_UTF8,
            0,
            ws.c_str(),
            -1,
            path.data(),
            sizeNeeded,
            nullptr,
            nullptr
        );

        CoTaskMemFree(filePath);

        item->Release();
        dialog->Release();

        if (shouldUninit)
        {
            CoUninitialize();
        }

        return path;
    }

}
