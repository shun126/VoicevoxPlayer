#pragma once
#include <memory>

namespace voicevox
{
	namespace windows
	{
		/*
		���C�h������UTF-8�����ɕϊ�����N���X
		*/
		class ToUTF8 final
		{
		private:
			ToUTF8() = delete;
			ToUTF8(const ToUTF8&) = delete;
			ToUTF8& operator = (const ToUTF8&) = delete;

		public:
			/*
			�R���X�g���N�^
			���C�h������UTF-8�����ɕϊ����܂�
			\param[in]	text	���C�h�����ւ̃|�C���^
			*/
			explicit ToUTF8(const wchar_t* text);

			/*
			UTF-8�������擾���܂�
			\return		UTF-8������ւ̃|�C���^
			*/
			const char* Get() const;

			/*
			UTF-8�������擾���܂�
			\return		UTF-8������ւ̃|�C���^
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
