#!/bin/bash


# Run this to update the launcher file with the current path to the application icon
APPDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [ -w "$APPDIR"/SystemMonitor.desktop ]; then
    echo "set_launcher_icon:..."
    sed -i -e "s@^Icon=.*@Icon=$APPDIR/res/images/logo64.ico@" "$APPDIR"/SystemMonitor.desktop
else
	echo "$APPDIR"/SystemMonitor.desktop is not writable
	exit 1
fi

# Run this to update the launcher file with the current path to the application Exec
APPDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [ -w "$APPDIR"/SystemMonitor.desktop ]; then
	echo "set_launcher_Exec:..."
	sed -i -e "s@^Exec=.*@Exec=$APPDIR/bin/SystemMonitor@" "$APPDIR"/SystemMonitor.desktop
else
	echo "$APPDIR"/SystemMonitor.desktop is not writable
	exit 1
fi

filename=`pwd`/$0
root=`dirname $filename`
echo "root: "$root

echo "Symlink SystemMonitor.desktop into ~/.local/share/applications/:..."
ln -s $root/SystemMonitor.desktop ~/.local/share/applications/SystemMonitor.desktop

echo "set_lib..."
echo export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$root/lib >> $HOME/.bashrc
