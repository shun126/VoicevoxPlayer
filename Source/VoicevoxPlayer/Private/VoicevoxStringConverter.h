#pragma once
#include <memory>

namespace voicevox
{
	namespace windows
	{
		/*
		ワイド文字をUTF-8文字に変換するクラス
		*/
		class ToUTF8 final
		{
		private:
			ToUTF8() = delete;
			ToUTF8(const ToUTF8&) = delete;
			ToUTF8& operator = (const ToUTF8&) = delete;

		public:
			/*
			コンストラクタ
			ワイド文字をUTF-8文字に変換します
			\param[in]	text	ワイド文字へのポインタ
			*/
			explicit ToUTF8(const wchar_t* text);

			/*
			UTF-8文字を取得します
			\return		UTF-8文字列へのポインタ
			*/
			const char* Get() const;

			/*
			UTF-8文字を取得します
			\return		UTF-8文字列へのポインタ
			*/
			operator const char* () const;

		private:
			std::unique_ptr<char[]> mText;
		};

		inline const char* ToUTF8::Get() const
		{
			return mText != nullptr ? mText.get() : "";
		}

		inline ToUTF8::operator const char* () const
		{
			return mText != nullptr ? mText.get() : "";
		}
	}
}
