# PassDepot

#### Video Demo:  <URL HERE>

## Project Description

PassDepot is an offline password manager for desktop platforms (currently only tested for Windows10 and Debian12). Its 
goal is to give users a simple way to store their private information, in a secure way. It encrypts plaintext into 
ciphertext and stores it in the local machine. When it comes to securing that database file, the onus is on the user 
since -as mentioned before- this is an offline password manager.

PassDepot has a simple GUI which utilizes immediate-mode GUI library, [Dear ImGui](https://github.com/ocornut/imgui). 
It uses cryptographic tools provided by [libsodium](https://github.com/jedisct1/libsodium). For database operation, 
[SQLite](https://github.com/sqlite/sqlite).

## High-level Details

PassDepot has five source files, excluding main.cpp file, and has five header files. Each of which contains a class in 
it:

- PassDepot.h
- PDWindow.h
- PDGUI.h
- PDDatabase.h
- PDSecurity.h

Most of these classes acts as a wrapper for the third-party libraries mentioned in the previous section.

### PassDepot.h

PassDepot.h file contains PassDepot class. This class is responsible of creating and containing all of the other 
classes. It also has a single function called Run(), which is the main while loop that the program runs in until 
instructed otherwise. PassDepot class is constructed in main.cpp file and its Run() function is called there as well. 
Other than these, this class has no responsibilities.

### PDWindow.h

PDWindow.h is the file where PDWindow class is declared. PDWindow handles window operations such as creating, moving, 
resizing, minimizing, maximizing and closing a window. "Window" is not to be confused with the windows created by 
Dear ImGui. Dear ImGui requires a rendering platform backend, so it can draw its GUI elements on it. For this purpose, 
PDWindow creates a [GLFW](https://github.com/glfw/glfw) window. GLFW also handles input events.

### PDGUI.h

PDGUI is a GUI manager class that handles Dear Imgui elements. Contains PDWindow, PDDatabase and PDSecurity and 
therefore this class is the starting point for most of the operations related to user input. A user is first faced with 
the Welcome() window. Welcome() has a welcome text and two radio buttons: Login() and Register(). Both of these 
operations have similar behaviours, where a user is expected to enter some input (username and password) and is also 
expected to press the respected buttons (login or register). After input validity checks and authorization -provided by 
PDDatabase and PDSecurity-, program derives a key from the user's master password, loads the database into memory and 
decrypts it. Then flow continues onto Depot(). Depot() has a table and four radio buttons. Table has five columns; 
EntryId, Title, Username, Password and Notes. Four radio buttons are: AddNewEntry(), EditEntry(), DeleteEntry() and 
JustDepot. As the names suggest, each does what they named after.

AddNewEntry() has four input fields along with two buttons. Input fields corresponds to Title, Username, Password and 
Notes whereas buttons to Save New Entry and Clear. Clear, clears the input fields. After some or all of the input fields 
are filled and Save New Entry button is pressed, user input is concatenated into a single string and PDGUI calls 
PDSecurity for encryption related tasks. Once encryption has completed, PDGUI calls for PDDatabase so the entry is 
recorded/inserted into database.

EditEntry() differs from AddNewEntry() in two ways: Instead of inserting a new entry, it updates an existing one and it 
expects an EntryId to be updated.

DeleteEntry(), just like the EditEntry() expects an EntryId to be deleted. When delete button is pressed, it pops a "Are 
you sure?" warning up in order to avoid mistakenly deleting entries.

JustDepot radio button is a no-operation. It only show the table.

PassDepot also has a custom TitleBar() and a custom ResizeHandle(), both of which are made with Dear ImGui. 
ResizeHandle() is a child window that is placed at the bottom right. When it is dragged and when PassDepot is not 
maximized, it resizes the window. While resizing, it also changes the cursor into resize icon. TitleBar() is another 
window shaped like an actual title bar. On its left it has OptionsButton() at center name of the program and on right 
it has three buttons; minimize, maximize and close.

Most of the operations whether successful or not, pops a status message up, informing the user. This pop-up can be 
disabled from the OptionsButton(). OptionsButton() also has a section called Themes, which allows users to pick from one 
of the three Dear ImGui themes. Themes are called Classic, Dark and Light. Finally, it has a button called Logout, which 
only appears if the user is logged in and when clicked, logs user out and takes the flow back to Welcome() window.

For fonts, PDGUI uses [Victor Mono](https://github.com/rubjo/victor-mono).

### PDDatabase.h

This file contains PDDatabase class, which is a wrapper for SQLite operations. PDDatabase is responsible of creating and 
opening the .db database file. It creates two tables in that file, User and Depot. User table has four columns: Id, 
Username, MasterPassword and Salt. Salt is used to derive a key from MasterPassword, that is used for encryption and 
decryption. Depot has five columns: Id, CipherText, Nonce, EntryInfo, UserId. UserId is a foreign key that references Id 
in User table. Most of the functions in this class involve database read and/or write. Mainly insert, update, delete and 
just select data from the database. Some verification also happens here, such as DoesUsernameExists() and 
DoesEntryExists().

### PDSecurity.h

PDSecurity.h contains PDSecurity class and same as PDDatabase, it serves as a wrapper for libsodium. Main 
responsibilities of PDSecurity are; encryption, decryption, deriving a key, hashing master passwords and verifying 
master passwords. While doing so, it can generate random salt and nonce. This class will also be responsible for virtual 
memory lock but currently this feature is not yet implemented. libsodium's password hashing API pwhash* currently 
utilizes [Argon2](https://github.com/P-H-C/phc-winner-argon2) algorithm by default and for encryption, [XSalsa20](
https://doc.libsodium.org/advanced/stream_ciphers/xsalsa20) stream cipher.