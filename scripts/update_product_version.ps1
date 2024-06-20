[CmdletBinding()]
param(
    [Parameter(Mandatory=$false, ParameterSetName='Version')]
    [int]$major,
    [Parameter(Mandatory=$false, ParameterSetName='Version')]
    [int]$minor,
    [Parameter(Mandatory=$true, ParameterSetName='Version')]
    [int]$revision
)

$scriptPath = Split-Path -Parent $MyInvocation.MyCommand.Definition
$headerFile = Join-Path $scriptPath "../MissionEditor/Version.h"

$content = Get-Content -Path $headerFile -Raw

if ($PSBoundParameters.ContainsKey('major')) {
    $content = $content -replace 'PRODUCT_VERSION_MAJOR\s+\d+', "PRODUCT_VERSION_MAJOR $major"
}

if ($PSBoundParameters.ContainsKey('minor')) {
    $content = $content -replace 'PRODUCT_VERSION_MINOR\s+\d+', "PRODUCT_VERSION_MINOR $minor"  
}

if ($PSBoundParameters.ContainsKey('revision')) {
    $content = $content -replace 'PRODUCT_VERSION_REVISION\s+\d+', "PRODUCT_VERSION_REVISION $revision"
}

$content | Set-Content -Path $headerFile