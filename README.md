# Chat

Client/server instant messaging software. Contains :
- server
- client_basic
- client
- client_basic_windows
- client_windows

Development is assisted by AI : Github Copilot, ChatGPT

## TODO

- Check if using boost asio for networking leads to more portable code in regards to Linux/Windows
- Use ncurses or another terminal management library, portable if possible, to replace the messy code.
- Study the IRC protocol

## Linux

make

## Windows

The build can be performed from a specially setup cmd : Developer Command Prompt.
It should be configured in vscode preferences (JSON) :
```
    "terminal.integrated.profiles.windows": {
        "Developer Command Prompt": {
            "path": "C:\\Windows\\system32\\cmd.exe",
            "args": [
                "/k",
                "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\Common7\\Tools\\VsDevCmd.bat",
                "-arch=x64"
            ]
        }
    },
```
Then use this profile: Ctrl+Shift+P > Terminal : Select Default Profile

nmake /f Makefile.windows

## Vscode

Install the extension C/C++ Extension Pack.
Then Ctrl+Shift+P > C/C++ : Select a Configuration > choose one from c_cpp_properties.json