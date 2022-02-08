# Sudo for Windows

This software is licensed under [MIT Lincense](LICENSE.md).

- **⚠ Warning: This software may have security vulnerabilities and should be used with caution.**

[Latest release](https://github.com/any-possible/Sudo-for-Windows/releases/latest)

This software has a kernel-mode extension ([Kernel API Hook](src/Kernel%20API%20Hook), [Kernel Mode Entry](src/Kernel%20Mode%20Entry), [Kernel Mode Host](src/Kernel%20Mode%20Host)), but is not available now.

- In the future, I will provide a way to install and uninstall automatically.

## 1.Introducing

As we all know, in Linux we can use sudo and su to elevate privileges (get root privileges).

But in Windows, we shuold elevate privileges through UAC (User Account Control). This makes us have to open a new terminal with administrator privileges when we want to elevate the terminal to the commands.

So I made this software. It can help us run commands as administrator without having to open the new terminal.

## 2.Install

First, download the compressed package of this software ([download the latest version](https://github.com/any-possible/Sudo-for-Windows/releases/latest)) and extract to a secure directory (**DON'T ALLOW *STANDARD USERS* TO MODIFY THE FILES INSIDE, OTHERWISE IT MAY LEAD TO *VULNERABILITY ELEVATION***).

Use this command to create the service for this software:

```powershell
sc create SudoService type= own start= auto binPath= "<SERVICE_PROGRAM>" DisplayName= "Sudo for Windows Service"
```

- Replace <SERVICE_PROGRAM> with the path to "SudoService.exe".

Then, start the service:

```powershell
sc start SudoService
```

- The service will start automatically the next time.

At last, add the directory where the software is stored to the PATH environment variable.

**Now, you can use sudo and su commands in Windows!**

## 3.Uninstall

Use this command to stop the service:

```powershell
sc stop SudoService
```

Then, remove the service:

```powershell
sc delete SudoService
```

At last, remove the directory where the software is stored.
