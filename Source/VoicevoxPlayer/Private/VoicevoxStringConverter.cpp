/*
ワイド文字をUTF-8文字に変換するクラス
*/

#include "VoicevoxStringConverter.h"

#include <Windows/AllowWindowsPlatformTypes.h>
#include <windows.h>

namespace voicevox
{
	namespace windows
	{
		ToUTF8::ToUTF8(const wchar_t* text)
		{
			if (text)
			{
				const int size = WideCharToMultiByte(CP_UTF8, 0, text, -1, NULL, 0, NULL, NULL);
				mText = std::make_unique<char[]>(size + 1);
				if (mText != nullptr)
				{
					WideCharToMultiByte(CP_UTF8, 0, text, -1, mText.get(), size, NULL, NULL);
				}
			}
		}
	}
}

#include <Windows/HideWindowsPlatformTypes.h>
