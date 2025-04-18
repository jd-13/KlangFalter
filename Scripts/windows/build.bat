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
@REM %JUCE_HOME%\Projucer --resave %PROJECT_DIR%\Projects\FX\BodyandSoulFX.jucer

ECHO "=== Starting builds ==="
msbuild -version
cd %PROJECT_DIR%\Projects\Intro\Builds\VisualStudio2019
msbuild "Body and Soul Intro.sln" /p:Configuration=Release /p:Platform=x64 || exit /b
cd %PROJECT_DIR%\Projects\Body\Builds\VisualStudio2019
msbuild "Body and Soul Body.sln" /p:Configuration=Release /p:Platform=x64 || exit /b
cd %PROJECT_DIR%\Projects\Soul\Builds\VisualStudio2019
msbuild "Body and Soul Soul.sln" /p:Configuration=Release /p:Platform=x64 || exit /b
@REM cd %PROJECT_DIR%\Projects\FX\Builds\VisualStudio2019
@REM msbuild "Body and Soul FX.sln" /p:Configuration=Release /p:Platform=x64 || exit /b

ECHO "=== Making Release folder ==="
cd %PROJECT_DIR%
mkdir dist\BodyandSoulIntro\TSoM-BodyandSoulIntro.vst3\Contents\x86_64-win  || exit /b
mkdir dist\BodyandSoul\TSoM-BodyandSoulBody.vst3\Contents\x86_64-win || exit /b
mkdir dist\BodyandSoul\TSoM-BodyandSoulSoul.vst3\Contents\x86_64-win || exit /b
@REM mkdir dist\BodyandSoul\TSoM-BodyandSoulFX.vst3\Contents\x86_64-win || exit /b

cd dist\BodyandSoulIntro\TSoM-BodyandSoulIntro.vst3\Contents\x86_64-win
copy %PROJECT_DIR%\Projects\Intro\Builds\VisualStudio2019\x64\Release\VST3\TSoM-BodyandSoulIntro.vst3\Contents\x86_64-win\TSoM-BodyandSoulIntro.vst3 || exit /b
cd ..
mkdir Resources\IRs || exit /b
robocopy %PROJECT_DIR%\IRs\Intro Resources\IRs\ /e /s
cd Resources
copy %PROJECT_DIR%\Themes\Intro.json || exit /b
ren Intro.json theme.json
cd %PROJECT_DIR%

cd dist\BodyandSoul\TSoM-BodyandSoulBody.vst3\Contents\x86_64-win
copy %PROJECT_DIR%\Projects\Body\Builds\VisualStudio2019\x64\Release\VST3\TSoM-BodyandSoulBody.vst3\Contents\x86_64-win\TSoM-BodyandSoulBody.vst3 || exit /b
cd ..
mkdir Resources\IRs || exit /b
robocopy %PROJECT_DIR%\IRs\Body Resources\IRs\ /e /s
cd Resources
copy %PROJECT_DIR%\Themes\Body.json || exit /b
ren Body.json theme.json
cd %PROJECT_DIR%

cd dist\BodyandSoul\TSoM-BodyandSoulSoul.vst3\Contents\x86_64-win
copy %PROJECT_DIR%\Projects\Soul\Builds\VisualStudio2019\x64\Release\VST3\TSoM-BodyandSoulSoul.vst3\Contents\x86_64-win\TSoM-BodyandSoulSoul.vst3 || exit /b
cd ..
mkdir Resources\IRs || exit /b
robocopy %PROJECT_DIR%\IRs\Soul Resources\IRs\ /e /s
cd Resources
copy %PROJECT_DIR%\Themes\Soul.json || exit /b
ren Soul.json theme.json
cd %PROJECT_DIR%

@REM cd dist\BodyandSoul\TSoM-BodyandSoulFX.vst3\Contents\x86_64-win
@REM copy %PROJECT_DIR%\Projects\FX\Builds\VisualStudio2019\x64\Release\VST3\TSoM-BodyandSoulFX.vst3\Contents\x86_64-win\TSoM-BodyandSoulFX.vst3 || exit /b
@REM cd ..
@REM mkdir Resources\IRs || exit /b
@REM robocopy %PROJECT_DIR%\IRs\FX Resources\IRs\ /e /s
@REM cd Resources
@REM copy %PROJECT_DIR%\Themes\FX.json || exit /b
@REM ren FX.json theme.json
@REM cd %PROJECT_DIR%
