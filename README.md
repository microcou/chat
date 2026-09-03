# Chat

Client/server instant messaging software. Contains :
- A server (linux only), start command : `server_linux 12345`
- A client for Linux and a client for Windows, start command : `client_linux 192.168.56.102 12345`

It also contains basic versions of the clients that are barely functional but as simple as possible.

Development is assisted by AI : Github Copilot, ChatGPT

## TODO

- Check if using boost asio for networking leads to more portable code in regards to Linux/Windows
- Use ncurses or another terminal management library, portable if possible, to replace the messy code.
- Study the IRC protocol
- Setup github actions to build the binaries and store in a release

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