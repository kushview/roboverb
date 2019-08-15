msbuild /t:Rebuild /p:Configuration=Release /p:Platform=Win32 tools\jucer\Builds\VisualStudio2017\Roboverb.sln || exit /b
msbuild /t:Rebuild /p:Configuration=Release /p:Platform=x64 tools\jucer\Builds\VisualStudio2017\Roboverb.sln || exit /b
