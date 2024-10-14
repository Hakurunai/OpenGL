@echo off
setlocal enabledelayedexpansion

set GLFW_URL=https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip

set OUTPUT_DIR=thirdParty\libs
set TEMP_DIR=%OUTPUT_DIR%\temp

set GLFW_ARCHIVE=%OUTPUT_DIR%\glfw.zip
set GLFW_LIB=%OUTPUT_DIR%\glfw-3.4.bin.WIN64\lib-vc2022\glfw3.lib


if not exist %TEMP_DIR% (
	echo Creating folder %TEMP_DIR%
	mkdir %TEMP_DIR%
)


REM Check if GLFW is already installed
if exist %GLFW_LIB% (
    echo GLFW is already installed. Skipping download...
) else (
    echo GLFW is not found or incomplete. Proceeding with download...
	
    echo Downloading GLFW binaries...
    curl -L %GLFW_URL% -o %GLFW_ARCHIVE%
	call :CheckDownloadError
	

    echo Extracting GLFW...
	call :UnzipFileToLocation %GLFW_ARCHIVE% %TEMP_DIR%

	REM Use PowerShell to get the name of the extracted folder
	for /f "delims=" %%i in ('powershell -NoProfile -Command "(Get-ChildItem -Path '%TEMP_DIR%' | Where-Object { $_.PSIsContainer } | Sort-Object LastWriteTime -Descending | Select-Object -First 1).Name"') do set EXTRACTED_FOLDER=%%i
	echo Extracted folder: !EXTRACTED_FOLDER!

	REM Creation of the final directory
	set "GLFW_FINAL_DIR=!OUTPUT_DIR!\!EXTRACTED_FOLDER!
	
	echo Final dir : !GLFW_FINAL_DIR!
	if not exist "!GLFW_FINAL_DIR!" (
		echo Creating folder !GLFW_FINAL_DIR!
		mkdir "!GLFW_FINAL_DIR!"
	)
	
	REM Copy desired folders and files in final destination
	call :CopyFolderFromTo !TEMP_DIR!\!EXTRACTED_FOLDER!\include !GLFW_FINAL_DIR!\include
	call :CopyFolderFromTo !TEMP_DIR!\!EXTRACTED_FOLDER!\lib-vc2022 !GLFW_FINAL_DIR!\lib-vc2022
	call :CopyFileFromTo !TEMP_DIR!\!EXTRACTED_FOLDER!\LICENSE.md !GLFW_FINAL_DIR!\LICENSE.md
	call :CopyFileFromTo !TEMP_DIR!\!EXTRACTED_FOLDER!\README.md !GLFW_FINAL_DIR!\README.md	
	
	echo temp : %TEMP_DIR%
	
    REM Clean up by deleting the downloaded zip file
    echo Cleaning up...
    del /Q %GLFW_ARCHIVE%

    echo GLFW setup complete !
)

set GLAD_URL=https://glad.dav1d.de/generate
set GLAD_ARCHIVE=%TEMP_DIR%\glad.zip

REM Download GLAD with OpenGL 4.6 Core profile in C/C++
echo Downloading GLAD archive...
curl -X POST "%GLAD_URL%" -H "Content-Type: application/x-www-form-urlencoded" --data "spec=gl&api=gl=4.6&profile=core&generator=c&language=c++" -L >nul
call :CheckDownloadError

REM (manual step to find the correct zip download URL)
set DOWNLOAD_URL=https://glad.dav1d.de/generated/tmp_dohdm05glad/glad.zip

REM Download the archive
curl -L -o "%GLAD_ARCHIVE%" "%DOWNLOAD_URL%"
call :CheckDownloadError

echo Extracting GLAD...
REM Unzip the downloaded GLAD zip file using PowerShell
call :UnzipFileToLocation %GLAD_ARCHIVE% %TEMP_DIR%


echo GLAD setup complete !


REM removing temp directory and his content
REM rmdir /S /Q %TEMP_DIR%
echo Status : Preparation ended.
endlocal
pause
exit /b



REM ====== Functions definitions ======

:CopyFolderFromTo
xcopy /E /Y /I %1 %2
exit /b

:CopyFileFromTo
xcopy /E /Y /-I %1 %2
exit /b

:UnzipFileToLocation
powershell -Command "Expand-Archive -Path %1 -DestinationPath %2"
call :CheckExtractionError
exit /b

:CheckExtractionError
if %ERRORLEVEL% neq 0 (
	echo !!! Extraction failed! Please check the zip file and try again. !!!
	exit /b 1
)	
exit /b

:CheckDownloadError
if %ERRORLEVEL% neq 0 (
	echo !!! Download failed ! Please check your internet connection and try again. !!!
	exit /b 1
)
exit /b