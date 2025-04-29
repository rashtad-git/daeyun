# Set UTF-8 encoding
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8

# Store current directory
$originalDir = Get-Location

# Create build directory
if (-not (Test-Path -Path "build")) {
    New-Item -ItemType Directory -Path "build"
}

# Move to build directory
Set-Location -Path "build"

# Run CMake
cmake ..
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake failed"
    Set-Location $originalDir
    exit 1
}

# Build project
cmake --build .
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed"
    Set-Location $originalDir
    exit 1
}

Write-Host "Build completed!"

# Run executable
$exePath = Join-Path -Path (Get-Location) -ChildPath "bin\Debug\MyProject.exe"
if (Test-Path -Path $exePath) {
    Write-Host "Running program..."
    # Return to original directory before running the program
    Set-Location $originalDir
    # Run program in background
    Start-Process -FilePath $exePath
} else {
    Write-Error "Executable not found: $exePath"
    Set-Location $originalDir
    exit 1
} 