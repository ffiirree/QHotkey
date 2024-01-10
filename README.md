<h1 align="center">QHotkey</h1>

A global shortcut/hotkey for Desktop Qt-Applications.

The QHotkey is a class that can be used to create hotkeys/global shortcuts, aka shortcuts that work everywhere, independent of the application state. This means your application can be active, inactive, minimized or not visible at all and still receive the shortcuts.

## Features

- Works on **Windows**, **Mac** and **X11**
- Easy to use, can use `QKeySequence` for easy shortcut input
- Supports almost all common keys (Depends on OS & Keyboard-Layout)
- Allows direct input of Key/Modifier-Combinations
- Supports multiple QHotkey-instances for the same shortcut (with optimisations)
- **Thread-Safe** - Can be used on all threads (See section Thread safety)
- Allows usage of native keycodes and modifiers, if needed

**Note:** For now Wayland is not supported, as it is simply not possible to register a global shortcut with wayland. For more details, or possible Ideas on how to get Hotkeys working on wayland, see [Issue #14](https://github.com/Skycoder42/QHotkey/issues/14).

## Building

QHotkey supports both Qt6 and Qt5. When using Qt6, version 6.2.0 or later required. It can be built by either the CMake or qmake building systems.

### CMake

```bash
cd QHotkey
cmake -B build -S . -DBUILD_SHARED_LIBS=ON
cmake --build build -j4 --config Relase # --target intall
```

#### Options

- `QT_DEFAULT_MAJOR_VERSION` : which major version of Qt is used (default `5`).
- `QHOTKEY_EXAMPLES` : build the testing application.
- `BUILD_SHARED_LIBS` : build static or shared libraries.

## Usage

The general usage is to create QHotkey instances for specific hotkeys, register them and then simply connect to the signal emitted once the hotkey is pressed.

### Example

The following example shows a simple application that will run without a window in the background until you press the key-combination <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>Q</kbd> (<kbd>⌘</kbd>+<kbd>⌥</kbd>+<kbd>Q</kbd> on Mac). This will quit the application. The debug output will tell if the hotkey was successfully registered and that it was pressed.

```cpp
#include <QHotkey>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QHotkey hotkey(QKeySequence("Ctrl+Alt+Q"), true, &app); //The hotkey will be automatically registered
    qDebug() << "Is segistered:" << hotkey.isRegistered();

    QObject::connect(&hotkey, &QHotkey::activated, qApp, [&](){
        qDebug() << "Hotkey Activated - the application will quit now";
        qApp->quit();
    });

    return app.exec();
}
```

### Testing

By running the example in `./HotkeyTest` you can test out the QHotkey class. There are 4 sections:

- **Playground:** You can enter some sequences here and try it out with different key combinations.
- **Testings:** A list of selected hotkeys. Activate it and try out which ones work for you (*Hint:* Depending on OS and keyboard layout, it's very possible that a few don't work).
- **Threading:** Activate the checkbox to move 2 Hotkeys of the playground to separate threads. It should work without a difference.
- **Native Shortcut**: Allows you to try out the direct usage of native shortcuts

### Logging

By default, QHotkey prints some warning messages if something goes wrong (For example, a key that cannot be translated). All messages of QHotkey are grouped into the [QLoggingCategory](https://doc.qt.io/qt-5/qloggingcategory.html) `"QHotkey"`. If you want to simply disable the logging, call the following function somewhere in your code:

```cpp
QLoggingCategory::setFilterRules(QStringLiteral("QHotkey.warning=false"));
```

This will turn all warnings of QHotkey of (It only uses warnings for now, that's why this is enough). For more information about all the things you can do with the logging categories, check the Qt-Documentation

## Thread safety

The QHotkey class itself is reentrant - which means you can create as many instances as required on any thread. This allows you to use the QHotkey on all threads. **But** you should never use the QHotkey instance on a thread that is different from the one the instance belongs to! Internally the system uses a singleton instance that handles the hotkey events and distributes them to the QHotkey instances. This internal class is completely threadsafe.

However, this singleton instance only runs on the main thread. (One reason is that some of the OS-Functions are not thread safe). To make threaded hotkeys possible, the critical functions (registering/unregistering hotkeys and keytranslation) are all run on the mainthread too. The QHotkey instances on other threads use `QMetaObject::invokeMethod` with a `Qt::BlockingQueuedConnection`.

For you this means: QHotkey instances on other threads than the main thread may take a little longer to register/unregister/translate hotkeys, because they have to wait for the main thread to do this for them. **Important:** there is however, one additional limitation that comes with that feature: QHotkey instances on other threads but the main thread *must* be unregistered or destroyed *before* the main eventloop ends. Otherwise, your application will hangup on destruction of the hotkey. This limitation does not apply for instances on the main thread. Furthermore, the same happens if you change the shortcut or register/unregister before the loop started, until it actually starts.

## Documentation

The documentation is available as release and on [github pages](https://skycoder42.github.io/QHotkey/).

The documentation was created using [doxygen](http://www.doxygen.org). It includes an HTML-documentation and Qt-Help files that can be included into QtCreator (QtAssistant) to show F1-Help (See [Adding External Documentation](https://doc.qt.io/qtcreator/creator-help.html#adding-external-documentation) for more details).

## Technical

### Requirements

- `C++ 17`
- `CMake 3.16`
- Explicit support is only given down to the latest Qt LTS, but may work with earlier versions, too
- At least the QtGui-Module (a QGuiApplication). Hotkeys on console based applications are not supported (By the operating systems). You can however create a gui application without any visible window.

### Known Limitations

- Only single key/modifier combinations are possible. If using `QKeySequence`, only the first key+modifier of the sequence will be used.
- `Qt::Key` makes no difference between normal numbers and the Numpad numbers. Most keyboards however require this. Thus, you can't register shortcuts for the numpad, unless you use a native shortcut.
- Supports not all keys, but most of the common ones. There are differences between platforms and it depends on the Keyboard-Layout. <kbd>Delete</kbd>, for example, works on windows and mac, but not on X11 (At least on my test machines). I tried to use OS-Functions where possible, but since the `Qt::Key` values need to be converted into native keys, there are some limitations. I can use need such a key, try using the native shortcut.
- The registered keys will be "taken" by QHotkey. This means after a hotkey was cosumend by your application, it will not be sent to the active application. This is done this way by the operating systems and cannot be changed.
- If you get a `QHotkey: Failed to register hotkey. Error: BadAccess (attempt to access private resource denied)` error on X11, this means you are trying to register a hotkey that is private to X11. Those keys simply cannot be registered using the normal API
