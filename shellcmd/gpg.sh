#Step one. 下载原文件与签名文件，最近下载了rust 安装文件，官方提供了gpg验证，第一次验证失败
gpg --verify rust-1.49.0-x86_64-unknown-linux-gnu.tar.gz.asc rust-1.49.0-x86_64-unknown-linux-gnu.tar.gz
:<<!
gpg: 于 2020年12月31日 星期四 22时22分51秒 CST 创建的签名，使用 RSA，钥匙号 FA1BE5FE
gpg: 无法检查签名：找不到公钥
# 报错说找不到公钥,然后发现下载页面提供了公钥文件，下载之，供下一步使用
!
#Step two. 《导入公钥文件》　
gpg --import rust-key.gpg.ascii
#print
:<<!
gpg: 密钥 FA1BE5FE：公钥“Rust Language (Tag and Release Signing Key) <rust-key@rust-lang.org>”已导入
gpg: 合计被处理的数量：1
gpg:           已导入：1  (RSA: 1)
!
#Step three: 再次验证，导入
gpg --verify rust-1.49.0-x86_64-unknown-linux-gnu.tar.gz.asc rust-1.49.0-x86_64-unknown-linux-gnu.tar.gz
#print
:<<!
gpg: 于 2020年12月31日 星期四 22时22分51秒 CST 创建的签名，使用 RSA，钥匙号 FA1BE5FE
gpg: 完好的签名，来自于“Rust Language (Tag and Release Signing Key) <rust-key@rust-lang.org>”
gpg: 警告：这把密钥未经受信任的签名认证！
gpg:       没有证据表明这个签名属于它所声称的持有者。
主钥指纹： 108F 6620 5EAE B0AA A8DD  5E1C 85AB 96E6 FA1B E5FE
!
