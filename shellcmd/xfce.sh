./thunar/thunar-preferences.c

./.config/xfce4/xfconf/xfce-perchannel-xml/thunar.xml

监控 某个频道 
xfconf-query -c thunar -m
xfconf-query --channel thunar --property /misc-thumbnail-mode
关闭预览
1.
xfconf-query --channel thunar --property /misc-thumbnail-mode --create --type string --set THUNAR_THUMBNAIL_MODE_NEVER

2.
or editing in :
xfce4-settings-editor

3. 关闭图片预览
thunar-settings
