#!/bin/bash


# Run this to update the launcher file with the current path to the application icon
# APPDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
APPDIR=`pwd`

if [ -w "$APPDIR"/SystemMonitor.desktop ]; then
    echo "set_launcher_icon:..."
    sed -i -e "s@^Icon=.*@Icon=$APPDIR/res/images/logo64.ico@" "$APPDIR"/SystemMonitor.desktop
else
	echo "$APPDIR"/SystemMonitor.desktop is not writable
	exit 1
fi

# Run this to update the launcher file with the current path to the application Exec
if [ -w "$APPDIR"/SystemMonitor.desktop ]; then
	echo "set_launcher_Exec:..."
	sed -i -e "s@^Exec=.*@Exec=$APPDIR/SystemMonitor@" "$APPDIR"/SystemMonitor.desktop
else
	echo "$APPDIR"/SystemMonitor.desktop is not writable
	exit 1
fi

root=`pwd`
echo "root: "$root
path=$root/SystemMonitor.desktop
echo "Symlink_path: "$path

echo "Symlink SystemMonitor.desktop into ~/.local/share/applications/:..."
ln -s $path ~/.local/share/applications/SystemMonitor.desktop

# echo "set_lib..."
# echo export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$root/lib >> $HOME/.bashrc
# sudo ln -s $root/lib /usr/lib/SystemMonitor
