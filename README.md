# VoicevoxPlayer
単純な音声合成を行うUnrealEngineのプラグインです。
Voicevox Player for Unreal Engine plug-in

# インストール方法
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
