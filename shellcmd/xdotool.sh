# xserver desktop send short key to window program
# This example is send key F5 to thunar that is a filemanager.
xdotool search --name 永雄云盘 windowactivate  --sync %1 key F5 windowactivate $(xdotool getactivewindow)
