@echo off
setlocal enabledelayedexpansion

REM Set GLFW variables
set GLFW_URL=https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
set GLFW_ARCHIVE=thirdParty\libs\glfw.zip
set GLFW_OUTPUT_DIR=thirdParty\libs
set GLFW_TEMP=%GLFW_OUTPUT_DIR%\temp
set GLFW_LIB=%GLFW_OUTPUT_DIR%\glfw-3.4.bin.WIN64\lib-vc2022\glfw3.lib

REM Check if GLFW is already installed
if exist %GLFW_LIB% (
    echo GLFW is already installed. Skipping download...
	
) else (
    echo GLFW is not found or incomplete. Proceeding with download...

    REM Create the directory if it doesn't exist
    if not exist %GLFW_TEMP% (
        echo Creating folder %GLFW_TEMP%
        mkdir %GLFW_TEMP%
    )
	
	REM Download the GLFW binaries
    echo Downloading GLFW binaries...
    curl -L %GLFW_URL% -o %GLFW_ARCHIVE%

    REM Check if the download was successful
    if %ERRORLEVEL% neq 0 (
        echo !!! Download failed ! Please check your internet connection and try again. !!!
        exit /b 1
    )
	
	REM Extract the downloaded zip file into the destination folder
    echo Extracting GLFW...
    powershell -Command "Expand-Archive -Path %GLFW_ARCHIVE% -DestinationPath %GLFW_TEMP%"

    REM Check if the extraction was successful
    if %ERRORLEVEL% neq 0 (
        !!! echo Extraction failed! Please check the zip file and try again. !!!
        exit /b 1
    )	

	REM Use PowerShell to get the name of the extracted folder
	for /f "delims=" %%i in ('powershell -NoProfile -Command "(Get-ChildItem -Path '%GLFW_TEMP%' | Where-Object { $_.PSIsContainer } | Sort-Object LastWriteTime -Descending | Select-Object -First 1).Name"') do set EXTRACTED_FOLDER=%%i
	echo Extracted folder: !EXTRACTED_FOLDER!

	REM Creation of the final directory
	set "GLFW_FINAL_DIR=!GLFW_OUTPUT_DIR!\!EXTRACTED_FOLDER!
	
	
	echo Final dir : !GLFW_FINAL_DIR!
	if not exist "!GLFW_FINAL_DIR!" (
		echo Creating folder !GLFW_FINAL_DIR!
		mkdir "!GLFW_FINAL_DIR!"
	)
	
	REM Copy desired folders and files in final destination
	xcopy /E /Y /I !GLFW_TEMP!\!EXTRACTED_FOLDER!\include !GLFW_FINAL_DIR!\include
	xcopy /E /Y /I !GLFW_TEMP!\!EXTRACTED_FOLDER!\lib-vc2022 !GLFW_FINAL_DIR!\lib-vc2022
	xcopy /E /Y /-I !GLFW_TEMP!\!EXTRACTED_FOLDER!\LICENSE.md !GLFW_FINAL_DIR!\LICENSE.md
	xcopy /E /Y /-I !GLFW_TEMP!\!EXTRACTED_FOLDER!\README.md !GLFW_FINAL_DIR!\README.md	
	
	echo temp : %GLFW_TEMP%
	
    REM Clean up by deleting the downloaded zip file
    echo Cleaning up...
    del /Q %GLFW_ARCHIVE%
	rmdir /S /Q %GLFW_TEMP%

    echo GLFW setup complete !
)

echo Status : Preparation ended.
endlocal