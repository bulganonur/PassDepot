# PassDepot - WIP

PassDepot is an offline password manager for desktop platforms (currently only tested for Windows10 and Debian12). Its 
goal is to give users a simple way to store their private information, in a secure way.

PassDepot has a simple GUI which utilizes immediate-mode GUI library, [Dear ImGui](https://github.com/ocornut/imgui). 
It uses cryptographic tools provided by [libsodium](https://github.com/jedisct1/libsodium). For database operation, 
[SQLite](https://github.com/sqlite/sqlite).

For fonts, PassDepot uses [Victor Mono](https://github.com/rubjo/victor-mono).

Currently, PassDepot utilizes libsodium's pwhash* and secretbox* APIs for password hashing and encryption respectively. 
[Argon2](https://github.com/P-H-C/phc-winner-argon2) is the default algorithm for pwhash* whereas [XSalsa20](
https://doc.libsodium.org/advanced/stream_ciphers/xsalsa20) stream cipher for secretbox*.

## Build && Run

PassDepot uses [xmake](https://github.com/xmake-io/xmake) to handle its dependecies and build. Once you get xmake, run below command in the project directory:

``
xmake run
``

This should first prompt you for package installation then build PassDepot and run it.


## Showcase / Screenshots

todo: ---