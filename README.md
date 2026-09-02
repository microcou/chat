# Chatcpp

Logiciel client/serveur de chat.
- server : le serveur
- client_basic : le client le plus simple
- client : un client plus complexe : conserve le message en cours de frappe lors de la réception d'un message.
- client_basic_windows
- client_windows : TODO

## TODO

- Etudier si l'utilisation de boost asio permet du code plus portable Linux/Windows
- Utiliser ncurses
- Etudier le protocole IRC

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