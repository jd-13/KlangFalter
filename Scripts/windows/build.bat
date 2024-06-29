set SCRIPT_DIR=%~dp0

ECHO "=== Downloading JUCE ==="
set JUCE_VERSION=7.0.9
powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest https://github.com/juce-framework/JUCE/releases/download/%JUCE_VERSION%/juce-%JUCE_VERSION%-windows.zip -OutFile %SCRIPT_DIR%..\..\juce-%JUCE_VERSION%-windows.zip}"
powershell -Command "& {Expand-Archive -LiteralPath %SCRIPT_DIR%..\..\juce-%JUCE_VERSION%-windows.zip -DestinationPath %SCRIPT_DIR%..\..}"

ECHO "=== Generating project ==="
set JUCE_HOME=%SCRIPT_DIR%..\..\JUCE
set PROJECT_DIR=%SCRIPT_DIR%..\..
%JUCE_HOME%\Projucer --resave %PROJECT_DIR%\KlangFalter.jucer

ECHO "=== Starting KlangFalter build ==="
cd %PROJECT_DIR%\Builds\VisualStudio2019
msbuild -version
msbuild KlangFalter.sln /p:Configuration=Release /p:Platform=x64 || exit /b

ECHO "=== Make Release folder ==="
cd %SCRIPT_DIR%..\..\
mkdir -p Release\BodyAndSoul.vst3\Contents
cd Release\BodyAndSoul.vst3\Contents

mkdir x86_64-win
cd x86_64-win
copy %PROJECT_DIR%\Builds\VisualStudio2019\x64\Release\VST3\KlangFalter.vst3\Contents\x86_64-win\KlangFalter.vst3 || exit /b
ren KlangFalter.vst3 BodyAndSoul.vst3

cd ..
mkdir Resources
