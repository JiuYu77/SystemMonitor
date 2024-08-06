#!/bin/bash

echo "delete SystemMonitor.desktop..."
# rm ~/.local/share/applications/SystemMonitor.desktop
rm $HOME/.local/share/applications/SystemMonitor.desktop

echo "delete lib..."
# sed -i '/SystemMonitor/d' $HOME/.bashrc
# sudo rm -r /usr/lib/SystemMonitor
