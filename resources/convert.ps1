# ファイル拡張子の取得
$files = Get-Item *.jpg,*.png

# for文を利用してファイル内のテクスチャを一括変換
foreach($f in $files){
    # TextureConverterにコマンド引数を渡しつつ実行する
    Start-Process -FilePath TextureConverter.exe -ArgumentList $f, "-ml 3" -Wait
}
Pause