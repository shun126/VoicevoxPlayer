# VoicevoxPlayer
Voicevoxを使ったテキスト読み上げを行うUnrealEngineのプラグインです。

[![license](https://img.shields.io/github/license/shun126/VoicevoxPlayer)](https://github.com/shun126/VoicevoxPlayer/blob/main/LICENSE)
[![release](https://img.shields.io/github/v/release/shun126/VoicevoxPlayer)](https://github.com/shun126/VoicevoxPlayer/releases)
[![stars](https://img.shields.io/github/stars/shun126/VoicevoxPlayer?style=social)](https://github.com/shun126/VoicevoxPlayer/stargazers)

# インストール方法
## Voicevoxの準備
* [Release](https://github.com/shun126/VoicevoxPlayer/releases)からVoicevoxPlayer{バージョン名}.zip」をダウンロードし、展開します。
* プロジェクトの`Plugins`フォルダに`VoicevoxPlayer`フォルダをコピーして下さい。
* [Releases](https://github.com/VOICEVOX/voicevox_core/releases/latest)から「voicevox_core-windows-x64-cpu-{バージョン名}.zip」をダウンロードし、展開します。[ダウンローダー](https://github.com/VOICEVOX/voicevox_core/blob/main/docs/downloads/download.md)を使うと便利です。  
* Open JTalk辞書フォルダを配置します。
  * ダウンローダーを使用していない場合は、http://open-jtalk.sourceforge.net/ を開き、Dictionary for Open JTalk 欄の Binary Package (UTF-8)をクリックして「open_jtalk_dic_utf_8-1.11.tar.gz」をダウンロードします。  
* 展開してできたファイル・フォルダをそれぞれ`Plugins\VoicevoxPlayer\Source\VoicevoxPlayer\ThirdParty`フォルダにコピーします。
* 編集メニュー内のプラグインを選択して下さい。
* `VoicevoxPlayer`プラグインを有効化して下さい。

最終的に、以下のようなフォルダ構成になっているはずです。
- `Plugins\VoicevoxPlayer\Source\VoicevoxPlayer\ThirdParty\voicevox_core`
  - `model` フォルダ
  - `open_jtalk_dic_utf_8-1.11` フォルダ
  - `onnxruntime.dll`
  - `onnxruntime_providers_shared.dll`
  - `README.txt`
  - `VERSION`
  - `voicevox_core.dll`
  - `voicevox_core.h`
  - `voicevox_core.lib`

## RuntimeAudioImporterの準備

必要なプラグイン[RuntimeAudioImporter](https://github.com/gtreshchev/RuntimeAudioImporter)をインストールします。

* [Release](https://github.com/gtreshchev/RuntimeAudioImporter/releases)から「RuntimeAudioImporter{バージョン名}.zip」をダウンロードし、展開します。
* プロジェクトの`Plugins`フォルダに`RuntimeAudioImporter`フォルダをコピーして下さい。

# 🔧 必要な環境
* [Unreal Engine 5.1.1 or Unreal Engine 4.27.2](https://www.unrealengine.com/)
* [Visual Studio 2022](https://visualstudio.microsoft.com/)

# 📜 ライセンス
* MIT

# 謝辞

* VoicevoxとHiroshibaさんには、テキスト読み上げを身近にしていただいたことに感謝します。
* Runtime Audio ImporterとGeorgy Treshchevさんには、簡単に音声ファイルを再生できるようにしてくださったことに感謝します。

# 🔧 Requirements
* [Unreal Engine 4.27.2 ~ 5.1.1](https://www.unrealengine.com/)
* [Visual Studio 2022](https://visualstudio.microsoft.com/)

# 📜 License
* MIT

# 👾 Authors
* [Shun Moriya](https://twitter.com/moriya_zx25r)
* [Nonbiri](https://www.youtube.com/channel/UCkLXe57GpUyaOoj2ycREU1Q)
