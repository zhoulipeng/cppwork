# Replace space with newline character on global mode.
sed -i "s/ /\n/g" ipslist
sed -e "s/ /\n/g" ipslist > ipslist.txt
# The 'sed -i "s/\n/ /g" ' command is useless, because the sed command will add a newline for every lines.
sed ":a;N;s/\n//g;ta" ipslist
# Replace 'example.com' with 'chat.cs.hnyongxiong.com' in file 'prosody.cfg.lua'
sed -i "s/example.com/chat.cs.hnyongxiong.com/g" prosody.cfg.lua
