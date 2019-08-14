msbuild /t:Rebuild /p:Configuration=Release /p:Platform=Win32 data\jucer\Builds\VisualStudio2017\Roboverb.sln || exit /b
msbuild /t:Rebuild /p:Configuration=Release /p:Platform=x64 data\jucer\Builds\VisualStudio2017\Roboverb.sln || exit /b
