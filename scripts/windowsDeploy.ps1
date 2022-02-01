#################################################
## Deploy windows ProxyEmy application
##
## You need create environment variable QTWINDEPLOYPATH
## and set value to full path of windeployqt.exe
##
#################################################


# Prepare files
Remove-Item -Path ../deploy/output -Recurse -Force -ErrorAction Ignore
New-Item -Path ../deploy -Name "output" -ItemType "directory"
Copy-Item -Path ../deploy/releasebuild/ProxyEmy.exe -Destination ../deploy/output

# Build project

& $Env:QTWINDEPLOYPATH --no-translations --release --no-sql --no-xml --no-positioning --no-system-d3d-compiler --no-virtualkeyboard --no-opengl-sw --no-qmltooling --no-3dcore --no-3drenderer --no-3dquick --no-3dquickrenderer --no-3dinput --no-3danimation --no-3dextras --no-serialport --qmldir ../src ../deploy\output

# Remove redundant thing
Remove-Item -Path ../deploy/output\Qt6QuickTimeline.dll
Remove-Item -Path ../deploy/output\Qt6QmlXmlListModel.dll
Remove-Item -Path ../deploy/output\Qt6QmlLocalStorage.dll
Remove-Item -Path ../deploy/output\QtQuick\Controls\Fusion -Recurse -Force
Remove-Item -Path ../deploy/output\QtQuick\Controls\Imagine -Recurse -Force
Remove-Item -Path ../deploy/output\QtQuick\Controls\Material -Recurse -Force
Remove-Item -Path ../deploy/output\QtQuick\Controls\Windows -Recurse -Force

# Copy license
New-Item -Path ../deploy/output -Name "licenses" -ItemType "directory"
Copy-Item -Path ../deploy/licenses/* -Destination ../deploy/output/licenses

# Copy certificates
New-Item -Path ../deploy/output -Name "certificate" -ItemType "directory"
Copy-Item -Path *.crt -Destination ../deploy/output/certificate
Copy-Item -Path *.key -Destination ../deploy/output/certificate
Copy-Item -Path *.cer -Destination ../deploy/output/certificate

# Compress final build to zip file
Remove-Item -Path ../deploy/release.zip -ErrorAction Ignore
Compress-Archive -Path ../deploy/output/* -DestinationPath ../deploy/release.zip