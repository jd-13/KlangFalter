set SCRIPT_DIR=%~dp0

ECHO "=== Downloading JUCE ==="
set JUCE_VERSION=8.0.4
powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest https://github.com/juce-framework/JUCE/releases/download/%JUCE_VERSION%/juce-%JUCE_VERSION%-windows.zip -OutFile %SCRIPT_DIR%..\..\juce-%JUCE_VERSION%-windows.zip}"
powershell -Command "& {Expand-Archive -LiteralPath %SCRIPT_DIR%..\..\juce-%JUCE_VERSION%-windows.zip -DestinationPath %SCRIPT_DIR%..\..}"

ECHO "=== Generating projects ==="
set JUCE_HOME=%SCRIPT_DIR%..\..\JUCE
set PROJECT_DIR=%SCRIPT_DIR%..\..
%JUCE_HOME%\Projucer --resave %PROJECT_DIR%\Projects\Intro\BodyandSoulIntro.jucer
%JUCE_HOME%\Projucer --resave %PROJECT_DIR%\Projects\Body\BodyandSoulBody.jucer
%JUCE_HOME%\Projucer --resave %PROJECT_DIR%\Projects\Soul\BodyandSoulSoul.jucer
%JUCE_HOME%\Projucer --resave %PROJECT_DIR%\Projects\FX\BodyandSoulFX.jucer

ECHO "=== Starting builds ==="
msbuild -version
cd %PROJECT_DIR%\Projects\Intro\Builds\VisualStudio2019
msbuild "Body and Soul Intro.sln" /p:Configuration=Release /p:Platform=x64 || exit /b
cd %PROJECT_DIR%\Projects\Body\Builds\VisualStudio2019
msbuild "Body and Soul Body.sln" /p:Configuration=Release /p:Platform=x64 || exit /b
cd %PROJECT_DIR%\Projects\Soul\Builds\VisualStudio2019
msbuild "Body and Soul Soul.sln" /p:Configuration=Release /p:Platform=x64 || exit /b
cd %PROJECT_DIR%\Projects\FX\Builds\VisualStudio2019
msbuild "Body and Soul FX.sln" /p:Configuration=Release /p:Platform=x64 || exit /b

ECHO "=== Making Release folder ==="
cd %SCRIPT_DIR%..\..\
mkdir -p Release\TSoM-BodyandSoulIntro.vst3\Contents\x86_64-win
mkdir -p Release\TSoM-BodyandSoulBody.vst3\Contents\x86_64-win
mkdir -p Release\TSoM-BodyandSoulSoul.vst3\Contents\x86_64-win
mkdir -p Release\TSoM-BodyandSoulFX.vst3\Contents\x86_64-win

cd Release\TSoM-BodyandSoulIntro.vst3\Contents\x86_64-win
copy %PROJECT_DIR%\Projects\Intro\Builds\VisualStudio2019\x64\Release\VST3\TSoM-BodyandSoulIntro.vst3\Contents\x86_64-win\TSoM-BodyandSoulIntro.vst3 || exit /b

cd Release\TSoM-BodyandSoulBody.vst3\Contents\x86_64-win
copy %PROJECT_DIR%\Projects\Body\Builds\VisualStudio2019\x64\Release\VST3\TSoM-BodyandSoulBody.vst3\Contents\x86_64-win\TSoM-BodyandSoulBody.vst3 || exit /b

cd Release\TSoM-BodyandSoulSoul.vst3\Contents\x86_64-win
copy %PROJECT_DIR%\Projects\Soul\Builds\VisualStudio2019\x64\Release\VST3\TSoM-BodyandSoulSoul.vst3\Contents\x86_64-win\TSoM-BodyandSoulSoul.vst3 || exit /b

cd Release\TSoM-BodyandSoulFX.vst3\Contents\x86_64-win
copy %PROJECT_DIR%\Projects\FX\Builds\VisualStudio2019\x64\Release\VST3\TSoM-BodyandSoulFX.vst3\Contents\x86_64-win\TSoM-BodyandSoulFX.vst3 || exit /b

@REM ECHO "=== Copying IRs ==="
@REM cd ..
@REM mkdir -p Resources\IRs
@REM robocopy %PROJECT_DIR%\IRs\ Resources\IRs\ /e /s
