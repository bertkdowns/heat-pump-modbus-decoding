Created 
Install-UsbipdWslAutoAttach.ps1.

Run from an elevated Windows PowerShell terminal:

usbipd list

.\Install-UsbipdWslAutoAttach.ps1 `
    -HardwareId 0483:374B,1A86:7523 `
    -Distro Ubuntu

Replace those IDs with the VID:PID values shown by usbipd list. The task will:

Start at boot/sign-in.
Start the selected WSL distribution.
Detect matching devices every two seconds.
Bind devices when necessary.
Attach and reattach them after plugging in, resetting, or reconnecting.
Run with elevated privileges.

To attach every eligible USB device:

.\Install-UsbipdWslAutoAttach.ps1 -AllDevices -Distro Ubuntu

Be careful: this can seize keyboards, mice, storage, Bluetooth, and webcams from Windows. The allow-list form is strongly recommended.

Check its operation with:

usbipd list
Get-ScheduledTask "Usbipd WSL Auto-Attach"
Get-Content C:\ProgramData\UsbipdWslAutoAttach\watcher.log -Wait

Remove it with:

.\Install-UsbipdWslAutoAttach.ps1 -Uninstall

Attachment is available to all WSL 2 distributions, although -Distro selects which distribution is started to bring up the WSL VM. This follows usbipd-win’s current usbipd state/PowerShell automation interface and WSL attachment syntax.