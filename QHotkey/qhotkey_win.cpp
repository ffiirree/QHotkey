#include "qhotkey.h"
#include "qhotkey_p.h"
#include <qt_windows.h>
#include <QDebug>

static QString formatWinError(DWORD winError);

bool QHotkeyPrivate::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(eventType);
	Q_UNUSED(result);

	MSG* msg = static_cast<MSG*>(message);
	if(msg->message == WM_HOTKEY) {
		QHotkey::NativeShortcut shortcut;
		shortcut.key = HIWORD(msg->lParam);
		shortcut.mods = LOWORD(msg->lParam);
		this->activateShortcut(shortcut);
	}

	return false;
}

quint32 QHotkeyPrivate::nativeKeycode(Qt::Key keycode)
{
	if(keycode <= 0xFFFF) {//Try to obtain the key from it's "character"
		const SHORT vk = VkKeyScanW(keycode);
		if(vk > -1)
			return LOBYTE(vk);
	}

	//find key from "switch/case" --> Only finds a very small subset of keys
	switch (keycode)
	{
	case Qt::Key_Escape:
		return VK_ESCAPE;
	case Qt::Key_Tab:
	case Qt::Key_Backtab:
		return VK_TAB;
	case Qt::Key_Backspace:
		return VK_BACK;
	case Qt::Key_Return:
	case Qt::Key_Enter:
		return VK_RETURN;
	case Qt::Key_Insert:
		return VK_INSERT;
	case Qt::Key_Delete:
		return VK_DELETE;
	case Qt::Key_Pause:
		return VK_PAUSE;
	case Qt::Key_Print:
		return VK_PRINT;
	case Qt::Key_Clear:
		return VK_CLEAR;
	case Qt::Key_Home:
		return VK_HOME;
	case Qt::Key_End:
		return VK_END;
	case Qt::Key_Left:
		return VK_LEFT;
	case Qt::Key_Up:
		return VK_UP;
	case Qt::Key_Right:
		return VK_RIGHT;
	case Qt::Key_Down:
		return VK_DOWN;
	case Qt::Key_PageUp:
		return VK_PRIOR;
	case Qt::Key_PageDown:
		return VK_NEXT;
	case Qt::Key_CapsLock:
		return VK_CAPITAL;
	case Qt::Key_NumLock:
		return VK_NUMLOCK;
	case Qt::Key_ScrollLock:
		return VK_SCROLL;
	case Qt::Key_F1:
		return VK_F1;
	case Qt::Key_F2:
		return VK_F2;
	case Qt::Key_F3:
		return VK_F3;
	case Qt::Key_F4:
		return VK_F4;
	case Qt::Key_F5:
		return VK_F5;
	case Qt::Key_F6:
		return VK_F6;
	case Qt::Key_F7:
		return VK_F7;
	case Qt::Key_F8:
		return VK_F8;
	case Qt::Key_F9:
		return VK_F9;
	case Qt::Key_F10:
		return VK_F10;
	case Qt::Key_F11:
		return VK_F11;
	case Qt::Key_F12:
		return VK_F12;
	case Qt::Key_F13:
		return VK_F13;
	case Qt::Key_F14:
		return VK_F14;
	case Qt::Key_F15:
		return VK_F15;
	case Qt::Key_F16:
		return VK_F16;
	case Qt::Key_F17:
		return VK_F17;
	case Qt::Key_F18:
		return VK_F18;
	case Qt::Key_F19:
		return VK_F19;
	case Qt::Key_F20:
		return VK_F20;
	case Qt::Key_F21:
		return VK_F21;
	case Qt::Key_F22:
		return VK_F22;
	case Qt::Key_F23:
		return VK_F23;
	case Qt::Key_F24:
		return VK_F24;
	case Qt::Key_Menu:
		return VK_APPS;
	case Qt::Key_Help:
		return VK_HELP;
	case Qt::Key_Space:
		return VK_SPACE;
	case Qt::Key_Asterisk:
		return VK_MULTIPLY;
	case Qt::Key_Plus:
		return VK_ADD;
	case Qt::Key_Comma:
		return VK_SEPARATOR;
	case Qt::Key_Minus:
		return VK_SUBTRACT;
	case Qt::Key_Slash:
		return VK_DIVIDE;
	case Qt::Key_MediaNext:
		return VK_MEDIA_NEXT_TRACK;
	case Qt::Key_MediaPrevious:
		return VK_MEDIA_PREV_TRACK;
	case Qt::Key_MediaPlay:
		return VK_MEDIA_PLAY_PAUSE;
	case Qt::Key_MediaStop:
		return VK_MEDIA_STOP;
	case Qt::Key_VolumeDown:
		return VK_VOLUME_DOWN;
	case Qt::Key_VolumeUp:
		return VK_VOLUME_UP;
	case Qt::Key_VolumeMute:
		return VK_VOLUME_MUTE;
	case Qt::Key_Mode_switch:
		return VK_MODECHANGE;
	case Qt::Key_Select:
		return VK_SELECT;
	case Qt::Key_Printer:
		return VK_PRINT;
	case Qt::Key_Execute:
		return VK_EXECUTE;
	case Qt::Key_Sleep:
		return VK_SLEEP;
	case Qt::Key_Period:
		return VK_DECIMAL;
	case Qt::Key_Play:
		return VK_PLAY;
	case Qt::Key_Cancel:
		return VK_CANCEL;

	case Qt::Key_Forward:
		return VK_BROWSER_FORWARD;
	case Qt::Key_Refresh:
		return VK_BROWSER_REFRESH;
	case Qt::Key_Stop:
		return VK_BROWSER_STOP;
	case Qt::Key_Search:
		return VK_BROWSER_SEARCH;
	case Qt::Key_Favorites:
		return VK_BROWSER_FAVORITES;
	case Qt::Key_HomePage:
		return VK_BROWSER_HOME;

	case Qt::Key_LaunchMail:
		return VK_LAUNCH_MAIL;
	case Qt::Key_LaunchMedia:
		return VK_LAUNCH_MEDIA_SELECT;
	case Qt::Key_Launch0:
		return VK_LAUNCH_APP1;
	case Qt::Key_Launch1:
		return VK_LAUNCH_APP2;

	case Qt::Key_Massyo:
		return VK_OEM_FJ_MASSHOU;
	case Qt::Key_Touroku:
		return VK_OEM_FJ_TOUROKU;

		// numbers
	case Qt::Key_0:
	case Qt::Key_1:
	case Qt::Key_2:
	case Qt::Key_3:
	case Qt::Key_4:
	case Qt::Key_5:
	case Qt::Key_6:
	case Qt::Key_7:
	case Qt::Key_8:
	case Qt::Key_9:
		return keycode;

		// letters
	case Qt::Key_A:
	case Qt::Key_B:
	case Qt::Key_C:
	case Qt::Key_D:
	case Qt::Key_E:
	case Qt::Key_F:
	case Qt::Key_G:
	case Qt::Key_H:
	case Qt::Key_I:
	case Qt::Key_J:
	case Qt::Key_K:
	case Qt::Key_L:
	case Qt::Key_M:
	case Qt::Key_N:
	case Qt::Key_O:
	case Qt::Key_P:
	case Qt::Key_Q:
	case Qt::Key_R:
	case Qt::Key_S:
	case Qt::Key_T:
	case Qt::Key_U:
	case Qt::Key_V:
	case Qt::Key_W:
	case Qt::Key_X:
	case Qt::Key_Y:
	case Qt::Key_Z:
		return keycode;

	default:
		return 0;
	}
}

quint32 QHotkeyPrivate::nativeModifiers(Qt::KeyboardModifiers modifiers)
{
	quint32 native = 0;
	if (modifiers & Qt::ShiftModifier)
		native |= MOD_SHIFT;
	if (modifiers & Qt::ControlModifier)
		native |= MOD_CONTROL;
	if (modifiers & Qt::AltModifier)
		native |= MOD_ALT;
	if (modifiers & Qt::MetaModifier)
		native |= MOD_WIN;
	return native;
}

bool QHotkeyPrivate::registerShortcut(QHotkey::NativeShortcut shortcut)
{
	BOOL ok = RegisterHotKey(NULL,
							 shortcut.key ^ shortcut.mods,
							 shortcut.mods,
							 shortcut.key);
	if(ok)
		return true;
	else {
		qWarning() << "Failed to register hotkey. Error:" << formatWinError(::GetLastError());
		return false;
	}
}

bool QHotkeyPrivate::unregisterShortcut(QHotkey::NativeShortcut shortcut)
{
	BOOL ok = UnregisterHotKey(NULL, shortcut.key ^ shortcut.mods);
	if(ok)
		return true;
	else {
		qWarning() << "Failed to unregister hotkey. Error:" << formatWinError(::GetLastError());
		return false;
	}
}

static QString formatWinError(DWORD winError)
{
	wchar_t *buffer = NULL;
	DWORD num = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
							   NULL,
							   winError,
							   0,
							   (LPWSTR)&buffer,
							   0,
							   NULL);
	if(buffer) {
		QString res = QString::fromWCharArray(buffer, num);
		LocalFree(buffer);
		return res;
	} else
		return QString();
}