# Sudo for Windows

This software is licensed under [MIT Lincense](LICENSE).

- **⚠ Warning: This software may have security vulnerabilities and should be used with caution.**

[Latest release](https://github.com/idealarch/Sudo-for-Windows/releases/latest)

- In the future, I will provide a way to install and uninstall automatically.

## 1.Introducing

As we all know, in Linux we can use sudo and su to elevate privileges (get root privileges).

But in Windows, we shuold elevate privileges through UAC (User Account Control). This makes us have to open a new terminal with administrator privileges when we want to run commands as administrator.

So I made this software. It can help us run commands as administrator without having to open a new terminal.

## 2.Install

First, download the compressed package of this software ([download the latest version](https://github.com/idealarch/Sudo-for-Windows/releases/latest)) and extract to a secure directory (**DON'T ALLOW *STANDARD USERS* TO MODIFY THE FILES INSIDE, OTHERWISE IT MAY LEAD TO *VULNERABILITY ELEVATION***).

Use this command to create the service for this software:

```powershell
sc create SudoService type= own start= auto binPath= "<service>" DisplayName= "Sudo for Windows Service"
```

- Replace \<service\> with the path to "SudoService.exe".

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

## 4.Build

Environment:

- **[Microsoft Visual Studio 2019](https://aka.ms/vs)** or more *with* **Microsoft Visual C++ Build Tools v142** or more

Commands:

```powershell
git clone https://github.com/idealarch/Sudo-for-Windows.git
cd Sudo-for-Windows
msbuild "Sudo for Windows.sln" -property:Configuration=Release -property:Platform=x64 # or "x86" if you want
```

Then you can see the target files in the directory "release\\x64" (or "release\\x86" if building with Platform=x86).
There should be the following target files:

- su.exe
- sudo.exe
- SudoConfiguration.dll
- SudoHelper.exe
- SudoRpcClient.dll
- SudoService.exe
