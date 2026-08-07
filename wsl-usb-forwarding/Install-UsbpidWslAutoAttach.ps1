#Requires -Version 5.1
<#
.SYNOPSIS
Installs a scheduled watcher that binds selected USB devices and attaches them to WSL.

.EXAMPLE
  # Run from an elevated PowerShell terminal:
  .\Install-UsbipdWslAutoAttach.ps1 -HardwareId 0483:374b,1a86:7523 -Distro Ubuntu

.EXAMPLE
  # DANGEROUS: hand every eligible USB device to WSL:
  .\Install-UsbipdWslAutoAttach.ps1 -AllDevices -Distro Ubuntu

.EXAMPLE
  .\Install-UsbipdWslAutoAttach.ps1 -Uninstall
#>
[CmdletBinding(DefaultParameterSetName = 'InstallSelected')]
param(
    [Parameter(ParameterSetName = 'InstallSelected', Mandatory = $true)]
    [ValidatePattern('^[0-9A-Fa-f]{4}:[0-9A-Fa-f]{4}$')]
    [string[]] $HardwareId,

    [Parameter(ParameterSetName = 'InstallAll', Mandatory = $true)]
    [switch] $AllDevices,

    [Parameter(ParameterSetName = 'InstallSelected')]
    [Parameter(ParameterSetName = 'InstallAll')]
    [string] $Distro,

    [Parameter(ParameterSetName = 'InstallSelected')]
    [Parameter(ParameterSetName = 'InstallAll')]
    [ValidateRange(1, 300)]
    [int] $PollSeconds = 2,

    [Parameter(ParameterSetName = 'Uninstall', Mandatory = $true)]
    [switch] $Uninstall,

    [Parameter(ParameterSetName = 'Worker', Mandatory = $true, DontShow = $true)]
    [switch] $Worker,

    [Parameter(ParameterSetName = 'Worker', DontShow = $true)]
    [string] $ConfigPath = "$env:ProgramData\UsbipdWslAutoAttach\config.json"
)

$ErrorActionPreference = 'Stop'
$taskName = 'Usbipd WSL Auto-Attach'
$installDirectory = Join-Path $env:ProgramData 'UsbipdWslAutoAttach'
$installedScript = Join-Path $installDirectory 'UsbipdWslAutoAttach.ps1'
$defaultConfigPath = Join-Path $installDirectory 'config.json'
$logPath = Join-Path $installDirectory 'watcher.log'

function Assert-Administrator {
    $identity = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = [Security.Principal.WindowsPrincipal]::new($identity)
    if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
        throw 'Run this installer from an elevated PowerShell terminal (Run as administrator).'
    }
}

function Write-WatcherLog([string] $Message) {
    $line = '{0:yyyy-MM-dd HH:mm:ss} {1}' -f (Get-Date), $Message
    Add-Content -LiteralPath $logPath -Value $line -Encoding UTF8
}

function Get-VidPid($Device) {
    if ($Device.PSObject.Properties.Name -contains 'HardwareId' -and $Device.HardwareId) {
        $text = [string]$Device.HardwareId
        if ($text -match '(?i)([0-9a-f]{4}):([0-9a-f]{4})') {
            return ('{0}:{1}' -f $Matches[1], $Matches[2]).ToLowerInvariant()
        }
    }
    if ([string]$Device.InstanceId -match '(?i)VID_([0-9a-f]{4}).*PID_([0-9a-f]{4})') {
        return ('{0}:{1}' -f $Matches[1], $Matches[2]).ToLowerInvariant()
    }
    return $null
}

function Invoke-Worker {
    if (-not (Test-Path -LiteralPath $ConfigPath)) {
        throw "Configuration not found: $ConfigPath"
    }

    $config = Get-Content -LiteralPath $ConfigPath -Raw | ConvertFrom-Json
    $usbipd = Get-Command usbipd.exe -ErrorAction Stop
    $modulePath = Join-Path $env:ProgramW6432 'usbipd-win\Usbipd.Powershell.dll'
    if (-not (Test-Path -LiteralPath $modulePath)) {
        throw "usbipd-win PowerShell module not found at '$modulePath'. Install usbipd-win 2.2.0 or newer."
    }
    Import-Module $modulePath -ErrorAction Stop

    # Start the user's WSL VM. USB devices attached to WSL are visible to every
    # running WSL 2 distribution, but selecting one here makes startup reliable.
    if ($config.Distro) {
        & wsl.exe --distribution ([string]$config.Distro) --exec /bin/true 2>&1 | Out-Null
    } else {
        & wsl.exe --exec /bin/true 2>&1 | Out-Null
    }

    $wanted = @($config.HardwareId | ForEach-Object { ([string]$_).ToLowerInvariant() })
    Write-WatcherLog "Watcher started (allDevices=$($config.AllDevices); hardwareIds=$($wanted -join ','))."

    while ($true) {
        try {
            $devices = @(Get-UsbipdDevice | Where-Object { $_.IsConnected -and $_.BusId })
            foreach ($device in $devices) {
                $vidPid = Get-VidPid $device
                if (-not $config.AllDevices -and $vidPid -notin $wanted) { continue }

                if (-not $device.IsBound) {
                    Write-WatcherLog "Binding $($device.BusId) $vidPid $($device.Description)"
                    & $usbipd.Source bind --busid ([string]$device.BusId) 2>&1 |
                        ForEach-Object { Write-WatcherLog ([string]$_) }
                    if ($LASTEXITCODE -ne 0) { continue }
                }

                # Refresh state because binding changes the device object.
                $current = Get-UsbipdDevice | Where-Object { $_.BusId -eq $device.BusId } | Select-Object -First 1
                if ($current -and -not $current.IsAttached) {
                    Write-WatcherLog "Attaching $($device.BusId) $vidPid $($device.Description)"
                    & $usbipd.Source attach --wsl --busid ([string]$device.BusId) 2>&1 |
                        ForEach-Object { Write-WatcherLog ([string]$_) }
                }
            }
        } catch {
            Write-WatcherLog "ERROR: $($_.Exception.Message)"
        }
        Start-Sleep -Seconds ([int]$config.PollSeconds)
    }
}

if ($Worker) {
    Invoke-Worker
    exit
}

Assert-Administrator

if ($Uninstall) {
    Unregister-ScheduledTask -TaskName $taskName -Confirm:$false -ErrorAction SilentlyContinue
    if (Test-Path -LiteralPath $installDirectory) {
        Remove-Item -LiteralPath $installDirectory -Recurse -Force
    }
    Write-Host "Removed scheduled task '$taskName' and $installDirectory"
    exit
}

if (-not (Get-Command usbipd.exe -ErrorAction SilentlyContinue)) {
    throw 'usbipd.exe was not found. Install usbipd-win first.'
}
if (-not (Get-Command wsl.exe -ErrorAction SilentlyContinue)) {
    throw 'wsl.exe was not found. Install WSL 2 first.'
}

New-Item -ItemType Directory -Path $installDirectory -Force | Out-Null
Copy-Item -LiteralPath $PSCommandPath -Destination $installedScript -Force

$config = [ordered]@{
    AllDevices = [bool]$AllDevices
    HardwareId = @($HardwareId | ForEach-Object { $_.ToLowerInvariant() })
    Distro = $Distro
    PollSeconds = $PollSeconds
}
$config | ConvertTo-Json | Set-Content -LiteralPath $defaultConfigPath -Encoding UTF8

$powerShell = Join-Path $PSHOME 'powershell.exe'
$actionArguments = '-NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass -File "{0}" -Worker -ConfigPath "{1}"' -f $installedScript, $defaultConfigPath
$action = New-ScheduledTaskAction -Execute $powerShell -Argument $actionArguments

# WSL distributions are registered per Windows user, so the task must run as
# the installing user. The logon trigger is the reliable startup point. A boot
# trigger is included as well for sessions where that user token already exists.
$startupTrigger = New-ScheduledTaskTrigger -AtStartup
$startupTrigger.Delay = 'PT20S'
$logonTrigger = New-ScheduledTaskTrigger -AtLogOn -User "$env:USERDOMAIN\$env:USERNAME"
$settings = New-ScheduledTaskSettingsSet -StartWhenAvailable -RestartCount 999 -RestartInterval (New-TimeSpan -Minutes 1) -ExecutionTimeLimit ([TimeSpan]::Zero)
$principal = New-ScheduledTaskPrincipal -UserId "$env:USERDOMAIN\$env:USERNAME" -LogonType Interactive -RunLevel Highest

Register-ScheduledTask -TaskName $taskName -Action $action -Trigger @($startupTrigger, $logonTrigger) -Settings $settings -Principal $principal -Force | Out-Null
Start-ScheduledTask -TaskName $taskName

Write-Host "Installed and started '$taskName'."
Write-Host "Configuration: $defaultConfigPath"
Write-Host "Log:           $logPath"
Write-Host 'Use usbipd list to verify that matching devices become Attached.'