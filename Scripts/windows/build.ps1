$ErrorActionPreference = "Stop"

$SCRIPT_DIR = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent

Write-Output "=== Downloading JUCE ==="
$JUCE_VERSION = "8.0.4"
$JUCE_ZIP = Join-Path -Path $SCRIPT_DIR -ChildPath "..\..\juce-$JUCE_VERSION-windows.zip"
$JUCE_URL = "https://github.com/juce-framework/JUCE/releases/download/$JUCE_VERSION/juce-$JUCE_VERSION-windows.zip"
Invoke-WebRequest -Uri $JUCE_URL -OutFile $JUCE_ZIP -UseBasicParsing

Expand-Archive -LiteralPath $JUCE_ZIP -DestinationPath (Join-Path -Path $SCRIPT_DIR -ChildPath "..\..")

Write-Output "=== Generating projects ==="
$JUCE_HOME = Join-Path -Path $SCRIPT_DIR -ChildPath "..\..\JUCE"
$PROJECT_DIR = Join-Path -Path $SCRIPT_DIR -ChildPath "..\.."

& "$JUCE_HOME\Projucer.exe" --resave "$PROJECT_DIR\Projects\Intro\BodyandSoulIntro.jucer"
& "$JUCE_HOME\Projucer.exe" --resave "$PROJECT_DIR\Projects\Body\BodyandSoulBody.jucer"
& "$JUCE_HOME\Projucer.exe" --resave "$PROJECT_DIR\Projects\Soul\BodyandSoulSoul.jucer"
# & "$JUCE_HOME\Projucer.exe" --resave "$PROJECT_DIR\Projects\FX\BodyandSoulFX.jucer"

Write-Output "=== Starting VST builds ==="
msbuild -version

cd "$PROJECT_DIR\Projects\Intro\Builds\VisualStudio2019"
msbuild "Body and Soul Intro.sln" /p:Configuration=Release /p:Platform=x64

cd "$PROJECT_DIR\Projects\Body\Builds\VisualStudio2019"
msbuild "Body and Soul Body.sln" /p:Configuration=Release /p:Platform=x64

cd "$PROJECT_DIR\Projects\Soul\Builds\VisualStudio2019"
msbuild "Body and Soul Soul.sln" /p:Configuration=Release /p:Platform=x64

# cd "$PROJECT_DIR\Projects\FX\Builds\VisualStudio2019"
# msbuild "Body and Soul FX.sln" /p:Configuration=Release /p:Platform=x64

# Write-Output "=== Starting CLAP builds ==="
# function BuildCLAP {
#     cmake -Bbuild-clap -GXcode -DCMAKE_BUILD_TYPE=Release
#     cmake --build build-clap --config Release
# }

# cd "$PROJECT_DIR\Projects\Intro" && BuildCLAP
# cd "$PROJECT_DIR\Projects\Body" && BuildCLAP
# cd "$PROJECT_DIR\Projects\Soul" && BuildCLAP

Write-Output "=== Collecting artefacts ==="
cd "$PROJECT_DIR"

function CollectVST {
    param(
        [string]$outputDir,
        [string]$variant
    )

    $pluginName = "TSoM-BodyandSoul$variant.vst3"

    # Copy the binary
    New-Item -ItemType Directory -Force -Path "$outputDir\$pluginName\Contents\x86_64-win"
    Copy-Item "$PROJECT_DIR\Projects\$variant\Builds\VisualStudio2019\x64\Release\VST3\$pluginName\Contents\x86_64-win\$pluginName" "$outputDir\TSoM-BodyandSoul$variant.vst3\Contents\x86_64-win"

    # Copy the resources
    New-Item -ItemType Directory -Force -Path "$outputDir\$pluginName\Contents\Resources\IRs"
    Copy-Item -Recurse "$PROJECT_DIR\IRs\$variant" "$outputDir\$pluginName\Contents\Resources\IRs"
    Copy-Item "$PROJECT_DIR\Themes\$variant.json" "$outputDir\$pluginName\Contents\Resources\theme.json"
}

function CollectCLAP {
    param(
        [string]$outputDir,
        [string]$variant
    )

    $pluginName = "TSoM-BodyandSoul$variant.clap"

    # # Copy the binary
    # New-Item -ItemType Directory -Force -Path "$outputDir\$pluginName/Contents/x86_64-win"
    # Copy-Item "$PROJECT_DIR\Projects\$variant\build-clap\TSoM-BodyandSoul${VARIANT}_artefacts\Release\$pluginName\Contents\x86_64-win\TSoM-BodyandSoul$variant" "$outputDir\$pluginName\Contents\x86_64-win"

    # # Copy the resources
    # New-Item -ItemType Directory -Force -Path "$outputDir\$pluginName\Contents\Resources\IRs"
    # Copy-Item -Recurse "$PROJECT_DIR\IRs\$variant" "$outputDir\$pluginName\Contents\Resources\IRs"
    # Copy-Item "$PROJECT_DIR\Themes\$variant.json" "$outputDir\$pluginName\Contents\Resources\theme.json"

    # For now, just copy from the arefacts so we can see what it looks like
    # Copy-Item -Recurse "$PROJECT_DIR\Projects\$variant\build-clap\TSoM-BodyandSoul${VARIANT}_artefacts\Release\$pluginName" "$outputDir\$pluginName"
}

CollectVST "dist\BodyandSoulIntro" "Intro"
# CollectCLAP "dist\BodyandSoulIntro" "Intro"
CollectVST "dist\BodyandSoul" "Body"
# CollectCLAP "dist\BodyandSoul" "Body"
CollectVST "dist\BodyandSoul" "Soul"
# CollectCLAP "dist\BodyandSoul" "Soul"
# CollectVST "dist\BodyandSoul" "FX"


