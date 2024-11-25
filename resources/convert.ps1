$files = Get-Item *.jpg,*.png

foreach($f in $files){
    Start-Process -FilePath TextureConverter.exe -ArgumentList $f -Wait
}
Pause